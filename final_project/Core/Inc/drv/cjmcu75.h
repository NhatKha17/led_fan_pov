/**
 * @file       cjmcu75.h
 * @copyright  Copyright (C) 2019 Fiot Co., Ltd. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-03-25
 * @author     Khoi Ly
 *
 * @brief      CJMCU75 Driver
 *
 **/

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __DRIVER_CJMCU75_H
#define __DRIVER_CJMCU75_H

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
#include "stdint.h"
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
    CJMCU75_OK = 0U,
    CJMCU75_ERROR,
} cjmcu75_status_t;
/**
 * @brief Struct for cjmcu75 object
 */
typedef struct
{
    i2c_serial_t *hi2c;

    uint8_t device_address; /* I2C device address */

    bsp_i2c_status_t (*i2c_check_device)(i2c_serial_t *hi2c, uint8_t address_slave);
    bsp_i2c_status_t (*i2c_read_at)(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data);
} cjmcu75_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Initialize the CJMCU75 temperature sensor.
 *
 * @param[in]   sys_cjmcu75     System CJMCU75 instance to initialize.
 *
 * @return      The status of the CJMCU75 initialization operation.
 */
cjmcu75_status_t cjmcu75_init(cjmcu75_t sys_cjmcu75);

/**
 * @brief       Read the temperature from the CJMCU75 temperature sensor.
 *
 * @param[in]   None
 *
 * @return      The temperature read from the sensor.
 */
int8_t cjmcu75_temp();
#endif // __DRIVER_DS1307_H

/* End of file -------------------------------------------------------- */
