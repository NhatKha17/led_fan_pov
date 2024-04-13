/**
 * @file       buzzer.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       23/03/2023
 * @author     Kha Nguyen
 *
 * @brief      Driver for Buzzer
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_BUZZER_H_
#define INC_BUZZER_H_

/* Includes ----------------------------------------------------------- */
#include "bsp_timer.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
	BUZZER_STATUS_OK = 0U,
	BUZZER_STATUS_ERROR,
}
buzzer_status_t;

typedef struct
{
	TIM_HandleTypeDef *htim;
	DMA_HandleTypeDef *hdma_tim_ch1;
	uint32_t pwn_channel;
	uint16_t tim_prescale;
	uint16_t tim_autoreload;
	uint8_t buzzer_volumn;
	bool (*pwm_start)(TIM_HandleTypeDef *htim, uint32_t pwm_channel);
	bool (*pwm_stop)(TIM_HandleTypeDef *htim, uint32_t pwm_channel);
	void (*pwm_set_compare)(TIM_HandleTypeDef *htim,  uint32_t output_channel, uint32_t value_ccr);
	void (*set_prescale)(TIM_HandleTypeDef *htim, uint32_t prescale);
	void (*set_autoreload)(TIM_HandleTypeDef *htim, uint32_t autoreload);
	void (*delay)(uint32_t ms);
}
buzzer_t;
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief  		Initialize for driver
 *
 * @param[in]   sys_buzzer	struct at file any system use buzzer that save information to pass to buzzer_t
 *
 * @attention 	Should call this function first before using other function
 *
 * @return		OK		successed
 * @return		ERROR	failed
 */
bool buzzer_init(buzzer_t sys_buzzer);

/**
 * @brief  		Choose music for buzzer
 *
 * @param[in]   choose_music      which tune to be played
 *
 * @attention   type buzzer_music_t is declared in this file with a enum
 *
 * @return		None
 */
void buzzer_music();

#endif /* INC_BUZZER_H_ */
