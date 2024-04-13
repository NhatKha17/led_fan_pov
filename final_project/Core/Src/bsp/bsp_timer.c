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

/* Includes ----------------------------------------------------------- */
#include <bsp_timer.h>
/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */

bool bsp_pwm_start(TIM_HandleTypeDef *htim, uint32_t pwm_channel)
{
    if (HAL_TIM_PWM_Start(htim, pwm_channel) != HAL_OK)
    {
        return false;
    }
    return true;
}

bool bsp_pwm_stop(TIM_HandleTypeDef *htim, uint32_t pwm_channel)
{
    if (HAL_TIM_PWM_Stop(htim, pwm_channel) != HAL_OK)
    {
        return false;
    }
    return true;
}

void bsp_tim_set_prescale(TIM_HandleTypeDef *htim, uint32_t prescale)
{
    __HAL_TIM_SET_PRESCALER(htim, prescale);
}

void bsp_tim_set_autoreload(TIM_HandleTypeDef *htim, uint32_t autoreload)
{
    __HAL_TIM_SET_AUTORELOAD(htim, autoreload);
}

void bsp_tim_set_output_compare(TIM_HandleTypeDef *htim, uint32_t output_channel, uint32_t value_ccr)
{
    __HAL_TIM_SetCompare(htim, output_channel, value_ccr);
}

uint32_t bsp_tim_get_counter(TIM_HandleTypeDef *htim)
{
    return __HAL_TIM_GET_COUNTER(htim);
}

void bsp_tim_reset_counter(TIM_HandleTypeDef *htim)
{
    __HAL_TIM_SET_COUNTER(htim, 0);
}

uint32_t bsp_tim_read_capture(TIM_HandleTypeDef *htim, uint32_t capture_channel)
{
    return HAL_TIM_ReadCapturedValue(htim, capture_channel);
}

void bsp_tim_clear_flag_irq(TIM_HandleTypeDef *htim, uint32_t flag_channel_irp)
{
    __HAL_TIM_CLEAR_FLAG(htim, flag_channel_irp);
}
void bsp_tim_start_input_capture_irq(TIM_HandleTypeDef *htim, uint32_t capture_channel)
{
    HAL_TIM_IC_Start_IT(htim, capture_channel); // start IC interrupt
}

void bsp_tim_start_output_compare_irq(TIM_HandleTypeDef *htim, uint32_t compare_channel)
{
    HAL_TIM_OC_Start_IT(htim, compare_channel); // start OC interrupt
}

void bsp_tim_start_irq(TIM_HandleTypeDef *htim)
{
    HAL_TIM_Base_Start_IT(htim);
}
void bsp_delay(uint32_t ms)
{
    HAL_Delay(ms);
}

/* End of file -------------------------------------------------------- */
