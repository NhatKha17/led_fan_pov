/**
 * @file       ds1307.c
 * @copyright
 * @license
 * @version    1.0.1
 * @date       21/02/2024
 * @author     Kha Nguyen
 *
 * @brief      driver ds1307
 *
 *
 */
/* Includes ----------------------------------------------------------- */
#include "ds1307.h"
/* Private defines ---------------------------------------------------- */
/**
 * @brief device addr
 */
#define DS1307_ADDRESS_8BIT (0xD0) /*address 8bit 0x68*/
#define DS1307_ADDRESS_7BIT (0x68) /*address 7bit 0x68*/

/**
 * @brief register addr
 */
#define DS1307_REG_SECONDS (0x00)
#define DS1307_REG_MINUTES (0x01)
#define DS1307_REG_HOURS (0x02)
#define DS1307_REG_DOW (0x03)
#define DS1307_REG_DATE (0x04)
#define DS1307_REG_MONTH (0x05)
#define DS1307_REG_YEAR (0x06)
#define DS1307_REG_CONTROL (0x07)
#define DS1307_REG_RAM (0x08)

/**
 * @brief register mask addr
 */
#define DS1307_MASK_SECOND (0b01111111)
#define DS1307_MASK_MINUTES (0b01111111)
#define DS1307_MASK_HOUR_12 (0b00011111)
#define DS1307_MASK_HOUR_24 (0b00111111)
#define DS1307_MASK_DOW (0b00000111)
#define DS1307_MASK_DATE (0b00111111)
#define DS1307_MASK_MONTH (0b00011111)
#define DS1307_MASK_YEAR (0b11111111)

/**
 * mode 12h or 24h - register DS1307_REGISTER_HOUR (0x02)
 */
#define DS1307_MODE (0b01000000)

/**
 * 12h mode am or pm - register DS1307_REGISTER_HOUR (0x02)
 */
#define DS1307_12_AM_PM (0b00100000)

/**
 * Clock Halt (CH) - register DS1307_REGISTER_SECOND (0x00).
 * When this bit is set to 1 RTC was halt - default state.
 */
#define DS1307_FLAG_CH (0b10000000)

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
static ds1307_t ds1307;
/* Private function prototypes ---------------------------------------- */
static uint8_t bcd_to_hex(uint8_t bin);
static uint8_t hex_to_bcd(uint8_t hex);
static ds1307_status_t check_time_set(ds1307_time_t *set_time);
static uint8_t day_of_month(uint8_t month, uint16_t year);
static uint8_t check_leap_year(uint16_t year);
static void convert_epoch_time(ds1307_time_t *date_time, uint32_t epoch_time);
static ds1307_status_t set_mode_time(ds1307_mode_t mode);
static ds1307_status_t set_clock_haft(uint8_t bit_clock_haft);
// static uint32_t ds1307_convert_to_epoch_time(ds1307_time_t date_time);
/* Function definitions ----------------------------------------------- */

ds1307_status_t ds1307_init(ds1307_t sys_ds1307)
{
	__ASSERT((sys_ds1307.ds1307_i2c != NULL), DS1307_NULL_PTR);

	ds1307 = sys_ds1307;
	ds1307.device_addr = DS1307_ADDRESS_8BIT;

#ifdef DEBUG
	HAL_StatusTypeDef error_handle = HAL_OK;
	error_handle = ds1307.i2c_device_check(ds1307.ds1307_i2c, ds1307.device_addr);
	__ASSERT((error_handle == HAL_OK), DS1307_ERROR);
#endif
	return DS1307_OK;
}

ds1307_status_t ds1307_get_time(ds1307_time_t *get_time)
{
//	__ASSERT((get_time != NULL), DS1307_NULL_PTR);

	uint8_t buf[7] = {0}; /* buffer save data get_time*/

	ds1307_status_t error_status = DS1307_OK;
	error_status = (ds1307_status_t)ds1307.i2c_read_multi(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_SECONDS, buf, 7);
	__RETURN((error_status == DS1307_OK), error_status);

	get_time->year = bcd_to_hex(buf[6] & DS1307_MASK_YEAR) + 2000;
	get_time->month = bcd_to_hex(buf[5] & DS1307_MASK_MONTH);
	get_time->date = bcd_to_hex(buf[4] & DS1307_MASK_DATE);
	get_time->dow = bcd_to_hex(buf[3] & DS1307_MASK_DOW);
	get_time->am_pm = (ds1307_am_pm_t)((buf[2] & DS1307_12_AM_PM) >> 5);
	get_time->mode = (ds1307_mode_t)((buf[2] & DS1307_MODE) >> 6);
	get_time->hours = bcd_to_hex(buf[2] & (get_time->mode == DS1307_MODE_24H ? DS1307_MASK_HOUR_24 : DS1307_MASK_HOUR_12));
	get_time->minutes = bcd_to_hex(buf[1] & DS1307_MASK_MINUTES);
	get_time->seconds = bcd_to_hex(buf[0] & DS1307_MASK_SECOND);

	return DS1307_OK;
}

ds1307_status_t ds1307_set_time(ds1307_time_t *set_time)
{
	__ASSERT((set_time != NULL), DS1307_NULL_PTR);
	__RETURN((check_time_set(set_time) == DS1307_OK), DS1307_INVALID_PARA);

	uint8_t reg;
	ds1307_status_t error_status = DS1307_OK;

	error_status = (ds1307_status_t)ds1307.i2c_read(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_SECONDS, &reg);
	__RETURN((error_status == DS1307_OK), error_status);

	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_SECONDS, hex_to_bcd(set_time->seconds) | (reg & DS1307_FLAG_CH));
	__RETURN((error_status == DS1307_OK), error_status);

	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_MINUTES, hex_to_bcd(set_time->minutes));
	__RETURN((error_status == DS1307_OK), error_status);

	if (set_time->mode == DS1307_MODE_12H)
	{
		reg = (uint8_t)(set_time->mode << 6) | (set_time->am_pm << 5) | hex_to_bcd(set_time->hours);
	}
	else
	{
		reg = (set_time->mode << 6) | hex_to_bcd(set_time->hours);
	}
	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_HOURS, reg);
	__RETURN((error_status == DS1307_OK), error_status);

	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_DOW, hex_to_bcd(set_time->dow));
	__RETURN((error_status == DS1307_OK), error_status);

	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_DATE, hex_to_bcd(set_time->date));
	__RETURN((error_status == DS1307_OK), error_status);

	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_MONTH, hex_to_bcd(set_time->month));
	__RETURN((error_status == DS1307_OK), error_status);

	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_YEAR, hex_to_bcd((uint8_t)(set_time->year - 2000)));
	__RETURN((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}

ds1307_status_t ds1307_set_by_epoch_time(uint32_t epoch_time)
{
	ds1307_status_t error_status = DS1307_OK;

	ds1307_time_t __set_time;

	convert_epoch_time(&__set_time, epoch_time);

	error_status = ds1307_set_time(&__set_time);
	__RETURN((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}

ds1307_status_t ds1307_set_mode_12h()
{
	ds1307_status_t error_status = DS1307_OK;
	error_status = set_mode_time(DS1307_MODE_12H);
	__RETURN((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}

ds1307_status_t ds1307_set_mode_24h()
{
	ds1307_status_t error_status = DS1307_OK;
	error_status = set_mode_time(DS1307_MODE_24H);
	__RETURN((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}

ds1307_status_t ds1307_set_clock_haft_enable()
{
	ds1307_status_t error_status = DS1307_OK;
	error_status = set_clock_haft(DS1307_CH_ENABLE);
	__ASSERT((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}

ds1307_status_t ds1307_set_clock_haft_disable()
{
	ds1307_status_t error_status = DS1307_OK;
	error_status = set_clock_haft(DS1307_CH_DISABLE);
	__RETURN((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}
/* Private function definitions ----------------------------------------------- */

static uint8_t bcd_to_hex(uint8_t bin)
{
	uint8_t hex = ((bin & 0xf0) >> 4) * 10;
	hex += (bin & 0x0f);
	return hex;
}
static uint8_t hex_to_bcd(uint8_t hex)
{
	uint8_t high = hex / 10;
	uint8_t low = hex % 10;
	return (high << 4) | low;
}
static ds1307_status_t check_time_set(ds1307_time_t *set_time)
{
	if ((set_time->year < 2000) || (set_time->year > 2100))
		return DS1307_INVALID_PARA;
	if ((set_time->month < 0) || (set_time->month > 12))
		return DS1307_INVALID_PARA;
	if ((set_time->date < 0) || (set_time->date > day_of_month(set_time->month, set_time->year)))
		return DS1307_INVALID_PARA;
	if ((set_time->dow) < 1 || (set_time->dow > 7))
		return DS1307_INVALID_PARA;
	if (set_time->mode == DS1307_MODE_12H)
	{
		if ((set_time->am_pm != DS1307_AM && set_time->am_pm != DS1307_PM))
			return DS1307_INVALID_PARA;
		if ((set_time->hours < 0) || (set_time->hours > 12))
			return DS1307_INVALID_PARA;
	}
	else if (set_time->mode == DS1307_MODE_24H)
	{
		if ((set_time->hours < 0) || (set_time->hours > 24))
			return DS1307_INVALID_PARA;
	}
	else
		return DS1307_INVALID_PARA;
	if ((set_time->minutes < 0) || (set_time->minutes > 59))
		return DS1307_INVALID_PARA;
	if ((set_time->seconds < 0) || (set_time->seconds > 59))
		return DS1307_INVALID_PARA;
	return DS1307_OK;
}
static uint8_t day_of_month(uint8_t month, uint16_t year)
{
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		return 31;

	case 4:
	case 6:
	case 9:
	case 11:
		return 30;

	case 2:
		if (check_leap_year(year))
			return 29;
		else
			return 28;
	default:
		break;
	}
	return 0;
}
static uint8_t check_leap_year(uint16_t year)
{
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}
static void convert_epoch_time(ds1307_time_t *date_time, uint32_t epoch_time)
{
	struct tm *_time;
	time_t epoch_time_c = epoch_time;
	_time = localtime(&epoch_time_c);
	_time->tm_year += 1900;
	//	  _time.tm_wday == 0 ? 7 : time.tm_wday;

	date_time->year = _time->tm_year;
	date_time->month = _time->tm_mon + 1;
	date_time->date = _time->tm_mday;
	date_time->hours = _time->tm_hour;
	date_time->minutes = _time->tm_min;
	date_time->seconds = _time->tm_sec;
	date_time->dow = (_time->tm_wday == 0 ? 7 : _time->tm_wday);
	date_time->mode = DS1307_MODE_24H;
}
// static uint32_t ds1307_convert_to_epoch_time(ds1307_time_t date_time)
//{
//	struct tm _time_date;
//	memset((void *)&_time_date, 0, sizeof(_time_date));
//
//	_time_date.tm_wday = date_time.dow == 7 ? 0 : date_time.dow;
//	_time_date.tm_min = date_time.minutes;
//	_time_date.tm_sec = date_time.seconds;
//	_time_date.tm_hour = date_time.hours;
//	_time_date.tm_mday = date_time.date;
//	_time_date.tm_mon = date_time.month - 1;
//	_time_date.tm_year = date_time.year - 1900;
//	uint32_t epoch_time = mktime(&_time_date);
//
//	return epoch_time;
// }
static ds1307_status_t set_mode_time(ds1307_mode_t mode)
{
	uint8_t reg = 0;
	switch (mode)
	{
	case DS1307_MODE_12H:
	{
		ds1307_status_t error_status = DS1307_OK;
		error_status = (ds1307_status_t)ds1307.i2c_read(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_HOURS, &reg);
		__ASSERT((error_status == DS1307_OK), error_status);

		if (reg & DS1307_MODE)
			return DS1307_OK;

		uint8_t hour = bcd_to_hex(reg & DS1307_MASK_HOUR_24);
		reg = 0;
		if (hour > 12)
		{
			reg = DS1307_12_AM_PM;
			hour -= 12;
		}
		else
		{
			reg = 0;
		}
		reg |= DS1307_MODE | hex_to_bcd((uint8_t)hour);

		error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_HOURS, reg);
		__ASSERT((error_status == DS1307_OK), error_status);
		return DS1307_OK;
	}
	case DS1307_MODE_24H:
	{
		ds1307_status_t error_status = DS1307_OK;
		error_status = (ds1307_status_t)ds1307.i2c_read(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_HOURS, &reg);
		__ASSERT((error_status == DS1307_OK), error_status);

		if (!(reg & DS1307_MODE))
			return DS1307_OK;

		uint8_t hour = bcd_to_hex(reg & DS1307_MASK_HOUR_12);

		if (reg & DS1307_12_AM_PM)
			hour += 12;
		reg = hex_to_bcd(hour);
		error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_HOURS, reg);
		__ASSERT((error_status == DS1307_OK), error_status);

		return DS1307_OK;
	}
	default:
		return DS1307_INVALID_PARA;
	}
	return DS1307_ERROR;
}
static ds1307_status_t set_clock_haft(uint8_t bit_clock_haft)
{
	uint8_t value = bit_clock_haft ? DS1307_FLAG_CH : 0;
	uint8_t reg = 0;

	ds1307_status_t error_status = DS1307_OK;
	error_status = (ds1307_status_t)ds1307.i2c_read(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_SECONDS, &reg);
	__ASSERT((error_status == DS1307_OK), error_status);

	value |= (reg & DS1307_MASK_SECOND);
	error_status = (ds1307_status_t)ds1307.i2c_write(ds1307.ds1307_i2c, ds1307.device_addr, DS1307_REG_SECONDS, value);
	__ASSERT((error_status == DS1307_OK), error_status);

	return DS1307_OK;
}

/* End of file -------------------------------------------------------- */
