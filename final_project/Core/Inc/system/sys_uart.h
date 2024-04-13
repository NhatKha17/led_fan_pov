/**
 * @file       sys_data.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       20/03/2024
 * @author     Kha Nguyen
 *
 * @brief      system data process data from user by uart
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_SYSTEM_SYS_UART_H_
#define INC_SYSTEM_SYS_UART_H_

/* Includes ----------------------------------------------------------- */
#include "drv_uart.h"
/* Public defines ----------------------------------------------------- */

/**
 * @brief enumeration define status system data
 */
typedef enum
{
	SYS_UART_OK = 0U,
	SYS_UART_ERROR,
} sys_uart_status_t;

/**
 * @brief enumeration define packet type
 */
typedef enum
{
	SYS_UART_CMD_CHECK_UART = 0U,
	SYS_UART_CMD_SET_TIME,
	SYS_UART_CMD_SET_EPOCH_TIME,
	SYS_UART_CMD_GET_TIME,
	SYS_UART_CMD_SET_MODE_TIME,
	SYS_UART_CMD_SET_ALARM,
	SYS_UART_CMD_CANCEL_ALARM,
	SYS_UART_CMD_SET_BRIGHTNESS,
	SYS_UART_CMD_SET_COLOR,
	SYS_UART_CMD_GET_TEMPERATURE,
} sys_uart_cmd_type_t;

/**
 * @brief 	enumeration define set mode time
 *
 * @example	data sample	: char[6] data = "uart2";
 * 			 process 	: bool uart_check = true;
 */
typedef bool sys_uart_cmd_check_t;

/**
 * @brief 	enumeration define set mode time
 *
 * @note	mode 12h : DS1307_MODE_12H
 * @note	mode 24h : DS1307_MODE_24H
 *
 * @example	data simple	: char data[] = "12"
 * 			data process: ds1307_mode_t mode_t = 12;
 */
typedef enum
{
	SYS_UART_MODE_24H = 0U, /* mode 24h  */
	SYS_UART_MODE_12H = 1U, /* mode 12h  */
} sys_uart_cmd_mode_time_t;

/**
 * @brief 	enumeration define set mode time
 *
 * @example	data sample	: char data[24] = "10h10p10s03d01m2024yfri"
 * 			process 	: 	uint8_t hours = 10;
 * 			 				uint8_t minutes = 10;
 * 			 				uint8_t seconds = 10;
 * 			 				uint8_t	dow =  6;
 * 			 				uint8_t date = 03;
 * 			 				uint8_t month = 01;
 * 			 				uint32_t year = 2024;
 * 			 				ds1307_mode_t mode = 24h;
 */
typedef struct
{
	uint8_t hours;	 /* Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t minutes; /* Minutes parameter, from 00 to 59 */
	uint8_t seconds; /* Seconds parameter, from 00 to 59 */
	uint8_t dow;	 /* Day in a week, from monday to sunday */
	uint8_t date;	 /* Date in a month, 1 to 31 */
	uint8_t month;	 /* Month in a year, 1 to 12 */
	uint32_t year;	 /* Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
} sys_uart_cmd_set_time_t;

/**
 * @brief 	enumeration define data set epoch time
 *
 * @example	data sample	: char data[11] = "1709303879";
 * 			process 	: uint32_t epoch_time = 1709303879;
 */
typedef uint32_t sys_uart_cmd_set_epoch_time_t;

/**
 * @brief 	enumeration define process data get time
 *
 * @example	data sample	: char data[8] = "gettime";
 * 			process 	: char get_time[8] = "gettime";
 */
typedef bool sys_uart_cmd_get_time_t;

/**
 * @brief 	enumeration define set alarm & modify alarm
 *
 * @example	data sample	: char data[10] = "10h30p30s"
 * 			process 	: 	uint8_t hours = 10;
 * 			 				uint8_t minutes = 10;
 * 			 				uint8_t seconds = 10;
 */
typedef struct
{
	uint8_t hours;	 /* Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t minutes; /* Minutes parameter, from 00 to 59 */
	uint8_t seconds; /* Seconds parameter, from 00 to 59 */
} sys_uart_cmd_alarm_t;

/**
 * @brief 	enumeration define cancle alarm
 *
 * @example	data simple	: char data[7] = "cancle"
 * 			data process: char cancle[7] = "cancle";
 */
typedef bool sys_uart_cmd_cancel_alarm_t;

/**
 * @brief 	enumeration define set brightness
 *
 * @note	brigthness's value from 0--100
 *
 * @example	data simple	: char data[3] = "50"
 * 			data process: sys_uart_cmd_brigthness_t brightness = 50;
 */
typedef uint8_t sys_uart_cmd_brightness_t;

/**
 * @brief 	enumeration define set color
 *
 * @note	color1	:	color1
 * @note	color2	:	color2
 *
 * @example	data simple	: char data[7] = "color1"
 * 			data process: sys_uart_cmd_color_t color = SYS_UART_COLOR_1;
 */
typedef enum
{
	SYS_UART_COLOR_1 = 1U,
	SYS_UART_COLOR_2,
} sys_uart_cmd_color_t;

/**
 * @brief 	enumeration define process data get time
 *
 * @example	data sample	: char data[8] = "gettemp";
 * 			process 	: char get_time[8] = "gettemp";
 */
typedef bool sys_uart_cmd_get_temp_t;

/**
 * @brief   Union for storing different types of data related to UART commands.
 *
 * @details This union allows the storage of different types of data structures
 *          representing various UART commands. It is used to encapsulate data
 *          associated with UART commands such as checking status, setting time,
 *          setting epoch time, getting time, setting mode, setting alarms,
 *          canceling alarms, adjusting brightness, setting color, and getting temperature.
 *
 * @note    This union is designed to be flexible and accommodate different types
 *          of data structures corresponding to different UART commands.
 *
 * @see     sys_uart_cmd_check_t, sys_uart_cmd_set_time_t, sys_uart_cmd_set_epoch_time_t,
 *          sys_uart_cmd_get_time_t, sys_uart_cmd_mode_time_t, sys_uart_cmd_alarm_t,
 *          sys_uart_cmd_cancel_alarm_t, sys_uart_cmd_brightness_t, sys_uart_cmd_color_t,
 *          sys_uart_cmd_get_temp_t
 */
typedef union
{
	sys_uart_cmd_check_t uart_check;
	sys_uart_cmd_set_time_t set_time;
	sys_uart_cmd_set_epoch_time_t epoch_time;
	sys_uart_cmd_get_time_t get_time;
	sys_uart_cmd_mode_time_t mode_time;
	sys_uart_cmd_alarm_t set_alarm;
	sys_uart_cmd_cancel_alarm_t cancel_alarm;
	sys_uart_cmd_brightness_t brightness;
	sys_uart_cmd_color_t color;
	sys_uart_cmd_get_temp_t temp;
} sys_uart_cmd_data_t;

/* Public enumerate/structure ----------------------------------------- */

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Start the system UART (Universal Asynchronous Receiver-Transmitter) module.
 *
 * @param[in]   None
 *
 * @attention   This function initializes the UART module for communication.
 *
 * @return      The status of the UART start operation.
 */
sys_uart_status_t sys_uart_start();

/**
 * @brief       Process UART commands.
 *
 * @param[out]  type    Pointer to store the type of UART command received.
 * @param[out]  data    Pointer to store the data associated with the UART command.
 *
 * @return      The status of the UART command processing operation.
 */
sys_uart_status_t sys_uart_process(sys_uart_cmd_type_t *type, sys_uart_cmd_data_t *data);

/**
 * @brief       Print formatted data to the UART.
 *
 * @param[in]   fmt     Format string followed by additional arguments.
 *                      Follows the same syntax as printf function.
 *
 * @return      None
 */
void sys_uart_print(const char *const fmt, ...);
#endif /* INC_SYSTEM_SYS_UART_H_ */
/* End of file -------------------------------------------------------- */
