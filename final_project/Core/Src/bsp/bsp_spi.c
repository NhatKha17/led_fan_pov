/**
 * @file        bsp_spi.c
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

/* Includes ----------------------------------------------------------- */
#include <bsp_spi.h>
/* Private defines ---------------------------------------------------- */
#define BSP_SPI_TIMEOUT (1000)
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
bsp_spi_status_t bsp_spi_transmit(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size_data)
{

	if (HAL_SPI_Transmit(hspi, data, size_data, BSP_SPI_TIMEOUT) != HAL_OK)
	{
		return BSP_SPI_ERROR;
	}
	return BSP_SPI_OK;
}

bsp_spi_status_t bsp_spi_transmit_dma(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size_data)
{

	if (HAL_SPI_Transmit_DMA(hspi, data, size_data) != HAL_OK)
	{
		return BSP_SPI_ERROR;
	}

	return BSP_SPI_OK;
}
/* End of file -------------------------------------------------------- */
