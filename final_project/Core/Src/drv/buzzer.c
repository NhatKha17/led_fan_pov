/**
 * @file       buzzer.h
 * @copyright  Copyright (C) 2019 Fiot Co., Ltd. All rights reserved.
 * @license    This project is released under the Fiot License.
 * @version    1.0.0
 * @date       23/03/2023
 * @author     Kha Nguyen
 *
 * @brief      Driver for Buzzer
 *
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "buzzer.h"
#include "music.h"
#include "stdlib.h"
/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
#define BUZZER_OFF (0) // volume = 0
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
static uint16_t buzzer_count_note = 0;
static uint32_t buzzer_count_duration = 0;
/* Private variables -------------------------------------------------- */
static buzzer_t buzzer; //global

/* Private function prototypes ---------------------------------------- */
static void buzzer_start();
static void buzzer_stop();
static void buzzer_tone(uint16_t frequency, uint32_t duration);
static void buzzer_no_tone();
/* Function definitions ----------------------------------------------- */
bool buzzer_init(buzzer_t sys_buzzer)
{
	buzzer = sys_buzzer;
	buzzer_start();
	//		buzzer_no_tone();
	return true;
}

void buzzer_music()
{
	// NOKIA TUNE
	uint32_t divider = 0, note_duration = 0;
	// iterate over the notes of the melody.
	// Remember, the array is twice the number of notes (notes + durations)
	for (uint16_t this_note = 0; this_note < notes_nokia * 2; this_note = this_note + 2)
	{

		// calculates the duration of each note
		divider = melody_nokia[this_note + 1];
		if (divider > 0)
		{
			// regular note, just proceed
			note_duration = (wholenote_nokia) / divider;
		}
		else if (divider < 0)
		{
			// dotted notes are represented with negative durations!!
			note_duration = (wholenote_nokia) / abs(divider);
			note_duration *= 1.5; // increases the duration in half for dotted notes
		}
		// we only play the note for 90% of the duration, leaving 10% as a pause
		buzzer_tone(melody_nokia[this_note], note_duration);

		// Wait for the specief duration before playing the next note.
		if (HAL_GetTick() - buzzer_count_duration >= note_duration)
		{
			// stop the waveform generation before the next note.
			buzzer_no_tone();
			buzzer_count_note += 2;
			buzzer_count_duration = HAL_GetTick();
		}

		// turn off music after whole note is finished
		if (buzzer_count_note >= notes_nokia * 2)
		{
			buzzer_count_note = 0;
			buzzer_no_tone();
		}
	}
	buzzer_no_tone();
}

/* Private function definitions ----------------------------------------------- */
/**
 * @brief  		start pwm channel buzzer
 *
 * @param[in]   None
 *
 * @attention 	Should call this function first before using other function
 *
 * @note		buzzer_stop but using pwm_start because pwm in HIGH buzzer in LOW and opposite
 *
 * @return		None
 */
static void buzzer_start()
{
	
	buzzer.set_prescale(buzzer.htim, buzzer.tim_prescale);
	buzzer.set_autoreload(buzzer.htim, buzzer.tim_autoreload);
	buzzer.pwm_start(buzzer.htim, buzzer.pwn_channel);
}

/**
 * @brief  		start pwm channel buzzer
 *
 * @param[in]   None
 *
 * @attention 	Should call this function after play a song music
 *
 * @note		buzzer_stop but using pwm_start because pwm in LOW buzzer in HIGH and opposite
 *
 * @return		None
 */
static void buzzer_stop()
{
	buzzer.pwm_stop(buzzer.htim, buzzer.pwn_channel);
}
/**
 * @brief  		Stop making tone from frequency
 *
 * @param[in]   frequency   Frequency represent for each note
 *
 * @return		None
 */
static void buzzer_no_tone()
{
	buzzer.pwm_set_compare(buzzer.htim, buzzer.pwn_channel, BUZZER_OFF); // set 0% for duty cycle
}
/**
 * @brief  Make tone from frequency
 *
 * @param[in]    frequency   Frequency represent for each note
 * @param[in]    duration	 duration is calculated in the local file at buzzer_choosemusic
 *
 *
 * @return		None
 */
static void buzzer_tone(uint16_t frequency, uint32_t duration)
{
	if (frequency < 0)
	{
		buzzer_no_tone(); // stop making sound
		buzzer.delay(duration);
	}
	else
	{
		uint32_t autoreload;
		autoreload = 1000000 / frequency; // Calculating period for timer
		buzzer.set_autoreload(buzzer.htim, autoreload);
		buzzer.pwm_set_compare(buzzer.htim, buzzer.pwn_channel, autoreload * buzzer.buzzer_volumn / 100); // set 50% for duty cycle
		buzzer.delay(duration);																			  // waiting until finish playing the note
		buzzer_no_tone();																				  // stop making sound
	}
}

/* End of file -------------------------------------------------------- */
