/**
 * @file       sys_data.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       20/03/2024
 * @author     Kha Nguyen
 *
 * @brief      system data process data from user by uart
 */

/* Includes ----------------------------------------------------------- */
#include "sys_uart.h"
#include "stdarg.h"
/* Private defines ---------------------------------------------------- */
#define CB_BUF_SIZE (512)
#define RX_BUF_SIZE (256)

#define SYS_UART_LENGTH_CHECK_UART (5)
#define SYS_UART_LENGTH_SET_TIME (23)
#define SYS_UART_LENGTH_SET_EPOCH_TIME (10)
#define SYS_UART_LENGTH_GET_TIME (7)
#define SYS_UART_LENGTH_SET_MODE_TIME (2)
#define SYS_UART_LENGTH_SET_ALARM (9)
#define SYS_UART_LENGTH_CANCEL_ALARM (6)
#define SYS_UART_LENGTH_SET_BRIGHTNESS (3)
#define SYS_UART_LENGTH_SET_COLOR (1)
#define SYS_UART_LENGTH_GET_TEMPERATURE (7)

#define SYS_UART_MINIUM_DATA (8)
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
extern UART_HandleTypeDef huart2;
/* Private variables -------------------------------------------------- */
static cbuffer_t sys_uart_cb;
static uint8_t sys_uart_cb_buf[CB_BUF_SIZE];												   // using for circular buffer
static uint8_t sys_uart_rx_buf[RX_BUF_SIZE];												   // using save data that be received
static uint8_t sys_uart_day_of_week[7][4] = {"mon", "tue", "wed", "thu", "fri", "sat", "sun"}; // using data parse
/* Private function prototypes ---------------------------------------- */
static sys_uart_status_t sys_uart_init();
static sys_uart_status_t sys_uart_receive();
static uint32_t count_data_cb_buf();
static void clear_data_cb_buf();
static sys_uart_status_t data_parse(sys_uart_cmd_type_t *cmd_type, uint8_t *data);

static void sys_uart_handle_data(huart_serial_t *huart, uint16_t size);
/* Function definitions ----------------------------------------------- */
sys_uart_status_t sys_uart_start()
{
	sys_uart_status_t error_status = SYS_UART_OK;
	error_status = sys_uart_init();
	__ASSERT((error_status == SYS_UART_OK), SYS_UART_ERROR);

	error_status = sys_uart_receive();
	__ASSERT((error_status == SYS_UART_OK), SYS_UART_ERROR);

	return SYS_UART_OK;
}

sys_uart_status_t sys_uart_process(sys_uart_cmd_type_t *type, sys_uart_cmd_data_t *data)
{
	__ASSERT((type != NULL), SYS_UART_ERROR);
	__ASSERT((data != NULL), SYS_UART_ERROR);

	uint8_t buf[30] = {0};
	sys_uart_cmd_type_t cmd_type = SYS_UART_CMD_CHECK_UART;

	if (count_data_cb_buf() > 0)
	{
		sys_uart_status_t error_status = data_parse(&cmd_type, buf);
		__ASSERT((error_status == SYS_UART_OK), SYS_UART_ERROR);
	}
	else
	{
		return SYS_UART_ERROR;
	}

	uint8_t str_len = strlen((char *)buf); // strlen function discard '\0' character

	switch (cmd_type)
	{
	case SYS_UART_CMD_CHECK_UART:
	{
		uint8_t default_data[] = "uart2";
		if ((str_len != SYS_UART_LENGTH_CHECK_UART) || (strcmp((char *)buf, (char *)default_data) != 0))
		{
			sys_uart_print("\r\nDATA CHECK UART WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_CHECK_UART;
			data->uart_check = true;
			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_SET_TIME:
	{
		if (str_len != SYS_UART_LENGTH_SET_TIME)
		{
			sys_uart_print("\r\nDATA SET TIME WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_SET_TIME;

			uint8_t str_time[7][5];

			/*coppy data hours*/
			str_time[0][0] = buf[0];
			str_time[0][1] = buf[1];
			str_time[0][2] = '\0';
			uint8_t hours = atoi((char *)str_time[0]);
			data->set_time.hours = hours;

			/*coppy data minutes*/
			str_time[1][0] = buf[3];
			str_time[1][1] = buf[4];
			str_time[1][2] = '\0';
			uint8_t minutes = atoi((char *)str_time[1]);
			data->set_time.minutes = minutes;

			/*coppy data seconds*/
			str_time[2][0] = buf[6];
			str_time[2][1] = buf[7];
			str_time[2][2] = '\0';
			uint8_t seconds = atoi((char *)str_time[2]);
			data->set_time.seconds = seconds;

			/*coppy data day*/
			str_time[3][0] = buf[9];
			str_time[3][1] = buf[10];
			str_time[3][2] = '\0';
			uint8_t date = atoi((char *)str_time[3]);
			data->set_time.date = date;

			/*coppy data month*/
			str_time[4][0] = buf[12];
			str_time[4][1] = buf[13];
			str_time[4][2] = '\0';
			uint8_t month = atoi((char *)str_time[4]);
			data->set_time.month = month;

			/*coppy data year*/
			str_time[5][0] = buf[15];
			str_time[5][1] = buf[16];
			str_time[5][2] = buf[17];
			str_time[5][3] = buf[18];
			str_time[5][4] = '\0';
			uint32_t year = atoi((char *)str_time[5]);
			data->set_time.year = year;

			/*coppy data day of week*/
			str_time[6][0] = buf[20];
			str_time[6][1] = buf[21];
			str_time[6][2] = buf[22];
			str_time[6][3] = '\0';
			for (uint8_t i = 0; i < 7; ++i)
			{
				if (strcmp((char *)str_time[6], (char *)sys_uart_day_of_week[i]) == 0)
				{
					data->set_time.dow = i + 1;
					break;
				}
			}

			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_SET_EPOCH_TIME:
	{
		if (str_len != SYS_UART_LENGTH_SET_EPOCH_TIME)
		{
			sys_uart_print("\r\nDATA SET EPOCH TIME WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_SET_EPOCH_TIME;
			uint32_t epoch_time = atoi((char *)buf);
			data->epoch_time = epoch_time;

			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_GET_TIME:
	{
		uint8_t default_data[] = "gettime";
		if ((str_len != SYS_UART_LENGTH_GET_TIME) || (strcmp((char *)buf, (char *)default_data) != 0))
		{
			sys_uart_print("\r\nDATA GET TIME WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_GET_TIME;
			data->get_time = true;

			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_SET_MODE_TIME:
	{
		if (str_len != SYS_UART_LENGTH_SET_MODE_TIME)
		{
			sys_uart_print("\r\nDATA SET MODE TIME WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_SET_MODE_TIME;

			uint8_t mode_time = atoi((char *)buf);
			if (mode_time == 12)
			{
				data->mode_time = SYS_UART_MODE_12H;
				return SYS_UART_OK;
			}
			else if (mode_time == 24)
			{
				data->mode_time = SYS_UART_MODE_24H;
				return SYS_UART_OK;
			}
			else
				return SYS_UART_ERROR;
		}
	}
	case SYS_UART_CMD_SET_ALARM:
	{
		if (str_len != SYS_UART_LENGTH_SET_ALARM)
		{
			sys_uart_print("\r\nDATA SET ALARM WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_SET_ALARM;

			uint8_t str_time[3][3];

			/*coppy data hours*/
			str_time[0][0] = buf[0];
			str_time[0][1] = buf[1];
			uint8_t hours = atoi((char *)str_time[0]);
			data->set_alarm.hours = hours;

			/*coppy data minutes*/
			str_time[1][0] = buf[3];
			str_time[1][1] = buf[4];

			uint8_t minutes = atoi((char *)str_time[1]);
			data->set_alarm.minutes = minutes;

			/*coppy data seconds*/
			str_time[2][0] = buf[6];
			str_time[2][1] = buf[7];
			uint8_t seconds = atoi((char *)str_time[2]);
			data->set_alarm.seconds = seconds;

			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_CANCEL_ALARM:
	{
		uint8_t default_data[] = "cancel";
		if ((str_len != SYS_UART_LENGTH_CANCEL_ALARM) || (strcmp((char *)buf, (char *)default_data) != 0))
		{
			sys_uart_print("\r\nDATA CANCEL ALARM WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_CANCEL_ALARM;
			data->cancel_alarm = true;
			//			sys_uart_print("\r\nok ok ok !!!\r\n\r\n");
			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_SET_BRIGHTNESS:
	{
		if (str_len != SYS_UART_LENGTH_SET_BRIGHTNESS)
		{
			sys_uart_print("\r\nDATA SET BRIGHTNESS WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_SET_BRIGHTNESS;
			uint8_t brightness = atoi((char *)buf);
			data->brightness = brightness;

			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_SET_COLOR:
	{
		if (str_len != SYS_UART_LENGTH_SET_COLOR)
		{
			sys_uart_print("\r\nDATA SET COLOR WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_SET_COLOR;
			uint8_t color = atoi((char *)buf);
			data->color = color;

			return SYS_UART_OK;
		}
	}
	case SYS_UART_CMD_GET_TEMPERATURE:
	{
		uint8_t default_data[] = "gettemp";
		if ((str_len != SYS_UART_LENGTH_GET_TEMPERATURE) || (strcmp((char *)buf, (char *)default_data) != 0))
		{
			sys_uart_print("\r\nDATA GET TEMPERATURE WRONG WITH PROTOCOL!!!\r\n\r\n");
			return SYS_UART_ERROR;
		}
		else
		{
			*type = SYS_UART_CMD_GET_TEMPERATURE;
			data->temp = true;

			return SYS_UART_OK;
		}
	}
	}
	return SYS_UART_ERROR;
}

void sys_uart_print(const char *const fmt, ...)
{
	char str[256];
	//    uint16_t len;
	va_list args;

	memset((char *)str, 0, sizeof(char) * 256);
	va_start(args, fmt);
	vsnprintf((char *)str, 255, (char const *)fmt, args);
	va_end(args);

	//    len = strlen((char *)str);
	(void)drv_uart_print((uint8_t *)str);
}
/* Private function definitions --------------------------------------- */

static sys_uart_status_t data_parse(sys_uart_cmd_type_t *cmd_type, uint8_t *data)
{
	uint32_t data_count = count_data_cb_buf();
	if (data_count < SYS_UART_MINIUM_DATA)
	{
		clear_data_cb_buf();
		return SYS_UART_ERROR;
	}
	uint8_t start_bit[] = "A";
	uint8_t stop_bit[] = "B";

	uint8_t buf_check_start_bit[2];
	if (cb_read(&sys_uart_cb, buf_check_start_bit, 1) != 1 || (buf_check_start_bit[0] != start_bit[0]))
	{
		return SYS_UART_ERROR;
	}
COMEBACK:
	if (cb_read(&sys_uart_cb, &buf_check_start_bit[1], 1) != 1 || (buf_check_start_bit[1] != start_bit[0]))
	{
		return SYS_UART_ERROR;
	}

	uint8_t buf_check_cmd[2];
	if (cb_read(&sys_uart_cb, buf_check_cmd, 2) != 2)
	{
		return SYS_UART_ERROR;
	}
	uint8_t cmd = (buf_check_cmd[0] - '0') * 10 + (buf_check_cmd[1] - '0');

	uint8_t buf_check_total_length[2];
	if (cb_read(&sys_uart_cb, buf_check_total_length, 2) != 2)
	{
		return SYS_UART_ERROR;
	}
	uint32_t total_length = (buf_check_total_length[0] - '0') * 10 + (buf_check_total_length[1] - '0');

	uint8_t buf_data[total_length];
	if (cb_read(&sys_uart_cb, buf_data, total_length) != total_length)
	{
		return SYS_UART_ERROR;
	}

	uint8_t buf_check_stop_bit[3];
	if (cb_read(&sys_uart_cb, buf_check_stop_bit, 1) != 1 || buf_check_stop_bit[0] != stop_bit[0])
	{
		if (buf_check_stop_bit[0] == start_bit[0])
		{
			buf_check_start_bit[0] = buf_check_stop_bit[0];
			goto COMEBACK;
		}
		return SYS_UART_ERROR;
	}

	if (cb_read(&sys_uart_cb, &buf_check_stop_bit[1], 1) != 1 || buf_check_stop_bit[1] != stop_bit[0])
	{
		if (buf_check_stop_bit[1] == start_bit[0])
		{
			buf_check_start_bit[0] = buf_check_stop_bit[1];
			goto COMEBACK;
		}
		return SYS_UART_ERROR;
	}

	*cmd_type = cmd;

	memcpy(data, buf_data, total_length);
	data[total_length] = '\0';

	return SYS_UART_OK;
}

static sys_uart_status_t sys_uart_init()
{
	sys_uart_status_t error_status = SYS_UART_OK;
	drv_uart_t suart;
	suart.drv_uart_huart = &huart2;
	suart.drv_uart_read = bsp_uart_read_to_idle_dma;
	suart.drv_uart_print = bsp_uart_print_length;

	error_status = (sys_uart_status_t)drv_uart_init(suart, &sys_uart_cb, sys_uart_cb_buf, CB_BUF_SIZE);
	__ASSERT((error_status == SYS_UART_OK), SYS_UART_ERROR);
	return SYS_UART_OK;
}

static sys_uart_status_t sys_uart_receive()
{
	sys_uart_status_t error_status = SYS_UART_OK;
	error_status = (sys_uart_status_t)drv_uart_receive(sys_uart_rx_buf, RX_BUF_SIZE);
	__ASSERT((error_status == SYS_UART_OK), SYS_UART_ERROR);
	return SYS_UART_OK;
}

/*
 * @fn      count_data_cb_buf
 *
 * @brief   This function returns the count of available UART data.
 *
 * @return  uint32_t data count
 */
static uint32_t count_data_cb_buf()
{
	uint32_t data_count = cb_data_count(&sys_uart_cb);
	return data_count;
}

/*
 * @fn      drv_uart_clear_data
 *
 * @brief   This function clears the UART data buffer.
 *
 */
static void clear_data_cb_buf()
{
	cb_clear(&sys_uart_cb);
}
/*
 * @fn      sys_uart_handle_data
 *
 * @brief   handle data in cbuffer
 *
 * @param	huart	A pointer of huart_serial_t
 * @param   size	The size of data
 *
 */
static void sys_uart_handle_data(huart_serial_t *huart, uint16_t size)
{
	if (huart->Instance == USART2)
	{
		uint16_t space_count = cb_space_count(&sys_uart_cb);
		if (space_count < size)
		{
			char *tx_data = "CB WRITE OVERFLOW\r\n";
			(void)bsp_uart_print_length(&huart2, (uint8_t *)tx_data, strlen((char *)tx_data));
		}
		else
		{
			cb_write(&sys_uart_cb, sys_uart_rx_buf, size);
		}
	}
}

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
	sys_uart_handle_data(huart, size);
	sys_uart_receive();
}
/* End of file -------------------------------------------------------- */
