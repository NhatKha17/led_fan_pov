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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_DRV_DRV_UART_H_
#define INC_DRV_DRV_UART_H_
/* Includes ----------------------------------------------------------- */
#include "bsp_uart.h"
#include "cbuffer.h"
/* Public defines ----------------------------------------------------- */

/* Public enumerate/structure ----------------------------------------- */

/**
 * @brief enumeration define status driver uart
 */
typedef enum
{
	DRV_UART_OK = 0U,
	DRV_UART_ERROR,
} drv_uart_status_t;

typedef struct
{
	huart_serial_t *drv_uart_huart;

	bsp_uart_status_t (*drv_uart_read)(huart_serial_t *huart, uint8_t *rx_data, uint32_t bytes);
	bsp_uart_status_t (*drv_uart_print)(huart_serial_t *huart, uint8_t *tx_data, uint32_t data_len);
} drv_uart_t;
/* Public macros ------------------------------------------------------ */

/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Initialize UART driver.
 *
 * @param[in]   sys_uart        System UART instance to initialize.
 * @param[in]   cb              Circular buffer structure pointer for UART data buffering.
 * @param[in]   cb_buf          Pointer to the buffer used by the circular buffer.
 * @param[in]   cb_buf_size     Size of the circular buffer.
 *
 * @return      The status of the UART initialization operation.
 */
drv_uart_status_t drv_uart_init(drv_uart_t sys_uart, cbuffer_t *cb, uint8_t *cb_buf, uint32_t cb_buf_size);

/**
 * @brief       Receive data over UART.
 *
 * @param[out]  rx_buf          Pointer to the buffer to store received data.
 * @param[in]   rx_buf_size     Size of the receive buffer.
 *
 * @return      The status of the UART reception operation.
 */
drv_uart_status_t drv_uart_receive(uint8_t *rx_buf, uint32_t rx_buf_size);

/**
 * @brief       Transmit data over UART.
 *
 * @param[in]   tx_data     Pointer to the data to be transmitted.
 *
 * @return      The status of the UART transmission operation.
 */
drv_uart_status_t drv_uart_print(uint8_t *tx_data);

#endif /* INC_DRV_DRV_UART_H_ */
/* End of file -------------------------------------------------------- */
