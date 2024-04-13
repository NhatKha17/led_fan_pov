/**
 * @file       ds1307.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       21/02/2024
 * @author     Kha Nguyen
 *
 * @brief      driver ds1307
 *
 *
 */
/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_DRIVER_DS1307_H_
#define INC_DRIVER_DS1307_H_

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include "stdint.h"
#include <time.h>
#include <string.h>
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/**
 * @brief ds1307 am pm enumeration definition
 */
typedef enum
{
	DS1307_CH_ENABLE = 0U,	/* enable clock*/
	DS1307_CH_DISABLE = 1U, /* dsiable clock */
} ds1307_clock_haft_t;

/**
 * @brief ds1307 am pm enumeration definition
 */
typedef enum
{
	DS1307_AM = 0U, /* am */
	DS1307_PM = 1U, /* pm */
} ds1307_am_pm_t;

/**
 * @brief ds1307 format enumeration definition
 */
typedef enum
{
	DS1307_MODE_24H = 0U, /* mode 24h  */
	DS1307_MODE_12H = 1U, /* mode 12h  */
} ds1307_mode_t;

/**
 * @brief ds1307 status enumeration definition
 */
typedef enum
{
	DS1307_OK = 0U,		 /* OK */
	DS1307_ERROR,		 /* i2c read write error	*/
	DS1307_BUSY,		 /* bus i2c busy */
	DS1307_TIMEOUT,		 /* i2c timeout	*/
	DS1307_INVALID_PARA, /* parameter invalid*/
	DS1307_NULL_PTR,	 /* null ptr */
} ds1307_status_t;

/**
 * @brief  Structure for date/time
 */
typedef struct
{
	uint8_t hours;		  /* Hours parameter, 24Hour mode, 00 to 23 */
	uint8_t minutes;	  /* Minutes parameter, from 00 to 59 */
	uint8_t seconds;	  /* Seconds parameter, from 00 to 59 */
	uint8_t dow;		  /* Day in a week, from monday to sunday 1-7 */
	uint8_t date;		  /* Date in a month, 1 to 31 */
	uint8_t month;		  /* Month in a year, 1 to 12 */
	uint32_t year;		  /* Year parameter, 00 to 99, 00 is 2000 and 99 is 2099 */
	ds1307_mode_t mode;	  /* 12h or 24h */
	ds1307_am_pm_t am_pm; /* mode 12h am/pm */
} ds1307_time_t;


/**
 * @brief   Structure representing the configuration of the DS1307 Real-Time Clock module.
 */
typedef struct
{
	i2c_serial_t *ds1307_i2c; /* serial i2c using */

	uint8_t device_addr; /* i2c device address */

	bsp_i2c_status_t (*i2c_device_check)(i2c_serial_t *hi2c, uint8_t addr_slave);
	bsp_i2c_status_t (*i2c_write)(i2c_serial_t *hi2c, uint8_t addr_slave, uint8_t reg_write, uint8_t data_write);
	bsp_i2c_status_t (*i2c_read_multi)(i2c_serial_t *hi2c, uint8_t addr_slave, uint8_t reg_write, uint8_t *data_write, uint16_t size_data);
	bsp_i2c_status_t (*i2c_read)(i2c_serial_t *hi2c, uint8_t addr_slave, uint8_t reg_read, uint8_t *data_read);

} ds1307_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Initialize the DS1307 Real-Time Clock module.
 *
 * @param[in]   sys_ds1307     System DS1307 instance to initialize.
 *
 * @return      The status of the DS1307 initialization operation.
 */
ds1307_status_t ds1307_init(ds1307_t sys_ds1307);

/**
 * @brief       Get the current time from the DS1307 RTC.
 *
 * @param[out]  get_time    Pointer to a structure to store the retrieved time.
 *
 * @return      The status of the time retrieval operation.
 */
ds1307_status_t ds1307_get_time(ds1307_time_t *get_time);

/**
 * @brief       Set the time on the DS1307 RTC.
 *
 * @param[in]   set_time    Pointer to a structure containing the time to set.
 *
 * @return      The status of the time setting operation.
 */
ds1307_status_t ds1307_set_time(ds1307_time_t *set_time);

/**
 * @brief       Set the time on the DS1307 RTC using epoch time.
 *
 * @param[in]   epoch_time  The epoch time value to set.
 *
 * @return      The status of the epoch time setting operation.
 */
ds1307_status_t ds1307_set_by_epoch_time(uint32_t epoch_time);

/**
 * @brief       Set the DS1307 RTC to 12-hour mode.
 *
 * @param[in]   None
 *
 * @return      The status of the mode setting operation.
 */
ds1307_status_t ds1307_set_mode_12h();

/**
 * @brief       Set the DS1307 RTC to 24-hour mode.
 *
 * @param[in]   None
 *
 * @return      The status of the mode setting operation.
 */
ds1307_status_t ds1307_set_mode_24h();

/**
 * @brief       Enable clock halt mode on the DS1307 RTC.
 *
 * @param[in]   None
 *
 * @return      The status of the clock halt enabling operation.
 */
ds1307_status_t ds1307_set_clock_haft_enable();

/**
 * @brief       Disable clock halt mode on the DS1307 RTC.
 *
 * @param[in]   None
 *
 * @return      The status of the clock halt disabling operation.
 */
ds1307_status_t ds1307_set_clock_haft_disable();

#endif /* INC_DRIVER_DS1307_H_ */
/* End of file -------------------------------------------------------- */
