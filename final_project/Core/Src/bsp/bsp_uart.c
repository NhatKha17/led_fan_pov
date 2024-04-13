/**
 * @file       bsp_uart.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       03/09/2024
 * @author     Kha Nguyen
 *
 * @brief      bsp uart
 *
 * @note
 */
/* Includes ----------------------------------------------------------- */
#include "bsp_uart.h"
#include "string.h"
/* Private defines ---------------------------------------------------- */
#define TIME_OUT_TRANSMIT_UART (1000)
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
/* Function definitions ----------------------------------------------- */
/* Function definitions ----------------------------------------------- */

bsp_uart_status_t bsp_uart_read_to_idle_dma(huart_serial_t *huart,
											uint8_t *rx_data, uint32_t bytes)
{
	__ASSERT((huart != NULL), BSP_UART_ERROR);
	__ASSERT((rx_data != NULL), BSP_UART_ERROR);

	bsp_uart_status_t error_status = BSP_UART_OK;
	error_status = (bsp_uart_status_t)HAL_UARTEx_ReceiveToIdle_DMA(huart, rx_data, bytes);
	__RETURN((error_status == BSP_UART_OK), error_status);

	return BSP_UART_OK;
}

bsp_uart_status_t bsp_uart_print(huart_serial_t *huart,
								 uint8_t *tx_data)
{
	__ASSERT((huart != NULL), BSP_UART_ERROR);
	__ASSERT((tx_data != NULL), BSP_UART_ERROR);

	bsp_uart_status_t error_status = BSP_UART_OK;
	error_status = (bsp_uart_status_t)HAL_UART_Transmit(huart, tx_data, strlen((char *)tx_data),
														TIME_OUT_TRANSMIT_UART);
	__RETURN((error_status == BSP_UART_OK), error_status);

	return BSP_UART_OK;
}

bsp_uart_status_t bsp_uart_print_length(huart_serial_t *huart,
										uint8_t *tx_data, uint32_t data_len)
{
	__ASSERT((huart != NULL), BSP_UART_ERROR);
	__ASSERT((tx_data != NULL), BSP_UART_ERROR);

	bsp_uart_status_t error_status = BSP_UART_OK;
	error_status = (bsp_uart_status_t)HAL_UART_Transmit(huart, tx_data, data_len,
														TIME_OUT_TRANSMIT_UART);
	__RETURN((error_status == BSP_UART_OK), error_status);

	return BSP_UART_OK;
}
/* Private function definitions ----------------------------------------------- */
/* End of file -------------------------------------------------------- */
