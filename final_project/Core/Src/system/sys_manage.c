/**
 * @file       sys_manage.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       01/04/2024
 * @author     Kha Nguyen
 *
 * @brief      system manage to handle flow activity project
 */

/* Includes ----------------------------------------------------------- */
#include "sys_manage.h"
#include "string.h"
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

#define __RETURN(_EXPR_, _RET_VAL_) \
	do                              \
	{                               \
		if (!(_EXPR_))              \
			return _RET_VAL_;       \
	} while (0)
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static const uint8_t day_of_week[8][10] = {" ", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
/*variable using for sys_uart*/
static sys_uart_cmd_type_t smanage_cmd_type;
static sys_uart_cmd_data_t smanage_data;
/*variable using for sys_rtc*/

/*variable using for sys_display*/
static sys_display_time_t smanage_get_time;
static bool smanage_flag_alarm;
/* Private function prototypes ---------------------------------------- */
static void sys_manage_start();
static void sys_manage_uart(void);
static void sys_manage_display(void);
static void sys_manage_rtc(void);
/* Function definitions ----------------------------------------------- */
void sys_manage_loop(void)
{
	sys_manage_start();

	while (1)
	{
		sys_manage_uart();
		sys_manage_rtc();
		sys_manage_display();
	}
}
/* Private function definitions --------------------------------------- */
static void sys_manage_start()
{
	HAL_Delay(1000); // Wait for the system voltage to stabilize

	sys_manage_status_t error_handle = SYS_MANAGE_OK;

	error_handle = (sys_manage_status_t)sys_uart_start(); // start uart2
	__RETURN((error_handle == SYS_MANAGE_OK), sys_uart_print("\r\nSTART SYSTEM UART FAILD\r\n"));

	error_handle = (sys_manage_status_t)sys_rtc_start(); // start ds1307, cjmcu75, buzzer
	__RETURN((error_handle == SYS_MANAGE_OK), sys_uart_print("\r\nSTART SYSTEM RTC FAILD\r\n"));

	error_handle = (sys_manage_status_t)sys_display_start(); // start led ws2812, hall sensor
	__RETURN((error_handle == SYS_MANAGE_OK), sys_uart_print("\r\nSTART SYSTEM DISPLAY FAILD\r\n"));

	sys_uart_print("\r\nStart system successful\r\n\r\n");
}
static void sys_manage_display(void)
{
	sys_display_fan(smanage_get_time, smanage_flag_alarm); // take smanage_get_time and smanage_flag_alarm to update it to display fan
}
static void sys_manage_uart(void)
{
	sys_uart_status_t error = SYS_UART_OK;
	error = sys_uart_process(&smanage_cmd_type, &smanage_data);
	if (error == SYS_UART_OK)
	{
		switch (smanage_cmd_type)
		{
		case SYS_UART_CMD_CHECK_UART:
		{
			if (smanage_data.uart_check == true)
				sys_uart_print("\r\nOK\r\n\r\n");
			else
				sys_uart_print("\r\nONLY USE UART2 FOR COMMUNICATE WITH MICROCONTROLLER!!!\r\n\r\n");
			break;
		}
		case SYS_UART_CMD_SET_TIME:
		{
			sys_rtc_status_t error_handle = SYS_RTC_OK;
			error_handle = sys_rtc_set_time((sys_rtc_time_t *)(&smanage_data.set_time));
			__RETURN((error_handle == SYS_RTC_OK), sys_uart_print("\r\nSET TIME FAILED!!!\r\n\r\n"));
			sys_rtc_time_t get_time;

			sys_rtc_get_time(&get_time);

			uint8_t str_dow[10];
			for (uint8_t i = 1; i < 9; i++)
			{
				if (get_time.dow == i)
				{
					memcpy(str_dow, &day_of_week[i], strlen((char *)&day_of_week[i]));
					break;
				}
			}
			sys_uart_print("\r\nSet time now  %s , %d : %d : %d  -  %d : %d : %d\r\n\r\n", str_dow, get_time.hours, get_time.minutes, get_time.seconds, get_time.date, get_time.month, get_time.year);
			break;
		}
		case SYS_UART_CMD_SET_EPOCH_TIME:
		{
			sys_rtc_status_t error_handle = SYS_RTC_OK;
			error_handle = sys_rtc_set_epoch_time((uint32_t)smanage_data.epoch_time);
			__RETURN((error_handle == SYS_RTC_OK), sys_uart_print("\r\nSET EPOCH TIME FAILED!!!\r\n\r\n"));

			sys_uart_print("\r\nSet epoch time %d\r\n\r\n", smanage_data.epoch_time);

			sys_rtc_time_t get_time;

			sys_rtc_get_time(&get_time);

			uint8_t str_dow[10];
			for (uint8_t i = 1; i < 9; i++)
			{
				if (get_time.dow == i)
				{
					memcpy(str_dow, &day_of_week[i], strlen((char *)&day_of_week[i]));
					break;
				}
			}
			sys_uart_print("\r\nSet epoch time now  %s , %d : %d : %d  -  %d : %d : %d\r\n\r\n", str_dow, get_time.hours, get_time.minutes, get_time.seconds, get_time.date, get_time.month, get_time.year);
			break;
		}
		case SYS_UART_CMD_GET_TIME:
		{
			if (smanage_data.get_time == true)
			{
				sys_rtc_time_t get_time;

				sys_rtc_status_t error_handle = SYS_RTC_OK;
				error_handle = sys_rtc_get_time(&get_time);

				__RETURN((error_handle == SYS_RTC_OK), sys_uart_print("\r\nSET GET TIME FAILED!!!\r\n\r\n"));
				uint8_t str_dow[10];
				for (uint8_t i = 1; i < 9; i++)
				{
					if (get_time.dow == i)
					{
						memcpy(str_dow, &day_of_week[i], strlen((char *)&day_of_week[i]));
						break;
					}
				}
				sys_uart_print("\r\nGet time now  %s , %d : %d : %d  -  %d : %d : %d\r\n\r\n", str_dow, get_time.hours, get_time.minutes, get_time.seconds, get_time.date, get_time.month, get_time.year);
				break;
			}
			else
			{
				sys_uart_print("\r\nSET GET TIME FAILED!!!\r\n\r\n");
				break;
			}
		}
		case SYS_UART_CMD_SET_MODE_TIME:
		{
			sys_rtc_time_t get_time;

			sys_rtc_status_t error_handle = SYS_RTC_OK;
			error_handle = sys_rtc_set_mode((sys_rtc_mode_time_t)smanage_data.mode_time);
			__RETURN((error_handle == SYS_RTC_OK), sys_uart_print("\r\nSET MODE TIME FAILED!!!\r\n\r\n"));

			error_handle = sys_rtc_get_time(&get_time);
			__RETURN((error_handle == SYS_RTC_OK), sys_uart_print("\r\nSET MODE SUCCESSFUL!!!\r\n\r\n"));

			uint8_t str_dow[10];
			for (uint8_t i = 1; i < 9; i++)
			{
				if (get_time.dow == i)
				{
					memcpy(str_dow, &day_of_week[i], strlen((char *)&day_of_week[i]));
					break;
				}
			}
			sys_uart_print("\r\nSet mode time successful\r\nTime now %s, %d : %d : %d  -  %d : %d : %d\r\n\r\n", str_dow, get_time.hours, get_time.minutes, get_time.seconds, get_time.date, get_time.month, get_time.year);
			break;
		}
		case SYS_UART_CMD_SET_ALARM:
		{
			sys_rtc_status_t error_handle = SYS_RTC_OK;
			error_handle = sys_rtc_set_alarm((sys_rtc_alarm_time_t *)&smanage_data.set_alarm);
			__RETURN((error_handle == SYS_RTC_OK), sys_uart_print("\r\nSET ALARM FAILED!!!\r\n\r\n"));

			sys_uart_print("\r\nSet alarm successful\r\n\r\n");
			break;
		}
		case SYS_UART_CMD_CANCEL_ALARM:
		{
			if (smanage_data.cancel_alarm == true)
			{
				sys_rtc_cancel_alarm();
				sys_uart_print("\r\nCancle alarm successful\r\n\r\n");
				break;
			}
			else
			{
				sys_uart_print("\r\nCANCLE ALARM FAILED!!!\r\n\r\n");
				break;
			}
		}
		case SYS_UART_CMD_SET_BRIGHTNESS:
		{
			sys_display_status_t error_handle = SYS_DISPLAY_OK;

			error_handle = sys_display_set_brightness((uint8_t)smanage_data.brightness);
			__RETURN((error_handle == SYS_DISPLAY_OK), sys_uart_print("\r\nSET BRIGHTNESS FAILED!!!\r\n\r\n"));

			sys_uart_print("\r\nSet brightness successful\r\n\r\n");
			break;
		}
		case SYS_UART_CMD_SET_COLOR:
		{
			sys_display_status_t error_handle = SYS_DISPLAY_OK;

			error_handle = sys_display_set_color((sys_display_color_default_t)smanage_data.color);
			__RETURN((error_handle == SYS_DISPLAY_OK), sys_uart_print("\r\nSET COLOR FAILED!!!\r\n\r\n"));

			sys_uart_print("\r\nSet color successful\r\n");
			break;
		}
		case SYS_UART_CMD_GET_TEMPERATURE:
		{
			if (smanage_data.temp == true)
			{
				int8_t temp = sys_rtc_get_cjmcu75();
				sys_uart_print("\r\n Temperature now : %d C temperature\r\n", temp);
				break;
			}
			else
			{
				sys_uart_print("\r\nGET TEMPERATURE DATA FAILD!!!\r\n\r\n");
				break;
			}
		}
		default:
			break;
		}
	}
}

static void sys_manage_rtc(void)
{
	/*get time to pass sys_display_fan*/
	sys_rtc_get_time((sys_rtc_time_t *)&smanage_get_time); // get data time

	/*Check alarm*/
	sys_rtc_alarm(&smanage_flag_alarm); // get status flag alarm
}

/* End of file -------------------------------------------------------- */
