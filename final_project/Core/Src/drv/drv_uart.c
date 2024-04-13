/**
 * @file       drv_uart.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       20/03/2024
 * @author     Kha Nguyen
 *
 * @brief      driver uart
 *
 *
 */
/* Includes ----------------------------------------------------------- */
#include "drv_uart.h"
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

#ifdef DEBUG
#define __RETURN(_EXPR_, _RET_VAL_) \
	do                              \
	{                               \
		if (!(_EXPR_))              \
			return _RET_VAL_;       \
	} while (0)
#else
#define __RETURN(_EXPR_, _RET_VAL_) ((void)0)
#endif
/* Public variables --------------------------------------------------- */
extern DMA_HandleTypeDef hdma_usart2_rx;
/* Private variables -------------------------------------------------- */
static drv_uart_t drv_uart;
/* Private function prototypes ---------------------------------------- */

/* Function definitions ----------------------------------------------- */
drv_uart_status_t drv_uart_init(drv_uart_t sys_uart, cbuffer_t *cb, uint8_t *cb_buf, uint32_t cb_buf_size)
{
	__ASSERT((cb != NULL), DRV_UART_ERROR);
	__ASSERT((cb_buf != NULL), DRV_UART_ERROR);

	drv_uart = sys_uart;
	cb_init(cb, cb_buf, cb_buf_size);

	return DRV_UART_OK;
}

drv_uart_status_t drv_uart_receive(uint8_t *rx_buf, uint32_t rx_buf_size)
{
	__ASSERT((rx_buf != NULL), DRV_UART_ERROR);

	bsp_uart_status_t error_status = BSP_UART_OK;
	error_status = drv_uart.drv_uart_read(drv_uart.drv_uart_huart, (uint8_t *)rx_buf, rx_buf_size);
	__RETURN((error_status == BSP_UART_OK), DRV_UART_ERROR);
	__HAL_DMA_DISABLE_IT(&hdma_usart2_rx, DMA_IT_HT);
	return DRV_UART_OK;
}

drv_uart_status_t drv_uart_print(uint8_t *tx_data)
{
	__ASSERT((tx_data != NULL), DRV_UART_ERROR);

	bsp_uart_status_t error_status = BSP_UART_OK;
	error_status = drv_uart.drv_uart_print(drv_uart.drv_uart_huart, (uint8_t *)tx_data, strlen((char *)tx_data));
	__RETURN((error_status == BSP_UART_OK), DRV_UART_ERROR);

	return DRV_UART_OK;
}

/* Private function definitions ----------------------------------------------- */

/* End of file -------------------------------------------------------- */
