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

/* Includes ----------------------------------------------------------- */
#include "hall.h"
/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
#ifdef DEBUG
#define __ASSERT(_EXPR_, _RET_VAL_) \
	do                              \
	{                               \
		if (!(_EXPR_))              \
			return _RET_VAL_;       \
	} while (0)
#else
#define __ASSERT(_EXPR_, _RET_VAL_) ((void)0)
#endif

/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static hall_t hall;
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
hall_status_t hall_init(hall_t hall_init)
{
	__ASSERT((hall_init.htim != NULL) ||
			 (hall_init.tim_clear_flag_irq != NULL) ||
			 (hall_init.tim_get_counter != NULL) ||
			 (hall_init.tim_read_capture != NULL) ||
			 (hall_init.tim_reset_counter != NULL) ||
			 (hall_init.tim_set_output_compare != NULL) ||
			 (hall_init.tim_start_input_capture_irq != NULL) ||
			 (hall_init.tim_start_irq != NULL) ||
			 (hall_init.tim_start_output_compare_irq != NULL), HALL_ERROR);

	hall = hall_init;

	return HALL_OK;
}
hall_status_t hall_start()
{
	hall.tim_start_irq(hall.htim);
	hall.tim_start_input_capture_irq(hall.htim, hall.capture_channel);
	hall.tim_start_output_compare_irq(hall.htim, hall.compare_channel);

	return HALL_OK;
}

hall_status_t hall_read_capture(uint32_t *capture_value)
{
	__ASSERT((capture_value != NULL), HALL_ERROR);

	*capture_value = hall.tim_read_capture(hall.htim, hall.capture_channel);
	return HALL_OK;
}

hall_status_t hall_set_output_compare(uint32_t value_ccr)
{

	hall.tim_set_output_compare(hall.htim, hall.compare_channel, value_ccr);

	return HALL_OK;
}

hall_status_t hall_reset_flag_capture_interrupt()
{
	hall.tim_clear_flag_irq(hall.htim, hall.flag_channel_irq[0]);
	return HALL_OK;
}
hall_status_t hall_reset_flag_compare_interrupt()
{
	hall.tim_clear_flag_irq(hall.htim, hall.flag_channel_irq[1]);
	return HALL_OK;
}

hall_status_t hall_reset_counter()
{
	hall.tim_reset_counter(hall.htim);
	return HALL_OK;
}

hall_status_t hall_get_counter(uint32_t *get_counter_value)
{
	*get_counter_value = hall.tim_get_counter(hall.htim);
	return HALL_OK;
}

/* Private function definitions --------------------------------------- */
/* End of file -------------------------------------------------------- */
