/**
 * @file       bsp_uart.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       27/02/2024
 * @author     Kha Nguyen
 *
 * @brief      bsp uart
 *
 * @note
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef BSP_USART_H
#define BSP_USART_H

/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public enumerate/structure ----------------------------------------- */
typedef UART_HandleTypeDef huart_serial_t;

typedef enum
{
	BSP_UART_OK = 0U,
	BSP_UART_ERROR,
	BSP_UART_BUSY,
	BSP_UART_TIMEOUT,
} bsp_uart_status_t;

/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */

/**
 * @brief       Read data from UART using DMA until an idle line is detected.
 *
 * @param[in]   huart       Pointer to the UART handle structure.
 * @param[out]  rx_data     Pointer to the buffer to store received data.
 * @param[in]   bytes       Number of bytes to read until an idle line is detected.
 *
 * @return      The status of the UART read operation.
 */
bsp_uart_status_t bsp_uart_read_to_idle_dma(huart_serial_t *huart, uint8_t *rx_data, uint32_t bytes);

/**
 * @brief       Print data over UART.
 *
 * @param[in]   huart       Pointer to the UART handle structure.
 * @param[in]   tx_data     Pointer to the data to be transmitted.
 *
 * @return      The status of the UART transmission operation.
 */
bsp_uart_status_t bsp_uart_print(huart_serial_t *huart, uint8_t *tx_data);

/**
 * @brief       Print data of specified length over UART.
 *
 * @param[in]   huart       Pointer to the UART handle structure.
 * @param[in]   tx_data     Pointer to the data to be transmitted.
 * @param[in]   data_len    Length of the data to be transmitted.
 *
 * @return      The status of the UART transmission operation.
 */
bsp_uart_status_t bsp_uart_print_length(huart_serial_t *huart, uint8_t *tx_data, uint32_t data_len);

#endif // __BSP_I2C_H

/* End of file -------------------------------------------------------- */
