/**
 * @file       bsp_i2c.h
 *
 * @copyright
 * @license
 * @version    1.0.0
 * @date       15/02/2024
 * @author     Kha Nguyen
 *
 * @brief      i2c2
 *
 *
 */
#ifndef INC_BSP_I2C_H_
#define INC_BSP_I2C_H_
/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */
typedef I2C_HandleTypeDef i2c_serial_t;

typedef enum
{
	BSP_I2C_OK = 0U,
	BSP_I2C_ERROR,
	BSP_I2C_BUSY,
	BSP_I2C_TIMEOUT,
} bsp_i2c_status_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief       Write data to a register of a slave device on I2C1 bus.
 *
 * @param[in]   hi2c            Pointer to the I2C1 handle structure.
 * @param[in]   address_slave   Address of the slave device.
 * @param[in]   reg_write       Register address to write to.
 * @param[in]   data_write      Data to be written to the register.
 *
 * @return      The status of the I2C write operation.
 */
bsp_i2c_status_t bsp_i2c1_write(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_write, uint8_t data_write);

/**
 * @brief       Read data from a register of a slave device on I2C1 bus.
 *
 * @param[in]   hi2c            Pointer to the I2C1 handle structure.
 * @param[in]   address_slave   Address of the slave device.
 * @param[in]   reg_read        Register address to read from.
 * @param[out]  data_read       Pointer to store the read data.
 *
 * @return      The status of the I2C read operation.
 */
bsp_i2c_status_t bsp_i2c1_read(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read);

/**
 * @brief       Read multiple bytes of data from consecutive registers of a slave device on I2C1 bus.
 *
 * @param[in]   hi2c            Pointer to the I2C1 handle structure.
 * @param[in]   address_slave   Address of the slave device.
 * @param[in]   reg_read        Starting register address to read from.
 * @param[out]  data_read       Pointer to store the read data.
 * @param[in]   size_data       Number of bytes to read.
 *
 * @return      The status of the I2C read operation.
 */
bsp_i2c_status_t bsp_i2c1_read_multi(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data);

									/**
 * @brief       Read multiple bytes of data from consecutive registers of a slave device on I2C2 bus.
 *
 * @param[in]   hi2c            Pointer to the I2C2 handle structure.
 * @param[in]   address_slave   Address of the slave device.
 * @param[in]   reg_read        Starting register address to read from.
 * @param[out]  data_read       Pointer to store the read data.
 * @param[in]   size_data       Number of bytes to read.
 *
 * @return      The status of the I2C read operation.
 */
bsp_i2c_status_t bsp_i2c2_read_multi(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data);

/**
 * @brief       Check if a slave device is ready on the I2C1 bus.
 *
 * @param[in]   hi2c            Pointer to the I2C1 handle structure.
 * @param[in]   address_slave   Address of the slave device.
 *
 * @return      The status of the device readiness check operation.
 */
bsp_i2c_status_t bsp_i2c1_device_is_ready(i2c_serial_t *hi2c, uint8_t address_slave);

/**
 * @brief       Check if a slave device is ready on the I2C2 bus.
 *
 * @param[in]   hi2c            Pointer to the I2C2 handle structure.
 * @param[in]   address_slave   Address of the slave device.
 *
 * @return      The status of the device readiness check operation.
 */
bsp_i2c_status_t bsp_i2c2_device_is_ready(i2c_serial_t *hi2c, uint8_t address_slave);

#endif // __BSP_I2C_H

/* End of file -------------------------------------------------------- */
