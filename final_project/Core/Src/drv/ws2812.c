/**
 * @file       ws2812.h
 * @copyright
 * @license
 * @version    1.0.0
 * @date       29/03/2024
 * @author     Kha Nguyen
 *
 * @brief      Driver for led trip ws2812
 */
/* Includes ----------------------------------------------------------- */
#include "ws2812.h"
/* Private defines ---------------------------------------------------- */
#define SPI_DMA_TRANSMIT
#define WS2812_BRIGHTNESS_MAX (100)
#define WS2812_BRIGHTNESS_MIN (0)
/*
 * @note 1 pulse = 1/2.25mbit/s -> 1 pulse = 0.44 us
 */
#define WS2812_RESET_PULSE (60) // reset pulse with ws2812 > 50us
#define WS2812_LENGTH_DATA (24) // frame data 24 bit
#define WS2812_BUFFER_SIZE (WS2812_LED_NUMS * WS2812_LENGTH_DATA + WS2812_RESET_PULSE)

#define WS2812_LED_DATA_GREEN (0)
#define WS2812_LED_DATA_RED (1)
#define WS2812_LED_DATA_BLUE (2)
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
#define WS2812_FILL_BUFFER(COLOR)               \
	for (uint8_t mask = 0x80; mask; mask >>= 1) \
	{                                           \
		if (COLOR & mask)                       \
		{                                       \
			*ptr++ = 0x06;                      \
		}                                       \
		else                                    \
		{                                       \
			*ptr++ = 0x04;                      \
		}                                       \
	}
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
static ws2812_t ws2812;
static uint8_t ws2812_buffer[WS2812_BUFFER_SIZE];
static uint8_t ws2812_led_data[WS2812_LED_NUMS][3]; // save data led

static uint8_t ws2812_brightness = WS2812_BRIGHTNESS_DEFAULT; // from 0 - 100
/* Private function prototypes ---------------------------------------- */
static void ws2812_send_spi(void);

static ws2812_status_t ws2812_update_brightness(uint8_t brightness);
/* Function definitions ----------------------------------------------- */
ws2812_status_t ws2812_init(ws2812_t ws2812_init)
{
	__ASSERT((ws2812_init.hspi != NULL) ||
			(ws2812_init.spi_transmit != NULL) ||
			(ws2812_init.spi_transmit_dma != NULL), WS2812_ERROR);

	ws2812 = ws2812_init;

	ws2812_pixel_all(0, 0, 0);

	ws2812_send_spi(); // Enable SPI DMA only once here

	return WS2812_OK;
}

void ws2812_pixel(uint16_t led_no, uint8_t red, uint8_t green, uint8_t blue)
{
	ws2812_led_data[led_no][WS2812_LED_DATA_GREEN] = green;
	ws2812_led_data[led_no][WS2812_LED_DATA_RED] = red;
	ws2812_led_data[led_no][WS2812_LED_DATA_BLUE] = blue;

	uint8_t *ptr = &ws2812_buffer[WS2812_LENGTH_DATA * led_no];

	WS2812_FILL_BUFFER(ws2812_led_data[led_no][WS2812_LED_DATA_GREEN] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
	WS2812_FILL_BUFFER(ws2812_led_data[led_no][WS2812_LED_DATA_RED] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
	WS2812_FILL_BUFFER(ws2812_led_data[led_no][WS2812_LED_DATA_BLUE] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
}

void ws2812_pixel_all(uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t *ptr = ws2812_buffer;
	for (uint16_t i = 0; i < WS2812_LED_NUMS; i++)
	{
		ws2812_led_data[i][WS2812_LED_DATA_GREEN] = green;
		ws2812_led_data[i][WS2812_LED_DATA_RED] = red;
		ws2812_led_data[i][WS2812_LED_DATA_BLUE] = blue;

		WS2812_FILL_BUFFER(ws2812_led_data[i][WS2812_LED_DATA_GREEN] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
		WS2812_FILL_BUFFER(ws2812_led_data[i][WS2812_LED_DATA_RED] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
		WS2812_FILL_BUFFER(ws2812_led_data[i][WS2812_LED_DATA_BLUE] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
	}
	return WS2812_OK;
}

ws2812_status_t ws2812_set_brightness(uint8_t brightness)
{
	if (brightness > WS2812_BRIGHTNESS_MAX)
		ws2812_brightness = WS2812_BRIGHTNESS_MAX;
	else if (brightness < WS2812_BRIGHTNESS_MIN)
		ws2812_brightness = WS2812_BRIGHTNESS_MIN;
	else
		ws2812_brightness = brightness;

	ws2812_status_t error_hanlde = WS2812_OK;
	error_hanlde = ws2812_update_brightness(ws2812_brightness);
	__ASSERT((error_hanlde == WS2812_OK), WS2812_ERROR);

	return WS2812_OK;
}

/* Private function definitions ----------------------------------------------- */
/**
 * @brief  		This function sends the data over SPI (Serial Peripheral Interface) to the WS2812 LED strip
 *
 * @param[in]   None
 *
 * @note		Send data by SPI DMA
 *
 * @return		ws2812_status_t
 */
static void ws2812_send_spi(void)
{
#ifdef SPI_DMA_TRANSMIT

	ws2812.spi_transmit_dma(ws2812.hspi, ws2812_buffer, WS2812_BUFFER_SIZE);

#else

	ws2812_status_t error_handle = WS2812_OK;
	error_handle = (ws2812_status_t)ws2812.spi_transmit(ws2812.hspi, ws2812_buffer, WS2812_BUFFER_SIZE);
	__ASSERT((error_handle == WS2812_OK), WS2812_ERROR);

	return WS2812_OK;
#endif
}

static ws2812_status_t ws2812_update_brightness()
{
	uint8_t *ptr = ws2812_buffer;
	for (uint8_t i = 0; i < WS2812_LED_NUMS; i++)
	{
		WS2812_FILL_BUFFER(ws2812_led_data[i][WS2812_LED_DATA_GREEN] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
		WS2812_FILL_BUFFER(ws2812_led_data[i][WS2812_LED_DATA_RED] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
		WS2812_FILL_BUFFER(ws2812_led_data[i][WS2812_LED_DATA_BLUE] * ws2812_brightness / WS2812_BRIGHTNESS_MAX);
	}
	return WS2812_OK;
}
/* End of file -------------------------------------------------------- */
