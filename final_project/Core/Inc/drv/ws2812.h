/**
 * @file       ws2812.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       29/03/2024
 * @author     Kha Nguyen
 *
 * @brief      Driver for led trip ws2812
 **/
#ifndef INC_WS2812_H_
#define INC_WS2812_H_
/* Includes ----------------------------------------------------------- */
#include "bsp_spi.h"
#include "string.h"
/* Public defines ----------------------------------------------------- */
#define WS2812_BRIGHTNESS_DEFAULT (10)
#define WS2812_LED_NUMS (13)

/* Public enumerate/structure ----------------------------------------- */
typedef struct
{
	SPI_HandleTypeDef *hspi;

	bsp_spi_status_t (*spi_transmit_dma)(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size_data);
	bsp_spi_status_t (*spi_transmit)(SPI_HandleTypeDef *hspi, uint8_t *data, uint16_t size_data);
} ws2812_t;
typedef enum
{
	WS2812_OK = 0U,
	WS2812_ERROR,
} ws2812_status_t;
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  		This function initializes the WS2812 LED controller
 *
 * @param[in]   None
 *
 * @note		Turn off all led
 *
 * @return		ws2812_status_t
 */
ws2812_status_t ws2812_init(ws2812_t ws2812_inited);

/**
 * @brief  		This function sets the RGB (Red, Green, Blue) color values for a specific LED in the WS2812 LED strip
 *
 * @param[in]   led_no		parameter specifies the index of the LED to be updated.
 * @param[in]   red		 	ranging from 0 (off) to 255 (full intensity)
 * @param[in]   green		ranging from 0 (off) to 255 (full intensity)
 * @param[in]   blue		ranging from 0 (off) to 255 (full intensity)
 *
 * @return		None
 */
void ws2812_pixel(uint16_t led_no, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief  		This function sets the RGB (Red, Green, Blue) color values for a specific LED in the WS2812 LED strip
 *
 * @param[in]   red		 	ranging from 0 (off) to 255 (full intensity)
 * @param[in]   green		ranging from 0 (off) to 255 (full intensity)
 * @param[in]   blue		ranging from 0 (off) to 255 (full intensity)
 *
 * @return		None
 */
void ws2812_pixel_all(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief  		This function sets brightness for led
 *
 * @param[in]   brightness		 	ranging from 0 to 100
 *
 * @return		ws2812_status_t
 */
ws2812_status_t ws2812_set_brightness(uint8_t brightness);

#endif /* INC_WS2812_H_ */
/* End of file -------------------------------------------------------- */
