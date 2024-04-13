/**
 * @file        bsp_timer.c
 * @copyright
 * @license
 * @version     1.0.0
 * @date       	23/03/2023
 * @author    	Kha Nguyen
 *
 * @brief       Board Support Package for Timer and PWM Peripheral
 *
 * @note        None
 * @example     None
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_BSP_TIMER_H_
#define INC_BSP_TIMER_H_

/* Includes ----------------------------------------------------------- */
#include "main.h"
#include "stdbool.h"
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */

/**
 * @brief       Start PWM generation on a specific channel of the timer.
 *
 * @param[in]   htim            Pointer to the timer handle structure.
 * @param[in]   pwm_channel     Channel for PWM generation.
 *
 * @return      True if PWM start was successful, false otherwise.
 */
bool bsp_pwm_start(TIM_HandleTypeDef *htim, uint32_t pwm_channel);

/**
 * @brief       Stop PWM generation on a specific channel of the timer.
 *
 * @param[in]   htim            Pointer to the timer handle structure.
 * @param[in]   pwm_channel     Channel for PWM generation.
 *
 * @return      True if PWM stop was successful, false otherwise.
 */
bool bsp_pwm_stop(TIM_HandleTypeDef *htim, uint32_t pwm_channel);

/**
 * @brief       Set the output compare value for a specific channel of the timer.
 *
 * @param[in]   htim                Pointer to the timer handle structure.
 * @param[in]   compare_channel     Output compare channel.
 * @param[in]   value_ccr           Value to be set in the compare register.
 *
 * @return      None
 */
void bsp_tim_set_output_compare(TIM_HandleTypeDef *htim, uint32_t compare_channel, uint32_t value_ccr);

/**
 * @brief       Set the prescaler value for the timer.
 *
 * @param[in]   htim        Pointer to the timer handle structure.
 * @param[in]   prescale    Prescaler value to be set.
 *
 * @return      None
 */
void bsp_tim_set_prescale(TIM_HandleTypeDef *htim, uint32_t prescale);

/**
 * @brief       Set the auto-reload value for the timer.
 *
 * @param[in]   htim            Pointer to the timer handle structure.
 * @param[in]   autoreload      Auto-reload value to be set.
 *
 * @return      None
 */
void bsp_tim_set_autoreload(TIM_HandleTypeDef *htim, uint32_t autoreload);

/**
 * @brief       Get the current counter value of the timer.
 *
 * @param[in]   htim    Pointer to the timer handle structure.
 *
 * @return      Current counter value of the timer.
 */
uint32_t bsp_tim_get_counter(TIM_HandleTypeDef *htim);

/**
 * @brief       Reset the counter value of the timer.
 *
 * @param[in]   htim    Pointer to the timer handle structure.
 *
 * @return      None
 */
void bsp_tim_reset_counter(TIM_HandleTypeDef *htim);

/**
 * @brief       Read the capture value for a specific capture channel of the timer.
 *
 * @param[in]   htim                Pointer to the timer handle structure.
 * @param[in]   capture_channel     Capture channel.
 *
 * @return      Capture value for the specified channel.
 */
uint32_t bsp_tim_read_capture(TIM_HandleTypeDef *htim, uint32_t capture_channel);

/**
 * @brief       Clear the specified flag or IRQ of the timer.
 *
 * @param[in]   htim                Pointer to the timer handle structure.
 * @param[in]   flag_channel_irp    Flag or IRQ to be cleared.
 *
 * @return      None
 */
void bsp_tim_clear_flag_irq(TIM_HandleTypeDef *htim, uint32_t flag_channel_irp);

/**
 * @brief       Delay execution for the specified time in milliseconds.
 *
 * @param[in]   ms      Time to delay in milliseconds.
 *
 * @return      None
 */
void bsp_delay(uint32_t ms);

/**
 * @brief       Start input capture IRQ for a specific capture channel of the timer.
 *
 * @param[in]   htim                Pointer to the timer handle structure.
 * @param[in]   capture_channel     Capture channel.
 *
 * @return      None
 */
void bsp_tim_start_input_capture_irq(TIM_HandleTypeDef *htim, uint32_t capture_channel);

/**
 * @brief       Start output compare IRQ for a specific compare channel of the timer.
 *
 * @param[in]   htim                Pointer to the timer handle structure.
 * @param[in]   compare_channel     Compare channel.
 *
 * @return      None
 */
void bsp_tim_start_output_compare_irq(TIM_HandleTypeDef *htim, uint32_t compare_channel);

/**
 * @brief       Start IRQ processing for the timer.
 *
 * @param[in]   htim    Pointer to the timer handle structure.
 *
 * @return      None
 */
void bsp_tim_start_irq(TIM_HandleTypeDef *htim);
#endif /* INC_BSP_TIMER_H_ */
