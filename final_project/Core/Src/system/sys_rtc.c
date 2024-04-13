/**
 * @file       sys_rtc.c
 * @copyright
 * @license
 * @version    1.0.0
 * @date       23/03/2024
 * @author     Kha Nguyen
 *
 * @brief      system rtc handle setting , get data time from ds1307
 */

/* Includes ----------------------------------------------------------- */
#include "sys_rtc.h"
#include "stdbool.h"
/* Private defines ---------------------------------------------------- */
/*define buzzer*/
#define SYS_RTC_BUZZER_TIM (&htim4)
#define SYS_RTC_BUZZER_CHANNEL (TIM_CHANNEL_1)
#define SYS_RTC_BUZZER_AUTORELOAD (1000)
#define SYS_RTC_BUZZER_PRESCALE (72)
#define SYS_RTC_BUZZER_VOLUME (90) // from 0 to 100

/*cjmcu75*/
#define SYS_RTC_CJMCU75_I2C (&hi2c2)

/*define ds1307*/
#define SYS_RTC_DS130_I2C (&hi2c1)

extern I2C_HandleTypeDef hi2c2; // cjmcu75
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
extern I2C_HandleTypeDef hi2c1; // ds1307
extern I2C_HandleTypeDef hi2c2; // cjmcu75
extern TIM_HandleTypeDef htim4; // buzzer
/* Private variables -------------------------------------------------- */
static sys_rtc_alarm_time_t sys_rtc_alarm_time; //using save data time of alarm
static bool sys_rtc_flag_alarm = false;	//using save status of alarm
/* Private function prototypes ---------------------------------------- */
static sys_rtc_status_t sys_rtc_check_alarm();
static sys_rtc_status_t sys_rtc_init();
static sys_rtc_status_t sys_buzzer_init();
static sys_rtc_status_t sys_rtc_cjmcu75_init();

/* Function definitions ----------------------------------------------- */
sys_rtc_status_t sys_rtc_start()
{
	sys_rtc_status_t error_handle = SYS_RTC_OK;

	error_handle = sys_buzzer_init();
	__ASSERT((error_handle == SYS_RTC_OK), SYS_RTC_ERROR);

	error_handle = sys_rtc_init();
	__ASSERT((error_handle == SYS_RTC_OK), SYS_RTC_ERROR);

	error_handle = (sys_rtc_status_t)ds1307_set_clock_haft_enable();
	__ASSERT((error_handle == SYS_RTC_OK), SYS_RTC_ERROR);

	error_handle = sys_rtc_cjmcu75_init();
	__ASSERT((error_handle == SYS_RTC_OK), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}

sys_rtc_status_t sys_rtc_get_time(sys_rtc_time_t *get_time)
{
	__ASSERT((get_time != NULL), SYS_RTC_ERROR);

	ds1307_status_t error_handle = DS1307_OK;
	error_handle = ds1307_get_time(get_time);
	__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}

sys_rtc_status_t sys_rtc_set_time(sys_rtc_time_t *set_time)
{
	__ASSERT((set_time != NULL), SYS_RTC_ERROR);

	ds1307_status_t error_handle = DS1307_OK;

	error_handle = ds1307_set_mode_24h();
	__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

	set_time->mode = DS1307_MODE_24H;
	set_time->am_pm = DS1307_AM;

	error_handle = ds1307_set_time(set_time);
	__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}

sys_rtc_status_t sys_rtc_set_epoch_time(uint32_t epoch_time)
{
	ds1307_status_t error_handle = DS1307_OK;

	error_handle = ds1307_set_by_epoch_time(epoch_time);
	__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}

sys_rtc_status_t sys_rtc_set_mode(sys_rtc_mode_time_t mode_time)
{
	switch (mode_time)
	{
	case SYS_RTC_MODE_24H:
	{
		ds1307_status_t error_handle = DS1307_OK;
		error_handle = ds1307_set_mode_24h();
		__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

		return SYS_RTC_OK;
	}
	case SYS_RTC_MODE_12H:
	{
		ds1307_status_t error_handle = DS1307_OK;
		error_handle = ds1307_set_mode_12h();
		__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

		return SYS_RTC_OK;
	}
	default:
		return SYS_RTC_ERROR;
	}
	return SYS_RTC_ERROR;
}

sys_rtc_status_t sys_rtc_set_alarm(sys_rtc_alarm_time_t *alarm_time)
{
	__ASSERT(alarm_time != NULL, SYS_RTC_ERROR);

	if ((alarm_time->hours >= 0 && alarm_time->hours < 24) &&
		(alarm_time->minutes >= 0 && alarm_time->minutes < 60) &&
		(alarm_time->seconds >= 0 && alarm_time->seconds < 60))
	{
		sys_rtc_alarm_time.hours = alarm_time->hours;
		sys_rtc_alarm_time.minutes = alarm_time->minutes;
		sys_rtc_alarm_time.seconds = alarm_time->seconds;
		sys_rtc_flag_alarm = true;

		return SYS_RTC_OK;
	}
	return SYS_RTC_ERROR;
}

sys_rtc_status_t sys_rtc_alarm(bool *flag_alarm)
{
	*flag_alarm = sys_rtc_flag_alarm;
	if (sys_rtc_flag_alarm == true)
	{
		sys_rtc_status_t erro_handle = SYS_RTC_OK;
		erro_handle = sys_rtc_check_alarm(flag_alarm);
		if (erro_handle == SYS_RTC_OK)
		{
			sys_rtc_flag_alarm = false;
			buzzer_music();
			return SYS_RTC_OK;
		}
		else
			return SYS_RTC_ERROR;
	}
	return SYS_RTC_ERROR;
}

sys_rtc_status_t sys_rtc_cancel_alarm()
{
	sys_rtc_flag_alarm = false;
	return SYS_RTC_OK;
}

int8_t sys_rtc_get_cjmcu75()
{
	return cjmcu75_temp();
}
/* Private function definitions --------------------------------------- */
static sys_rtc_status_t sys_rtc_check_alarm(bool *flag_alarm)
{
	sys_rtc_time_t current_time;
	ds1307_status_t error_handle = ds1307_get_time(&current_time);
	__ASSERT(error_handle == DS1307_OK, SYS_RTC_ERROR);

	if (current_time.mode == (ds1307_mode_t)SYS_RTC_MODE_12H)
	{
		if (current_time.am_pm == DS1307_PM)
		{
			current_time.hours += 12;
			current_time.hours %= 24; // Ensure that the number of hours does not exceed 24.
		}
	}
	if (current_time.hours == sys_rtc_alarm_time.hours &&
		current_time.minutes == sys_rtc_alarm_time.minutes &&
		current_time.seconds == sys_rtc_alarm_time.seconds)
	{
		sys_rtc_flag_alarm = false;
		*flag_alarm = sys_rtc_flag_alarm;
		return SYS_RTC_OK;
	}
	return SYS_RTC_ERROR;
}
static sys_rtc_status_t sys_buzzer_init()
{
	buzzer_t srtc_buzzer;

	srtc_buzzer.htim = SYS_RTC_BUZZER_TIM;
	srtc_buzzer.pwn_channel = SYS_RTC_BUZZER_CHANNEL;
	srtc_buzzer.tim_prescale = SYS_RTC_BUZZER_PRESCALE;
	srtc_buzzer.tim_autoreload = SYS_RTC_BUZZER_AUTORELOAD;
	srtc_buzzer.buzzer_volumn = SYS_RTC_BUZZER_VOLUME;
	srtc_buzzer.pwm_start = bsp_pwm_start;
	srtc_buzzer.pwm_stop = bsp_pwm_stop;
	srtc_buzzer.set_autoreload = bsp_tim_set_autoreload;
	srtc_buzzer.set_prescale = bsp_tim_set_prescale;
	srtc_buzzer.pwm_set_compare = bsp_tim_set_output_compare;
	srtc_buzzer.delay = bsp_delay;
	bool error_handle = true;
	
	error_handle = buzzer_init(srtc_buzzer);
	__ASSERT((error_handle == true), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}

static sys_rtc_status_t sys_rtc_cjmcu75_init()
{
	cjmcu75_t srtc_cjmcu75;
	srtc_cjmcu75.hi2c = SYS_RTC_CJMCU75_I2C;
	srtc_cjmcu75.i2c_read_at = bsp_i2c2_read_multi;
	srtc_cjmcu75.i2c_check_device = bsp_i2c2_device_is_ready;

	cjmcu75_status_t error_handle = CJMCU75_OK;
	error_handle = cjmcu75_init(srtc_cjmcu75);
	__ASSERT((error_handle == CJMCU75_OK), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}

static sys_rtc_status_t sys_rtc_init()
{
	ds1307_t srtc_ds1307;
	srtc_ds1307.ds1307_i2c = SYS_RTC_DS130_I2C;

	srtc_ds1307.i2c_device_check = bsp_i2c1_device_is_ready;
	srtc_ds1307.i2c_read = bsp_i2c1_read;
	srtc_ds1307.i2c_read_multi = bsp_i2c1_read_multi;
	srtc_ds1307.i2c_write = bsp_i2c1_write;

	ds1307_status_t error_handle = DS1307_OK;
	error_handle = ds1307_init(srtc_ds1307);
	__ASSERT((error_handle == DS1307_OK), SYS_RTC_ERROR);

	return SYS_RTC_OK;
}
/* End of file -------------------------------------------------------- */
