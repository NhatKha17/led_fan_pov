/**
 * @file       cjmcu75.h
 * @copyright  Copyright (C) 2019 Fiot Co., Ltd. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       2024-03-25
 * @author     Khoi Ly
 *
 * @brief      cjmcu75 driver
 */

/* Includes ----------------------------------------------------------- */
#include "cjmcu75.h"
/* Private defines ---------------------------------------------------- */
#define CJMCU75_TEMP_REG (0x00)
#define CJMCU75_CONFIG_REG (0x01)
#define CJMCU75_ADDRESS_8BIT (0b10010000)
#define CJMCU75_ADDRESS_7BIT (0b1001000)
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
/* Private variables -------------------------------------------------- */
static cjmcu75_t cjmcu75;
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
cjmcu75_status_t cjmcu75_init(cjmcu75_t sys_cjmcu75)
{
    __ASSERT((sys_cjmcu75.hi2c != NULL), CJMCU75_ERROR);
    __ASSERT((sys_cjmcu75.i2c_read_at != NULL), CJMCU75_ERROR);

    cjmcu75 = sys_cjmcu75;
    cjmcu75.device_address = CJMCU75_ADDRESS_8BIT;

#ifdef DEBUG
    HAL_StatusTypeDef error_handle = HAL_OK;
    error_handle = cjmcu75.i2c_check_device(cjmcu75.hi2c, cjmcu75.device_address);
    __ASSERT((error_handle == HAL_OK), CJMCU75_ERROR);
#endif

    return CJMCU75_OK;
}

int8_t cjmcu75_temp()
{
    uint8_t temp_reg[2];
    float temp;

    cjmcu75.i2c_read_at(cjmcu75.hi2c, cjmcu75.device_address, CJMCU75_TEMP_REG, temp_reg, 2);

    uint16_t temp_reg_full = ((temp_reg[0] << 8) & 0xFF00) | (temp_reg[1] & 0x00FF);

    // Check if the MSB bit is 1 (negative value)
    if (temp_reg_full & 0x8000)
    {
        temp_reg_full |= 0xF000; // Sign extension
        temp = (float)(temp_reg_full >> 8) - ((temp_reg_full >> 7) & 0x0001) * 0.5;
    }
    else
    {
        temp = (float)(temp_reg_full >> 8) + ((temp_reg_full >> 7) & 0x0001) * 0.5;
    }

    return (int8_t)temp;
}
/* End of file -------------------------------------------------------- */
