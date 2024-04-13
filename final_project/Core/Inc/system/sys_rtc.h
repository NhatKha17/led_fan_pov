/**
 * @file       sys_rtc.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       23/03/2024
 * @author     Kha Nguyen
 *
 * @brief      system rtc handle setting , get data time from ds1307
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_SYSTEM_SYS_RTC_H_
#define INC_SYSTEM_SYS_RTC_H_

/* Includes ----------------------------------------------------------- */
#include "ds1307.h"
#include "buzzer.h"
#include "cjmcu75.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
	SYS_RTC_OK = 0U,
	SYS_RTC_ERROR,
} sys_rtc_status_t;
typedef enum
{
	SYS_RTC_MODE_24H = 0U,
	SYS_RTC_MODE_12H,
} sys_rtc_mode_time_t;

typedef struct
{
	uint8_t hours;	 /* Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t minutes; /* Minutes parameter, from 00 to 59 */
	uint8_t seconds; /* Seconds parameter, from 00 to 59 */
} sys_rtc_alarm_time_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */
typedef ds1307_time_t sys_rtc_time_t;

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Start the system RTC (Real-Time Clock) module.
 *
 * @param[in]   None
 *
 * @attention   This function initializes the RTC module for system timekeeping.
 *
 * @return      The status of the RTC start operation.
 */
sys_rtc_status_t sys_rtc_start();

/**
 * @brief       Get the current time from the RTC.
 *
 * @param[out]  get_time    Pointer to a structure to store the retrieved time.
 *
 * @return      The status of the time retrieval operation.
 */
sys_rtc_status_t sys_rtc_get_time(sys_rtc_time_t *get_time);


/**
 * @brief       Set the RTC to a specified time.
 *
 * @param[in]   set_time    Pointer to a structure containing the time to set.
 *
 * @return      The status of the time setting operation.
 */
sys_rtc_status_t sys_rtc_set_time(sys_rtc_time_t *set_time);

/**
 * @brief       Set the RTC to a specified epoch time.
 *
 * @param[in]   epoch_time  The epoch time value to set.
 *
 * @return      The status of the epoch time setting operation.
 */
sys_rtc_status_t sys_rtc_set_epoch_time(uint32_t epoch_time);

/**
 * @brief       Set the mode of the RTC.
 *
 * @param[in]   mode_time   The mode to set for the RTC.
 *
 * @return      The status of the mode setting operation.
 */
sys_rtc_status_t sys_rtc_set_mode(sys_rtc_mode_time_t mode_time);


/**
 * @brief       Set an alarm on the RTC.
 *
 * @param[in]   alarm_time  Pointer to a structure containing the alarm time settings.
 *
 * @return      The status of the alarm setting operation.
 */
sys_rtc_status_t sys_rtc_set_alarm(sys_rtc_alarm_time_t *alarm_time);

/**
 * @brief       Check if an alarm is active.
 *
 * @param[out]  flag_alarm  Pointer to a boolean variable to indicate the alarm status.
 *
 * @return      The status of the alarm check operation.
 */
sys_rtc_status_t sys_rtc_alarm(bool *flag_alarm);

/**
 * @brief       Cancel an active alarm on the RTC.
 *
 * @param[in]   None
 *
 * @return      The status of the alarm cancellation operation.
 */
sys_rtc_status_t sys_rtc_cancel_alarm();


/**
 * @brief       Get the temperature from CJMCU75 temperature sensor.
 *
 * @param[in]   None
 *
 * @return      The temperature read from the sensor.
 */
int8_t sys_rtc_get_cjmcu75();
#endif /* INC_SYSTEM_SYS_RTC_H_ */
/* End of file -------------------------------------------------------- */
