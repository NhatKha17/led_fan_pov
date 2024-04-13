/**
 * @file		music.h
 * @copyright 	Copyright (C) 2019 Fiot Co., Ltd. All rights reserved.
 * @license    	This project is released under the Fiot License.
 * @version    0.0.1
 * @date       24/03/2024
 * @author     Khoi Ly
 *
 * @brief      music for buzzer
 *
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __TABLE_MUSIC_H
#define __TABLE_MUSIC_H
/* Public defines ----------------------------------------------------- */
//#define BUZZER_DMA
// define frequency of note music
#define NOTE_B0        31
#define NOTE_C1        33
#define NOTE_CS1       35
#define NOTE_D1        37
#define NOTE_DS1       39
#define NOTE_E1        41
#define NOTE_F1        44
#define NOTE_FS1       46
#define NOTE_G1        49
#define NOTE_GS1       52
#define NOTE_A1        55
#define NOTE_AS1       58
#define NOTE_B1        62
#define NOTE_C2        65
#define NOTE_CS2       69
#define NOTE_D2        73
#define NOTE_DS2       78
#define NOTE_E2        82
#define NOTE_F2        87
#define NOTE_FS2       93
#define NOTE_G2        98
#define NOTE_GS2       104
#define NOTE_A2        110
#define NOTE_AS2       117
#define NOTE_B2        123
#define NOTE_C3        131
#define NOTE_CS3       139
#define NOTE_D3        147
#define NOTE_DS3       156
#define NOTE_E3        165
#define NOTE_F3        175
#define NOTE_FS3       185
#define NOTE_G3        196
#define NOTE_GS3       208
#define NOTE_A3        220
#define NOTE_AS3       233
#define NOTE_B3        247
#define NOTE_C4        262
#define NOTE_CS4       277
#define NOTE_D4        294
#define NOTE_DS4       311
#define NOTE_E4        330
#define NOTE_F4        349
#define NOTE_FS4       370
#define NOTE_G4        392
#define NOTE_GS4       415
#define NOTE_A4        440
#define NOTE_AS4       466
#define NOTE_B4        494
#define NOTE_C5        523
#define NOTE_CS5       554
#define NOTE_D5        587
#define NOTE_DS5       622
#define NOTE_E5        659
#define NOTE_F5        698
#define NOTE_FS5       740
#define NOTE_G5        784
#define NOTE_GS5       831
#define NOTE_A5        880
#define NOTE_AS5       932
#define NOTE_B5        988
#define NOTE_C6        1047
#define NOTE_CS6       1109
#define NOTE_D6        1175
#define NOTE_DS6       1245
#define NOTE_E6        1319
#define NOTE_F6        1397
#define NOTE_FS6       1480
#define NOTE_G6        1568
#define NOTE_GS6       1661
#define NOTE_A6        1760
#define NOTE_AS6       1865
#define NOTE_B6        1976
#define NOTE_C7        2093
#define NOTE_CS7       2217
#define NOTE_D7        2349
#define NOTE_DS7       2489
#define NOTE_E7        2637
#define NOTE_F7        2794
#define NOTE_FS7       2960
#define NOTE_G7        3136
#define NOTE_GS7       3322
#define NOTE_A7        3520
#define NOTE_AS7       3729
#define NOTE_B7        3951
#define NOTE_C8        4186
#define NOTE_CS8       4435
#define NOTE_D8        4699
#define NOTE_DS8       4978
#define REST           0


#define TEMPO_NOKIA				(180)
//#define TEMPO_HARRY_POTTER	(144)
//#define TEMPO_IMPERIAL_MARCH	(140)
//#define TEMPO_THE_GOD_FATHER	(200)

/* Public enumerate/structure ----------------------------------------- */

/* Public variables --------------------------------------------------- */

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!

// NokiaRingtone

int melody_nokia[] = {
		  NOTE_E5, 8, NOTE_D5, 8, NOTE_FS4, 4, NOTE_GS4, 4,
		  NOTE_CS5, 8, NOTE_B4, 8, NOTE_D4, 4, NOTE_E4, 4,
		  NOTE_B4, 8, NOTE_A4, 8, NOTE_CS4, 4, NOTE_E4, 4,
		  NOTE_A4, 2,
		};
//// Harry Potter
//int melody_harrypotter[] = {
//		REST, 2, NOTE_D4, 4,
//		  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
//		  NOTE_G4, 2, NOTE_D5, 4,
//		  NOTE_C5, -2,
//		  NOTE_A4, -2,
//		  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
//		  NOTE_F4, 2, NOTE_GS4, 4,
//		  NOTE_D4, -1,
//		  NOTE_D4, 4,
//
//		  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4, //10
//		  NOTE_G4, 2, NOTE_D5, 4,
//		  NOTE_F5, 2, NOTE_E5, 4,
//		  NOTE_DS5, 2, NOTE_B4, 4,
//		  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
//		  NOTE_CS4, 2, NOTE_B4, 4,
//		  NOTE_G4, -1,
//		  NOTE_AS4, 4,
//
//		  NOTE_D5, 2, NOTE_AS4, 4,//18
//		  NOTE_D5, 2, NOTE_AS4, 4,
//		  NOTE_DS5, 2, NOTE_D5, 4,
//		  NOTE_CS5, 2, NOTE_A4, 4,
//		  NOTE_AS4, -4, NOTE_D5, 8, NOTE_CS5, 4,
//		  NOTE_CS4, 2, NOTE_D4, 4,
//		  NOTE_D5, -1,
//		  REST,4, NOTE_AS4,4,
//
//		  NOTE_D5, 2, NOTE_AS4, 4,//26
//		  NOTE_D5, 2, NOTE_AS4, 4,
//		  NOTE_F5, 2, NOTE_E5, 4,
//		  NOTE_DS5, 2, NOTE_B4, 4,
//		  NOTE_DS5, -4, NOTE_D5, 8, NOTE_CS5, 4,
//		  NOTE_CS4, 2, NOTE_AS4, 4,
//		  NOTE_G4, -1,
//};
//
//// Imperial March
//int melody_imperialmarch[] = {
//		NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
//		  NOTE_A4,-4, NOTE_A4,-4, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_A4,16, NOTE_F4,8, REST,8,
//		  NOTE_A4,4, NOTE_A4,4, NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16,
//
//		  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,//4
//		  NOTE_E5,4, NOTE_E5,4, NOTE_E5,4, NOTE_F5,-8, NOTE_C5,16,
//		  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
//
//		  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7
//		  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,
//
//		  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
//		  NOTE_C5,4, NOTE_A4,-8, NOTE_C5,16, NOTE_E5,2,
//
//		  NOTE_A5,4, NOTE_A4,-8, NOTE_A4,16, NOTE_A5,4, NOTE_GS5,-8, NOTE_G5,16, //7
//		  NOTE_DS5,16, NOTE_D5,16, NOTE_DS5,8, REST,8, NOTE_A4,8, NOTE_DS5,4, NOTE_D5,-8, NOTE_CS5,16,
//
//		  NOTE_C5,16, NOTE_B4,16, NOTE_C5,16, REST,8, NOTE_F4,8, NOTE_GS4,4, NOTE_F4,-8, NOTE_A4,-16,//9
//		  NOTE_A4,4, NOTE_F4,-8, NOTE_C5,16, NOTE_A4,2,
//
//    };
//
//// The God Father
//int melody_thegodfather[] = {
//
//		REST, 4, REST, 8, REST, 8, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //1
//		  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8,
//		  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
//		  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,
//
//		  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
//		  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
//		  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8,
//		  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,
//
//		  NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
//		  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8,
//		  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
//		  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8,
//
//		  NOTE_E4, 2, REST, 8, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8, //13
//
//		  //repeats from 2
//		  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8, NOTE_G4, 8, //2
//		  NOTE_E4, 2, NOTE_E4, 8, NOTE_A4, 8, NOTE_C5, 8,
//		  NOTE_B4, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_C5, 8, NOTE_A4, 8, NOTE_E4, 8, NOTE_DS4, 8,
//
//		  NOTE_D4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8, //5
//		  NOTE_B4, 2, NOTE_D4, 8, NOTE_F4, 8, NOTE_GS4, 8,
//		  NOTE_A4, 2, NOTE_C4, 8, NOTE_C4, 8, NOTE_G4, 8,
//		  NOTE_F4, 8, NOTE_E4, 8, NOTE_G4, 8, NOTE_F4, 8, NOTE_F4, 8, NOTE_E4, 8, NOTE_E4, 8, NOTE_GS4, 8,
//
//		  NOTE_A4, 2, REST,8, NOTE_A4, 8, NOTE_A4, 8, NOTE_GS4, 8, //9
//		  NOTE_G4, 2, NOTE_B4, 8, NOTE_A4, 8, NOTE_F4, 8,
//		  NOTE_E4, 2, NOTE_E4, 8, NOTE_G4, 8, NOTE_E4, 8,
//		  NOTE_D4, 2, NOTE_D4, 8, NOTE_D4, 8, NOTE_F4, 8, NOTE_DS4, 8,
//
//		  NOTE_E4, 2 //13
//
//};
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes

int notes_nokia = sizeof(melody_nokia) / sizeof(melody_nokia[0]) / 2;
//int notes_harrypotter = sizeof(melody_harrypotter) / sizeof(melody_harrypotter[0]) / 2;
//int notes_imperialmarch = sizeof(melody_imperialmarch) / sizeof(melody_imperialmarch[0]) / 2;
//int notes_thegodfather = sizeof(melody_thegodfather) / sizeof(melody_thegodfather[0]) / 2;
// this calculates the duration of a whole note in ms
int wholenote_nokia = (60000 * 4) / TEMPO_NOKIA;
//int wholenote_harrypotter = (60000 * 4) / TEMPO_HARRY_POTTER;
//int wholenote_imperialmarch = (60000 * 4) / TEMPO_IMPERIAL_MARCH;
//int wholenote_thegodfather = (60000 * 4) / TEMPO_THE_GOD_FATHER;

#endif /* __TABLE_MUSIC_H */
