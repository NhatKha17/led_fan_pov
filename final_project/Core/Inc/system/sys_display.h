/**
 * @file       sys_display.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       31/03/2024
 * @author     Kha Nguyen
 *
 * @brief      system display for display on led fan
 */
#ifndef INC_SYSTEM_SYS_DISPLAY_H_
#define INC_SYSTEM_SYS_DISPLAY_H_

/* Includes ----------------------------------------------------------- */
#include "hall.h"
#include "ws2812.h"
/* Public defines ----------------------------------------------------- */
/*define color defaut 1*/
#define SYS_DISPLAY_COLOR_TIME_GREEN_1				(255)
#define SYS_DISPLAY_COLOR_TIME_RED_1				(255)
#define SYS_DISPLAY_COLOR_TIME_BLUE_1				(255)
#define SYS_DISPLAY_COLOR_ALARM_GREEN_1				(255)
#define SYS_DISPLAY_COLOR_ALARM_RED_1				(255)
#define SYS_DISPLAY_COLOR_ALARM_BLUE_1				(255)
#define SYS_DISPLAY_COLOR_OUTLINE_GREEN_1			(0)
#define SYS_DISPLAY_COLOR_OUTLINE_RED_1				(255)
#define SYS_DISPLAY_COLOR_OUTLINE_BLUE_1			(255)
#define SYS_DISPLAY_COLOR_COLON_GREEN_1				(255)
#define SYS_DISPLAY_COLOR_COLON_RED_1				(0)
#define SYS_DISPLAY_COLOR_COLON_BLUE_1				(255)

/*define color default 2*/
#define SYS_DISPLAY_COLOR_TIME_GREEN_2				(0)
#define SYS_DISPLAY_COLOR_TIME_RED_2				(0)
#define SYS_DISPLAY_COLOR_TIME_BLUE_2				(255)
#define SYS_DISPLAY_COLOR_ALARM_GREEN_2				(0)
#define SYS_DISPLAY_COLOR_ALARM_RED_2				(0)
#define SYS_DISPLAY_COLOR_ALARM_BLUE_2				(255)
#define SYS_DISPLAY_COLOR_OUTLINE_GREEN_2			(0)
#define SYS_DISPLAY_COLOR_OUTLINE_RED_2				(0)
#define SYS_DISPLAY_COLOR_OUTLINE_BLUE_2			(255)
#define SYS_DISPLAY_COLOR_COLON_GREEN_2				(0)
#define SYS_DISPLAY_COLOR_COLON_RED_2				(0)
#define SYS_DISPLAY_COLOR_COLON_BLUE_2				(255)

/* Public enumerate/structure ----------------------------------------- */
/* ===============get data to display================================== */
/*@brief struct save data that convert from epoch sys manage pass into*/
typedef struct
{
		uint8_t hours;		 			/* Hours parameter, 24Hour mode, 00 to 23 */
		uint8_t minutes;	  			/* Minutes parameter, from 00 to 59 */
		uint8_t seconds;	  			/* Seconds parameter, from 00 to 59 */
		uint8_t dow;		  			/* Day in a week, from monday to sunday 1-7 */
		uint8_t date;		  			/* Date in a month, 1 to 31 */
		uint8_t month;		  			/* Month in a year, 1 to 12 */
		uint32_t year;		  			/* Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
}
sys_display_time_t;

typedef enum
{
	SYS_DISPLAY_OK = 0U,
	SYS_DISPLAY_ERROR,
}
sys_display_status_t;

typedef enum
{
	SYS_DISPLAY_COLOR_DEFAULT_1 = 1U,
	SYS_DISPLAY_COLOR_DEFAULT_2,
}
sys_display_color_default_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief  		Start the system display module.
 *
 * @param[in]   None
 *
 * @attention 	This function should be called first before using other functions in the system display module.
 *
 * @return		The status of the display start operation.
 */
sys_display_status_t sys_display_start();

/**
 * @brief  		Control the system display fan.
 *
 * @param[in]   data_time   The time data to be displayed.
 * @param[in]   flag_alarm  A boolean indicating whether an alarm is active or not.
 *
 * @return		None
 */
void sys_display_fan(sys_display_time_t data_time, bool flag_alarm);

/**
 * @brief  		Set the brightness level of the system display.
 *
 * @param[in]   sys_brightness   The brightness level to be set.
 *
 * @return		The status of the brightness setting operation.
 */
sys_display_status_t sys_display_set_brightness(uint8_t sys_brightness);

/**
 * @brief  		Set the color of the system display to a default color.
 *
 * @param[in]   color_no   The predefined color number to be set.
 *
 * @return		The status of the color setting operation.
 */
sys_display_status_t sys_display_set_color(sys_display_color_default_t color_no);

#endif /* INC_SYSTEM_SYS_DISPLAY_H_ */
 /* End of file -------------------------------------------------------- */
