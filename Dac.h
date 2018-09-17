// Dac.c
// Runs on TM4C123
// This file contains APIs needed for controlling the music,
// as well as outputing DAC voltages
// Aria Pahlavan, Mehrad Yousefi

#ifndef EE445L_DAC_H
#define EE445L_DAC_H

#include "Music.h"
#include "SynchSerialInterface.h"
#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

/** output 12-bit value to the DAC, by writing it to SSI data register */
//#define DacOut(voltageAmpl) \
//        SSI1_DR_R = voltageAmpl; \
//        SSI1_DR_R = (voltageAmpl + (0x8 << 12)) \

/**
 * basic handler for DAC, which only plays
 * the next element of the sine-wave
 */
void BasicDacHandler(void);

/**
 * @brief Initialize 12-bit DAC, called once
 */
void DacInit(uint16_t data);

/**
 * @param sampled value to filter
 * @return low pass filtered voice
 */
uint16_t FilterVoice(uint16_t sampled);

/**
 * @brief Start sound output, and set Systick interrupt period
 *        Called once, with sound initially off
 *
 * @param period interrupt period
 */
void PlayPitch(uint32_t pitch);

/**
 * @brief Initialize Systick periodic interrupts
 *        Called once, with sound initially off
 *
 * @param pitchPeriod interrupt period
 */
void SystickInit(uint32_t pitchPeriod, void (*handler)(void));

/**
 * @brief Initialize Systick periodic interrupts
 *        Called once, with sound initially off
 *
 * @param pitchPeriod interrupt period
 */
void SoundInit(uint32_t pitchPeriod, bool startPlaying, void (*handler)(void));

/**
 * @brief initalize the DAC and play a note for the specified time
 *
 * @param period the note to be played
 * @param millis the amount of time for the note to be played
 */
void InitAndPlayPitchFor(uint32_t period, uint32_t millis, void (*handler)(void));

/**
 * @brief plays {@code frequency}
 *
 * @param frequency frequency for measuring note duration
 * @param handler the function run inside SysTickHandler
 */
void Play(uint32_t frequency, void (*handler)(void));

/**
 * @brief pauses the frequency being played
 */
void Pause(void);

//----------------------------- LAB 5 ---------------------------------

/**
 * @brief toggles the enveloping functionality on or off
 */
void toggleEnvelopEnable();

/**
 * DAC handler for when playing music (LAB 5)
 */
void MusicDacHandler(void);

/**
 * @brief Initialize 12-bit DAC, called once
 */
void DacInitForMusic();

/**
 * @brief Initialize Systick periodic interrupts
 *        Called once, with sound initially off
 *
 * @param pitchPeriod interrupt period
 */
void SoundInitForMusic(uint32_t pitchPeriod, bool startPlaying, void (*handler)(void));

/**
 * @brief plays a music at a certain clock frequency
 *
 * @param musicToPlay the music to be played
 * @param frequency frequency for measuring note duration
 * @param handler the function run inside SysTickHandler
 */
void PlayMusic(Song *musicToPlay, uint32_t frequency, void (*handler)(void));

/**
 * @brief stops the current music being played
 */
void PauseAndRewindMusic(void);

/**
 * @brief pauses the current music being played
 */
void PauseMusic(void);

#endif //EE445L_DAC_H
