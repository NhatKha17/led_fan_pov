/**
 * @file        bsp_spi.h
 * @copyright
 * @license
 * @version     1.0.0
 * @date       	29/03/2024
 * @author    	Kha Nguyen
 *
 * @brief       Board Support Package for spi use for ws2812
 *
 * @note        None
 * @example     None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef INC_BSP_BSP_SPI_H_
#define INC_BSP_BSP_SPI_H_
/* Includes ----------------------------------------------------------- */
#include "main.h"
/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
	BSP_SPI_OK = 0U,
	BSP_SPI_ERROR,
} bsp_spi_status_t;
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief       Transmit data over SPI using polling mode.
 *
 * @param[in]   hspi        Pointer to the SPI handle structure.
 * @param[in]   data        Pointer to the data to be transmitted.
 * @param[in]   size_data   Size of the data to be transmitted.
 *
 * @return      The status of the SPI transmission operation.
 */
bsp_spi_status_t bsp_spi_transmit(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size_data);

/**
 * @brief       Transmit data over SPI using DMA (Direct Memory Access) mode.
 *
 * @param[in]   hspi        Pointer to the SPI handle structure.
 * @param[in]   data        Pointer to the data to be transmitted.
 * @param[in]   size_data   Size of the data to be transmitted.
 *
 * @return      The status of the SPI transmission operation.
 */
bsp_spi_status_t bsp_spi_transmit_dma(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size_data);
#endif /* INC_BSP_BSP_SPI_H_ */
