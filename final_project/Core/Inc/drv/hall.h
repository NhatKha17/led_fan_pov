/**
 * @file       hall.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       31/03/2024
 * @author     Kha Nguyen
 *
 * @brief      Hall driver
 *
 **/

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_DRV_HALL_H_
#define INC_DRV_HALL_H_

/* Includes ----------------------------------------------------------- */
#include "bsp_timer.h"
#include "stdbool.h"
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef TIM_HandleTypeDef tim_serial_t;
typedef enum
{
	HALL_OK = 0U,
	HALL_ERROR,
} hall_status_t;
/**
 * @brief Struct for hall object
 */
typedef struct
{
	tim_serial_t *htim;

	uint32_t capture_channel;

	uint32_t compare_channel;

	uint32_t flag_channel_irq[2]; // flag_channel_irq[0] save flag irq channel capture, flag_channel_irq[1] save flag irq channel compare

	uint32_t (*tim_get_counter)(TIM_HandleTypeDef *htim);
	void (*tim_reset_counter)(TIM_HandleTypeDef *htim);
	uint32_t (*tim_read_capture)(TIM_HandleTypeDef *htim, uint32_t capture_channel);
	void (*tim_set_output_compare)(TIM_HandleTypeDef *htim, uint32_t compare_channel, uint32_t value_ccr);
	void (*tim_clear_flag_irq)(TIM_HandleTypeDef *htim, uint32_t flag_channel_irp);
	void (*tim_start_irq)(TIM_HandleTypeDef *htim);
	void (*tim_start_output_compare_irq)(TIM_HandleTypeDef *htim, uint32_t compare_channel);
	void (*tim_start_input_capture_irq)(TIM_HandleTypeDef *htim, uint32_t capture_channel);
} hall_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Initialize the Hall Effect sensor module.
 *
 * @param[in]   hall_init   Configuration parameters for the Hall Effect sensor.
 *
 * @return      The status of the Hall Effect sensor initialization.
 */
hall_status_t hall_init(hall_t hall_init);

/**
 * @brief       Start the Hall Effect sensor.
 *
 * @return      The status of the Hall Effect sensor start operation.
 */
hall_status_t hall_start();

/**
 * @brief       Read the captured value from the Hall Effect sensor.
 *
 * @param[out]  capture_value   Pointer to store the captured value.
 *
 * @return      The status of the capture value reading operation.
 */
hall_status_t hall_read_capture(uint32_t *capture_value);

/**
 * @brief       Set the output compare value for the Hall Effect sensor.
 *
 * @param[in]   value_ccr   Value to be set in the compare register.
 *
 * @return      The status of the output compare setting operation.
 */
hall_status_t hall_set_output_compare(uint32_t value_ccr);

/**
 * @brief       Reset the capture interrupt flag of the Hall Effect sensor.
 *
 * @return      The status of the capture interrupt flag reset operation.
 */
hall_status_t hall_reset_flag_capture_interrupt();

/**
 * @brief       Reset the compare interrupt flag of the Hall Effect sensor.
 *
 * @return      The status of the compare interrupt flag reset operation.
 */
hall_status_t hall_reset_flag_compare_interrupt();

/**
 * @brief       Reset the counter of the Hall Effect sensor.
 *
 * @return      The status of the counter reset operation.
 */
hall_status_t hall_reset_counter();

/**
 * @brief       Get the counter value of the Hall Effect sensor.
 *
 * @param[out]  get_counter_value   Pointer to store the counter value.
 *
 * @return      The status of the counter value retrieval operation.
 */
hall_status_t hall_get_counter(uint32_t *get_counter_value);

#endif /* INC_DRV_HALL_H_ */
/* End of file -------------------------------------------------------- */
