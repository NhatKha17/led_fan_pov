/**
 * @file       bsp_i2c.c
 *
 * @copyright
 * @license
 * @version    1.0.0
 * @date       15/02/2024
 * @author     Kha Nguyen
 *
 * @brief      i2c1
 *
 *
 */
/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"
/* Private defines ---------------------------------------------------- */
#define BSP_I2C_TIMEOUT (HAL_MAX_DELAY)
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

/* Private function prototypes ---------------------------------------- */
static bsp_i2c_status_t bsp_i2c_check_busy(i2c_serial_t *hi2c);
/* Function definitions ----------------------------------------------- */


bsp_i2c_status_t bsp_i2c1_write(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_write, uint8_t data_write)
{
	__ASSERT((hi2c != NULL), BSP_I2C_ERROR);

		HAL_StatusTypeDef error_status = HAL_OK;
		/* Check I2C2*/
		error_status = bsp_i2c_check_busy(hi2c);
		__RETURN((error_status == HAL_OK), error_status);

		/* Create data including reg address and data*/
		uint8_t data[2] = {reg_write, data_write};

		error_status = HAL_I2C_Master_Transmit(hi2c, address_slave, data, 2, BSP_I2C_TIMEOUT);
		__RETURN((error_status == HAL_OK), error_status);

		return HAL_OK;
}

bsp_i2c_status_t bsp_i2c1_read(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read)
{
	__ASSERT((hi2c != NULL), BSP_I2C_ERROR);
	__ASSERT((data_read != NULL), BSP_I2C_ERROR);

		HAL_StatusTypeDef error_status = HAL_OK;

		/* Check I2C2 was initialized*/
		error_status = bsp_i2c_check_busy(hi2c);
		__RETURN((error_status == HAL_OK), error_status);

		error_status = HAL_I2C_Master_Transmit(hi2c, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT);
		__RETURN((error_status == HAL_OK), error_status);

		error_status = HAL_I2C_Master_Receive(hi2c, address_slave, data_read, 1, BSP_I2C_TIMEOUT);
		__RETURN((error_status == HAL_OK), error_status);

		return HAL_OK;
}

bsp_i2c_status_t bsp_i2c1_read_multi(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data)
{
	__ASSERT((hi2c != NULL), BSP_I2C_ERROR);
	__ASSERT((data_read != NULL), BSP_I2C_ERROR);

//		HAL_StatusTypeDef error_status = HAL_OK;
//
//		/* Check I2C1 was initialized*/
//		error_status = bsp_i2c_check_busy(hi2c);
//		__ASSERT((error_status == HAL_OK), error_status);
//
//		if (HAL_I2C_Mem_Read(hi2c, address_slave, reg_read, I2C_MEMADD_SIZE_8BIT, data_read, size_data, BSP_I2C_TIMEOUT) != HAL_OK)
//		    return HAL_ERROR;
//
//		return HAL_OK;
	HAL_StatusTypeDef error_status = HAL_OK;

	/* Check I2C1 was initialized*/
	/* Check I2C1 was initialized*/
	error_status = bsp_i2c_check_busy(hi2c);
	__RETURN((error_status == HAL_OK), error_status);

	/* Transmit data
	 * Generate start bit
	 * Send 7 bit SLAVE ADDRESS + 1 bit (0) ~ (W) == 8 bit
	 * Check ACK
	 * Send REG ADDRESS
	 * Check ACK
	 * Generate stop bit*/
	error_status = HAL_I2C_Master_Transmit(hi2c, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT);
	__RETURN((error_status == HAL_OK), error_status);

	/* Receive data
	 * Generate start bit
	 * Send 7 bit SLAVE ADDRESS + 1 bit (1) ~ (R) == 8 bit
	 * Check ACK
	 * Read data 1 byte at a time
	 * Generate stop bit*/
	error_status = HAL_I2C_Master_Receive(hi2c, address_slave, data_read, size_data, BSP_I2C_TIMEOUT);
	__RETURN((error_status == HAL_OK), error_status);

	return HAL_OK;
}

bsp_i2c_status_t bsp_i2c1_device_is_ready(i2c_serial_t *hi2c, uint8_t address_slave)
{
	HAL_StatusTypeDef error_status = HAL_OK;
	error_status = HAL_I2C_IsDeviceReady(hi2c, address_slave, 3, BSP_I2C_TIMEOUT);
	__RETURN((error_status == HAL_OK), error_status);
	return HAL_OK;
}

bsp_i2c_status_t bsp_i2c2_read_multi(i2c_serial_t *hi2c, uint8_t address_slave, uint8_t reg_read, uint8_t *data_read, uint16_t size_data)
{
	__ASSERT((hi2c != NULL), BSP_I2C_ERROR);
	__ASSERT((data_read != NULL), BSP_I2C_ERROR);

	HAL_StatusTypeDef error_status = HAL_OK;

	/* Check I2C1 was initialized*/
	/* Check I2C1 was initialized*/
	error_status = bsp_i2c_check_busy(hi2c);
	__RETURN((error_status == HAL_OK), error_status);

	/* Transmit data
	 * Generate start bit
	 * Send 7 bit SLAVE ADDRESS + 1 bit (0) ~ (W) == 8 bit
	 * Check ACK
	 * Send REG ADDRESS
	 * Check ACK
	 * Generate stop bit*/
	error_status = HAL_I2C_Master_Transmit(hi2c, address_slave, &reg_read, 1, BSP_I2C_TIMEOUT);
	__RETURN((error_status == HAL_OK), error_status);

	/* Receive data
	 * Generate start bit
	 * Send 7 bit SLAVE ADDRESS + 1 bit (1) ~ (R) == 8 bit
	 * Check ACK
	 * Read data 1 byte at a time
	 * Generate stop bit*/
	error_status = HAL_I2C_Master_Receive(hi2c, address_slave, data_read, size_data, BSP_I2C_TIMEOUT);
	__RETURN((error_status == HAL_OK), error_status);

	return HAL_OK;
//		HAL_StatusTypeDef error_status = HAL_OK;
//
////		/* Check I2C1 was initialized*/
//		error_status = bsp_i2c_check_busy(hi2c);
//		__RETURN((error_status == HAL_OK), error_status);
//
//		if (HAL_I2C_Mem_Read(hi2c, address_slave, reg_read, I2C_MEMADD_SIZE_8BIT, data_read, size_data, BSP_I2C_TIMEOUT) != HAL_OK)
//		    return HAL_ERROR;
//
//		return HAL_OK;
}
bsp_i2c_status_t bsp_i2c2_device_is_ready(i2c_serial_t *hi2c, uint8_t address_slave)
{
	HAL_StatusTypeDef error_status = HAL_OK;
	error_status = HAL_I2C_IsDeviceReady(hi2c, address_slave, 3, BSP_I2C_TIMEOUT);
	__ASSERT((error_status == HAL_OK), error_status);
	return HAL_OK;
}

/* Private function definitions ----------------------------------------------- */
/**
 * @brief  		check i2c is busy
 *
 * @param[in]   hi2c	i2c structure
 *
 * @return		HAL status
 *
 */
static bsp_i2c_status_t bsp_i2c_check_busy(i2c_serial_t *hi2c)
{
	__ASSERT((hi2c != NULL), BSP_I2C_ERROR);
	if (hi2c->Instance == I2C1)
	{
		if (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
			return HAL_ERROR;
		return HAL_OK;
	}
	else
		return HAL_ERROR;
	if (hi2c->Instance == I2C2)
	{
		if (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY)
			return HAL_ERROR;
		return HAL_OK;
	}
	else
		return HAL_ERROR;
}

/* End of file -------------------------------------------------------- */
