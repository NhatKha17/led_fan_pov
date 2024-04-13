/**
 * @file       sys_display.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       31/03/2024
 * @author     Kha Nguyen
 *
 * @brief      system display for display on led fan
 */

/* Includes ----------------------------------------------------------- */
#include "sys_display.h"
/* Private defines ---------------------------------------------------- */
#define SYS_DISPLAY_RESOLUTION (0x3C)

/*define information for struct hall_t*/
#define SYS_DISPLAY_HALL_TIM (&htim3)
#define SYS_DISPLAY_HALL_INPUT_CAPTURE_CHANNEL (TIM_CHANNEL_1)
#define SYS_DISPLAY_HALL_OUTPUT_COMPARE_CHANNEL (TIM_CHANNEL_2)
#define SYS_DISPLAY_HALL_FLAG_IRQ_INPUT_CAPTURE (TIM_FLAG_CC1)
#define SYS_DISPLAY_HALL_FLAG_IRQ_OUTPUT_COMPARE (TIM_FLAG_CC2)
#define SYS_DISPLAY_HALL_TIM_COUNTER_MAX (0xffff) // period counter of timer 3

/*display time*/
#define SYS_DISPLAY_CHARACTER_NUMBER (11) // using define array Character[SYS_DISPLAY_CHARACTER_NUMBER][SYS_DISPLAY_CHARACTER_COLUMN]
#define SYS_DISPLAY_CHARACTER_COLUMN (6)  // using define array Character[SYS_DISPLAY_CHARACTER_NUMBER][SYS_DISPLAY_CHARACTER_COLUMN]
#define SYS_DISPLAY_CHARACTER_ROW (7)	  // from led 12 to led 18
#define SYS_DISPLAY_TIME_LED_NUM_MIN (1)  // led start display time
#define SYS_DISPLAY_TIME_LED_NUM_MAX (7)  // led stop display time
#define SYS_DISPLAY_POINT_START (6)
#define SYS_DISPLAY_WIDTH_CHARACTER (5)

/*display colon*/
#define SYS_DISPLAY_PIXEL_COLON (SYS_DISPLAY_LED_NO_3)
/*display alarm*/
#define SYS_DISPLAY_PIXEL_ALARM (SYS_DISPLAY_LED_NO_12)
/*display outline*/
#define SYS_DISPLAY_PIXEL_OUTLINE (SYS_DISPLAY_LED_NO_13)
/*define information for structure ws2812_t*/
#define SYS_DISPLAY_WS2812_SPI (&hspi1)
/* Private enumerate/structure ---------------------------------------- */
/*typedef enum using for sys_display_number_time and sys_display_number_day*/
typedef enum
{
	SYS_DISPLAY_LED_NO_1 = 0U,
	SYS_DISPLAY_LED_NO_2,
	SYS_DISPLAY_LED_NO_3,
	SYS_DISPLAY_LED_NO_4,
	SYS_DISPLAY_LED_NO_5,
	SYS_DISPLAY_LED_NO_6,
	SYS_DISPLAY_LED_NO_7,
	SYS_DISPLAY_LED_NO_8,
	SYS_DISPLAY_LED_NO_9,
	SYS_DISPLAY_LED_NO_10,
	SYS_DISPLAY_LED_NO_11,
	SYS_DISPLAY_LED_NO_12,
	SYS_DISPLAY_LED_NO_13
} sys_display_led_no_t;

typedef struct
{
	uint8_t sys_display_color_time[3];
	uint8_t sys_display_color_alarm[3];
	uint8_t sys_display_color_outline[3];
	uint8_t sys_display_color_colon[3];
} sys_display_color_t;

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
extern TIM_HandleTypeDef htim3; // for hall sensor
extern SPI_HandleTypeDef hspi1; // for ws2812
/* Private variables -------------------------------------------------- */
/*variable calculator interrupt for cycle display*/
volatile uint8_t sys_display_interrupt_timer_count_overflow = 0;
volatile uint32_t sys_display_compare_value = 0;
volatile uint32_t sys_display_count_oc = 0;
/*variable calculator sys_display_fan*/
static uint8_t hours_unit = 0; // save character value after saparated
static uint8_t hours_tens = 0;

static uint8_t minutes_unit = 0;
static uint8_t minutes_tens = 0;

static uint8_t seconds_unit = 0;
static uint8_t seconds_tens = 0;

static uint8_t character_number[SYS_DISPLAY_CHARACTER_NUMBER][SYS_DISPLAY_CHARACTER_COLUMN] =
	{
		{0x7f, 0x41, 0x41, 0x41, 0x7f}, // 0
		{0x11, 0x21, 0x7f, 0x01, 0x01}, // 1
		{0x4f, 0x49, 0x49, 0x49, 0x79}, // 2
		{0x49, 0x49, 0x49, 0x49, 0x7f}, // 3
		{0x78, 0x08, 0x08, 0x08, 0x7f}, // 4
		{0x79, 0x49, 0x49, 0x49, 0x4f}, // 5
		{0x7f, 0x49, 0x49, 0x49, 0x4f}, // 6
		{0x40, 0x40, 0x4f, 0x50, 0x60}, // 7
		{0x7f, 0x49, 0x49, 0x49, 0x7f}, // 8
		{0x79, 0x49, 0x49, 0x49, 0x7f}	// 9
};
static sys_display_time_t sdisplay_data_time = {.hours = 00, .minutes = 00, .seconds = 00, .date = 05, .month = 04, .year = 2024};
static bool sdisplay_flag_alarm = true;
/*variable sys_display_set_color*/
static sys_display_color_t sys_display_color = {.sys_display_color_time[0] = SYS_DISPLAY_COLOR_TIME_GREEN_1,
												.sys_display_color_time[1] = SYS_DISPLAY_COLOR_TIME_RED_1,
												.sys_display_color_time[2] = SYS_DISPLAY_COLOR_TIME_BLUE_1,
												.sys_display_color_alarm[0] = SYS_DISPLAY_COLOR_ALARM_GREEN_1,
												.sys_display_color_alarm[1] = SYS_DISPLAY_COLOR_ALARM_RED_1,
												.sys_display_color_alarm[2] = SYS_DISPLAY_COLOR_ALARM_BLUE_1,
												.sys_display_color_outline[0] = SYS_DISPLAY_COLOR_OUTLINE_GREEN_1,
												.sys_display_color_outline[1] = SYS_DISPLAY_COLOR_OUTLINE_RED_1,
												.sys_display_color_outline[2] = SYS_DISPLAY_COLOR_OUTLINE_BLUE_1,
												.sys_display_color_colon[0] = SYS_DISPLAY_COLOR_COLON_GREEN_1,
												.sys_display_color_colon[1] = SYS_DISPLAY_COLOR_COLON_RED_1,
												.sys_display_color_colon[2] = SYS_DISPLAY_COLOR_COLON_BLUE_1

};
/* Private function prototypes ---------------------------------------- */
static sys_display_status_t sys_display_hall_init();
static sys_display_status_t sys_display_ws2812_init();

static void sys_display_fan_all_pixel_time_off();
static void sys_display_fan_pixel_time(sys_display_led_no_t led_no);
static void sys_display_fan_colon();
static void sys_display_fan_pixel_colon(sys_display_led_no_t led_no);
static void sys_display_fan_outline();
static void sys_display_fan_pixel_outline(sys_display_led_no_t led_no);
static void sys_display_fan_alarm();
static void sys_display_fan_pixel_alarm(sys_display_led_no_t led_no);
static void sys_display_fan_alarm_off();

static void sys_display_character(uint8_t value, uint8_t column);
static void sys_display_fan_separate_2_digit(uint8_t number, uint8_t *unit, uint8_t *tens);

static void sys_display_loop();

/* Function definitions ----------------------------------------------- */
sys_display_status_t sys_display_start()
{
	sys_display_status_t error_handle = SYS_DISPLAY_OK;
	error_handle = sys_display_hall_init(); // initialize hall sensor
	__RETURN((error_handle == SYS_DISPLAY_OK), SYS_DISPLAY_ERROR);

	error_handle = sys_display_ws2812_init(); // initialize led ws2812
	__RETURN((error_handle == SYS_DISPLAY_OK), SYS_DISPLAY_ERROR);

	return SYS_DISPLAY_OK;
}

void sys_display_fan(sys_display_time_t data_time, bool flag_alarm)
{
	sdisplay_data_time = data_time;	  // assign value data for sdisplay_data_time
	sdisplay_flag_alarm = flag_alarm; ////assign value flag alarm for static global variable
}

sys_display_status_t sys_display_set_brightness(uint8_t sys_brightness)
{
	ws2812_status_t error_handle = WS2812_OK;
	error_handle = ws2812_set_brightness(sys_brightness);
	__RETURN((error_handle == WS2812_OK), SYS_DISPLAY_ERROR);

	return SYS_DISPLAY_OK;
}

sys_display_status_t sys_display_set_color(sys_display_color_default_t color_no)
{
	switch (color_no)
	{
	case SYS_DISPLAY_COLOR_DEFAULT_1:
	{
		sys_display_color.sys_display_color_time[0] = SYS_DISPLAY_COLOR_TIME_GREEN_1;
		sys_display_color.sys_display_color_time[1] = SYS_DISPLAY_COLOR_TIME_RED_1;
		sys_display_color.sys_display_color_time[2] = SYS_DISPLAY_COLOR_TIME_BLUE_1;

		sys_display_color.sys_display_color_alarm[0] = SYS_DISPLAY_COLOR_ALARM_GREEN_1;
		sys_display_color.sys_display_color_alarm[1] = SYS_DISPLAY_COLOR_ALARM_RED_1;
		sys_display_color.sys_display_color_alarm[2] = SYS_DISPLAY_COLOR_ALARM_BLUE_1;

		sys_display_color.sys_display_color_outline[0] = SYS_DISPLAY_COLOR_OUTLINE_GREEN_1;
		sys_display_color.sys_display_color_outline[1] = SYS_DISPLAY_COLOR_OUTLINE_RED_1;
		sys_display_color.sys_display_color_outline[2] = SYS_DISPLAY_COLOR_OUTLINE_BLUE_1;

		sys_display_color.sys_display_color_colon[0] = SYS_DISPLAY_COLOR_COLON_GREEN_1;
		sys_display_color.sys_display_color_colon[1] = SYS_DISPLAY_COLOR_COLON_RED_1;
		sys_display_color.sys_display_color_colon[2] = SYS_DISPLAY_COLOR_COLON_BLUE_1;

		return SYS_DISPLAY_OK;
	}
	case SYS_DISPLAY_COLOR_DEFAULT_2:
	{
		sys_display_color.sys_display_color_time[0] = SYS_DISPLAY_COLOR_TIME_GREEN_2;
		sys_display_color.sys_display_color_time[1] = SYS_DISPLAY_COLOR_TIME_RED_2;
		sys_display_color.sys_display_color_time[2] = SYS_DISPLAY_COLOR_TIME_BLUE_2;

		sys_display_color.sys_display_color_alarm[0] = SYS_DISPLAY_COLOR_ALARM_GREEN_2;
		sys_display_color.sys_display_color_alarm[1] = SYS_DISPLAY_COLOR_ALARM_RED_2;
		sys_display_color.sys_display_color_alarm[2] = SYS_DISPLAY_COLOR_ALARM_BLUE_2;

		sys_display_color.sys_display_color_outline[0] = SYS_DISPLAY_COLOR_OUTLINE_GREEN_2;
		sys_display_color.sys_display_color_outline[1] = SYS_DISPLAY_COLOR_OUTLINE_RED_2;
		sys_display_color.sys_display_color_outline[2] = SYS_DISPLAY_COLOR_OUTLINE_BLUE_2;

		sys_display_color.sys_display_color_colon[0] = SYS_DISPLAY_COLOR_COLON_GREEN_2;
		sys_display_color.sys_display_color_colon[1] = SYS_DISPLAY_COLOR_COLON_RED_2;
		sys_display_color.sys_display_color_colon[2] = SYS_DISPLAY_COLOR_COLON_BLUE_2;

		return SYS_DISPLAY_OK;
	}
	}
	return SYS_DISPLAY_ERROR;
}

/* Private function definitions --------------------------------------- */
static sys_display_status_t sys_display_hall_init()
{
	hall_t sdisplay_hall;
	sdisplay_hall.htim = SYS_DISPLAY_HALL_TIM;

	sdisplay_hall.capture_channel = SYS_DISPLAY_HALL_INPUT_CAPTURE_CHANNEL;
	sdisplay_hall.compare_channel = SYS_DISPLAY_HALL_OUTPUT_COMPARE_CHANNEL;

	sdisplay_hall.flag_channel_irq[0] = SYS_DISPLAY_HALL_FLAG_IRQ_INPUT_CAPTURE;
	sdisplay_hall.flag_channel_irq[1] = SYS_DISPLAY_HALL_FLAG_IRQ_OUTPUT_COMPARE;

	sdisplay_hall.tim_get_counter = bsp_tim_get_counter;
	sdisplay_hall.tim_reset_counter = bsp_tim_reset_counter;
	sdisplay_hall.tim_read_capture = bsp_tim_read_capture;
	sdisplay_hall.tim_set_output_compare = bsp_tim_set_output_compare;
	sdisplay_hall.tim_clear_flag_irq = bsp_tim_clear_flag_irq;
	sdisplay_hall.tim_start_irq = bsp_tim_start_irq;
	sdisplay_hall.tim_start_output_compare_irq = bsp_tim_start_output_compare_irq;
	sdisplay_hall.tim_start_input_capture_irq = bsp_tim_start_input_capture_irq;

	sys_display_status_t error_handle = SYS_DISPLAY_OK;
	error_handle = (sys_display_status_t)hall_init(sdisplay_hall);
	__ASSERT((error_handle == SYS_DISPLAY_OK), SYS_DISPLAY_ERROR);
	error_handle = (sys_display_status_t)hall_start();
	__ASSERT((error_handle == SYS_DISPLAY_OK), SYS_DISPLAY_ERROR);

	return SYS_DISPLAY_OK;
}
static sys_display_status_t sys_display_ws2812_init()
{
	ws2812_t sdisplay_ws2812;
	sdisplay_ws2812.hspi = SYS_DISPLAY_WS2812_SPI;

	sdisplay_ws2812.spi_transmit = bsp_spi_transmit;
	sdisplay_ws2812.spi_transmit_dma = bsp_spi_transmit_dma;

	ws2812_status_t error_handle = WS2812_OK;
	error_handle = ws2812_init(sdisplay_ws2812);
	__ASSERT((error_handle == WS2812_OK), SYS_DISPLAY_ERROR);

	return SYS_DISPLAY_OK;
}
static void sys_display_loop()
{
	if (sys_display_count_oc == 1)
	{
		sys_display_fan_separate_2_digit(sdisplay_data_time.hours, &hours_unit, &hours_tens);
		sys_display_fan_separate_2_digit(sdisplay_data_time.minutes, &minutes_unit, &minutes_tens);
		sys_display_fan_separate_2_digit(sdisplay_data_time.seconds, &seconds_unit, &seconds_tens);
	}

	sys_display_fan_all_pixel_time_off();
	sys_display_fan_outline();

	if (sdisplay_flag_alarm == true)
		sys_display_fan_alarm();
	else
		sys_display_fan_alarm_off();

	// hours
	if ((sys_display_count_oc >= SYS_DISPLAY_POINT_START) && (sys_display_count_oc <= SYS_DISPLAY_POINT_START + SYS_DISPLAY_WIDTH_CHARACTER))
	{
		uint8_t col = sys_display_count_oc - SYS_DISPLAY_POINT_START;
		sys_display_character(hours_tens, col);
	}
	if ((sys_display_count_oc >= SYS_DISPLAY_POINT_START + 7) && (sys_display_count_oc <= SYS_DISPLAY_POINT_START + 12))
	{
		uint8_t col = sys_display_count_oc - (SYS_DISPLAY_POINT_START + 7);
		sys_display_character(hours_unit, col);
	}

	// minutes
	if (sys_display_count_oc == (SYS_DISPLAY_POINT_START + 15))
		sys_display_fan_colon();
	if ((sys_display_count_oc >= (SYS_DISPLAY_POINT_START + 18)) && (sys_display_count_oc <= SYS_DISPLAY_POINT_START + 23))
	{
		uint8_t col = sys_display_count_oc - (SYS_DISPLAY_POINT_START + 18);
		sys_display_character(minutes_tens, col);
	}
	if ((sys_display_count_oc >= (SYS_DISPLAY_POINT_START + 25)) && (sys_display_count_oc <= (SYS_DISPLAY_POINT_START + 30)))
	{
		uint8_t col = sys_display_count_oc - (SYS_DISPLAY_POINT_START + 25);
		sys_display_character(minutes_unit, col);
	}

	// seconds
	if (sys_display_count_oc == (SYS_DISPLAY_POINT_START + 33))
		sys_display_fan_colon();
	if ((sys_display_count_oc >= (SYS_DISPLAY_POINT_START + 36)) && (sys_display_count_oc <= (SYS_DISPLAY_POINT_START + 41)))
	{
		uint8_t col = sys_display_count_oc - (SYS_DISPLAY_POINT_START + 36);
		sys_display_character(seconds_tens, col);
	}
	if ((sys_display_count_oc >= (SYS_DISPLAY_POINT_START + 43)) && (sys_display_count_oc <= (SYS_DISPLAY_POINT_START + 48)))
	{
		uint8_t col = sys_display_count_oc - (SYS_DISPLAY_POINT_START + 43);
		sys_display_character(seconds_unit, col);
	}
}

static void sys_display_fan_all_pixel_time_off()
{
	for (uint8_t i = SYS_DISPLAY_TIME_LED_NUM_MIN; i <= SYS_DISPLAY_TIME_LED_NUM_MAX; i++)
	{
		ws2812_pixel(i, 0, 0, 0);
	}
}
static void sys_display_fan_pixel_time(sys_display_led_no_t led_no)
{
	ws2812_pixel((uint16_t)led_no, sys_display_color.sys_display_color_time[0],
				 sys_display_color.sys_display_color_time[1],
				 sys_display_color.sys_display_color_time[2]);
}
static void sys_display_fan_pixel_outline(sys_display_led_no_t led_no)
{
	ws2812_pixel((uint16_t)led_no, sys_display_color.sys_display_color_outline[0],
				 sys_display_color.sys_display_color_outline[1],
				 sys_display_color.sys_display_color_outline[2]);
}
static void sys_display_fan_pixel_colon(sys_display_led_no_t led_no)
{
	ws2812_pixel((uint16_t)led_no, sys_display_color.sys_display_color_colon[0],
				 sys_display_color.sys_display_color_colon[1],
				 sys_display_color.sys_display_color_colon[2]);
}
static void sys_display_fan_pixel_alarm(sys_display_led_no_t led_no)
{
	ws2812_pixel((uint16_t)led_no, sys_display_color.sys_display_color_alarm[0],
				 sys_display_color.sys_display_color_alarm[1],
				 sys_display_color.sys_display_color_alarm[2]);
}

static void sys_display_fan_colon()
{
	sys_display_fan_pixel_colon(SYS_DISPLAY_PIXEL_COLON);
	sys_display_fan_pixel_colon(SYS_DISPLAY_PIXEL_COLON + 1);
	sys_display_fan_pixel_colon(SYS_DISPLAY_PIXEL_COLON + 3);
	sys_display_fan_pixel_colon(SYS_DISPLAY_PIXEL_COLON + 4);
}
static void sys_display_fan_outline()
{
	sys_display_fan_pixel_outline(SYS_DISPLAY_PIXEL_OUTLINE);
}

static void sys_display_fan_alarm()
{
	sys_display_fan_pixel_alarm(SYS_DISPLAY_PIXEL_ALARM);
}
static void sys_display_fan_alarm_off()
{
	ws2812_pixel((uint16_t)SYS_DISPLAY_PIXEL_ALARM, 0, 0, 0);
}
void sys_display_character(uint8_t value, uint8_t column)
{
	for (uint8_t i = 0; i < SYS_DISPLAY_CHARACTER_ROW; i++)
	{
		if ((character_number[value][column] & (1 << i)) == (1 << i))
			sys_display_fan_pixel_time(SYS_DISPLAY_TIME_LED_NUM_MIN + i);
	}
}

static void sys_display_fan_separate_2_digit(uint8_t number, uint8_t *unit, uint8_t *tens)
{
	*unit = number % 10;
	*tens = number / 10;
}

/* handle display*/
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3) // check interrupt tim3
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) // check channel 1 interrupt
		{
			uint32_t get_capture_value = TIM3->CCR1;

			sys_display_compare_value = (get_capture_value + (sys_display_interrupt_timer_count_overflow * SYS_DISPLAY_HALL_TIM_COUNTER_MAX)) / SYS_DISPLAY_RESOLUTION;

			TIM3->CCR2 = sys_display_compare_value;
			TIM3->CNT = 0;
			sys_display_count_oc = 0;
			sys_display_interrupt_timer_count_overflow = 0;
			// Read the captured value from the capture/compare register (e.g., CCR1 for channel 1)
			TIM3->SR &= ~TIM_FLAG_CC1;

			//			hall_read_capture(&get_capture_value);
			//
			//			// Divide this value that is got from between 2 times capture for 120
			//			sys_display_compare_value = (get_capture_value + (sys_display_interrupt_timer_count_overflow * SYS_DISPLAY_HALL_TIM_COUNTER_MAX)) / SYS_DISPLAY_RESOLUTION;
			//
			//			// set value output compare
			//			hall_set_output_compare(sys_display_compare_value);
			//
			//			// set value CNT = 0
			//			hall_reset_counter();
			//
			//			// update interrupt_timer_count_overflow value
			//			sys_display_interrupt_timer_count_overflow = 0;
			//
			//			// reset jump OC interrupt sys_display_count_oc counter
			//			sys_display_count_oc = 0;
			//
			//			// reset flag interrupt
			//			hall_reset_flag_capture_interrupt();
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3)
	{
		sys_display_interrupt_timer_count_overflow++;
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3) // check timer 3 interrupt
	{
		if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) // check channel 2 interrupt
		{
			TIM3->CCR2 = sys_display_compare_value + (TIM3->CNT) - 1;
			sys_display_count_oc++;
			sys_display_loop();
			TIM3->SR &= ~TIM_FLAG_CC2;
			/*	uint32_t get_cnt_value = 0;
				hall_get_counter(&get_cnt_value);
				hall_set_output_compare(sys_display_compare_value + (get_cnt_value - 1));

				sys_display_count_oc++;
				// TODO
				sys_display_fan(sdisplay_data_time, sdisplay_flag_alarm);
				hall_reset_flag_compare_interrupt();
				*/
		}
	}
}
/* End of file -------------------------------------------------------- */
