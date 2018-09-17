// Music.h
// Runs on TM4C123
// This file contains the APIs and structures for representing
// musical notes, durations, as well as a sheet of music
// Aria Pahlavan, Mehrad Yousefi

#ifndef EE445L_MUSIC_H
#define EE445L_MUSIC_H

#include <stdint.h>
#include <shared/PLL.h>

/** instrument wave shapes. */
typedef enum {
    SINE = 0,
    GUITAR = 1,
    OBOE = 2,
    BASSON = 3,
    FLUTE = 4,
    TRUMPET = 5,
} Instrument ;

/**
 * @brief determine the array to be used for representing an instrument
 *
 * @param i the instrument being played
 *
 * @return the array corresponding to instrument i
 */
const unsigned short *ShapeOf(Instrument i);

#define bpm 120
#define busFreq 50000
#define quarterNote (60*busFreq)/bpm

/** duration of a note. */
//typedef enum Duration {
#define SIXTEENTH quarterNote / 4
#define EIGHTH quarterNote / 2
#define QUARTER quarterNote
#define HALF quarterNote * 2
#define WHOLE = quarterNote * 4
//} Duration;

/** note attributes. */
typedef struct Note {
    uint32_t pitch;
    uint32_t duration;
} Note;

typedef struct SongInstrument {
    Instrument melodyShape;
    Instrument harmonyShape;
} SongInstrument;

/** song note sheet. */
typedef struct Song {
    uint32_t tempo; // Speed of music being played (beats/min)
    uint32_t length;
    char songName[19]; // 2 letter less than LCD limits
    SongInstrument shapes;
    const Note *melody;
    const Note *harmony;
} Song;

#endif //EE445L_MUSIC_H
