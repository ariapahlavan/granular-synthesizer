// Dac.c
// Runs on TM4C123
// This is the implementation of DAC and music controlling APIs
// Aria Pahlavan, Mehrad Yousefi

#include "SoundArrays.h"
#include "Utils.h"
#include "startup.h"
#include "TimerA.h"
#include "Logging.h"
#include "PortD.h"
#include "Dac.h"
#include "FilterCoeffs.h"

#define waveSize                0x40
#define SetSysTickPriTo(x)      NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x1FFFFFFF) | ((x) << 29);
#define SysTickDisabled         NVIC_ST_CTRL_R = 0
#define SysTickEnabled          NVIC_ST_CTRL_R = 0x0007
#define SysTickReloadTo(x)      NVIC_ST_RELOAD_R = ((x)==0 ? 0 : (x)-1)
#define SysTickClear            NVIC_ST_CURRENT_R = 0

static uint16_t waveIndex = 0;

void (*systickHandler)(void);

void SysTick_Handler(void) {
        systickHandler();
}

void PlayPitch(uint32_t pitch) {
        SysTickDisabled;
        SysTickReloadTo(pitch);
        SysTickEnabled;
}

void Play(uint32_t frequency, void (*handler)(void)) {
        long sr = StartCritical();

        SoundInit(frequency, true, handler); // Init DAC and SysThick

        EndCritical(sr);
}

void Pause() {
        PlayPitch(MUTE);
        SysTickDisabled;
}

void SystickInit(uint32_t pitchPeriod, void (*handler)(void)) {
        systickHandler = handler;
        SysTickDisabled;
        SysTickReloadTo(pitchPeriod);
        SysTickClear;
        SetSysTickPriTo(2);
        SysTickEnabled;
}

void SoundInit(uint32_t pitchPeriod, bool startPlaying, void (*handler)(void)) {
        systickHandler = handler;
        DacInit(pitchPeriod);
        waveIndex = 0;
        SysTickDisabled;
        SysTickReloadTo(pitchPeriod);
        SysTickClear;
        SetSysTickPriTo(2);

        if (startPlaying)
                SysTickEnabled;
}

void DacInit(uint16_t data) {
        uint32_t pinsToActivate = PIN0 | PIN1 | PIN3;
        PortAttribs dacAttribs = {
                .dataDirection = DATA_OUT,
                .pullUpEnable = false,
                .pctl = SSI1,
                .alternateFunctionEnable = true,
                .analogEnable = false,
                .portCommitLock = false
        };

        PortDInit(pinsToActivate, dacAttribs);
        Ssi1Init(data);
}

void InitAndPlayPitchFor(uint32_t period, uint32_t millis, void (*handler)(void)) {
        SoundInit(period, true, handler);
        SleepFor(millis);
        SysTickReloadTo(MUTE);
        SysTickDisabled;
}


//----------------------------- DEPRECATED ---------------------------------
/*


#define melodyPause     DisableTimer0a;
#define harmonyPause    DisableTimer1a;
#define PitchToPlay     ((songBeingPlayed->harmony[harmonyIndex].pitch \
                        + songBeingPlayed->melody[melodyIndex].pitch)/2)

bool isSoundStarted = false;
Song *songBeingPlayed = 0;
int melodyIndex = 0;
int harmonyIndex = 0;
uint32_t melodyDuration = 0;
uint32_t harmonyDuration = 0;

uint32_t envIndex = 0;
uint32_t counter = 0;

bool usingEnvelope = false;
bool harmonyEnabled = false;

void toggleEnvelopEnable() {
        usingEnvelope = !usingEnvelope;
}

void MusicDacHandler(void) {
        if (usingEnvelope) {
                DacOut((ShapeOf(songBeingPlayed->shapes.melodyShape)[waveIndex] * envelope100[envIndex]) / 100);
        } else {
                DacOut(ShapeOf(songBeingPlayed->shapes.melodyShape)[waveIndex]);
        }

        waveIndex = (waveIndex + 1) % (waveSize + 1);

        if (counter++ >= 1000) {
                envIndex++;
                envIndex %= 99;
                counter = 0;
        }
}

*/
/**
 * @brief play the next melody note of the music until
 * the end of the song is reached then pause the music.
 *//*

void melodyCursorHandler(void) {
        if (!isSoundStarted) { return; }

        if (melodyIndex < POKEMON_LENGTH) {
                if (melodyDuration-- > 0) {
                        if (!harmonyEnabled) {
                                PlayPitch(songBeingPlayed->melody[melodyIndex].pitch);
                        } else {
                                PlayPitch(PitchToPlay);
                        }
                } else {
                        melodyIndex++;
                        melodyDuration = songBeingPlayed->melody[melodyIndex].duration;
                        envIndex = 0;
                }
        } else {
                melodyPause;
                envIndex = 0;
        }
}

*/
/**
 * @brief play the next harmony note of the music until
 * the end of the song is reached then pause the music.
 *//*

void harmonyCursorHandler(void) {
        if (harmonyIndex < POKEMON_LENGTH) {
                if (harmonyDuration-- > 0) {
                        PlayPitch(PitchToPlay);
                } else {
                        harmonyIndex++;
                        harmonyDuration = songBeingPlayed->harmony[harmonyIndex].duration;
                }
        } else {
                harmonyPause;
        }
}

void PlayMusic(Song *musicToPlay, uint32_t frequency, void (*handler)(void)) {
        volatile uint32_t delay;
        long sr = StartCritical();

        // **** Initialize DAC, SysThick, and Timers ****
        SoundInitForMusic(MUTE, false, handler);
        Timer0aInit(frequency, melodyCursorHandler);
        melodyDuration = pokemonMusic.melody[0].duration;
        if (harmonyEnabled) {
                Timer2aInit(frequency, harmonyCursorHandler);
                harmonyDuration = pokemonMusic.harmony[0].duration;
        }

        songBeingPlayed = musicToPlay;
        isSoundStarted = true;
        EndCritical(sr);
}

void PauseAndRewindMusic() {
        isSoundStarted = false;

        PlayPitch(MUTE);
        NVIC_ST_CTRL_R = 0;
        melodyPause;
        if (harmonyEnabled) {
                harmonyPause;
        }
        melodyIndex = harmonyIndex = 0;
        melodyDuration = harmonyDuration = 0;
        waveIndex = 0;
}

void PauseMusic() {
        if (!isSoundStarted) {
                return;
        }

        isSoundStarted = false;

        PlayPitch(MUTE);
        NVIC_ST_CTRL_R = 0;
        melodyPause;
        if (harmonyEnabled) {
                harmonyPause;
        }
}

void SoundInitForMusic(uint32_t pitchPeriod, bool startPlaying, void (*handler)(void)) {
        systickHandler = handler;
        DacInitForMusic();
        waveIndex = 0;
        NVIC_ST_CTRL_R = 0;                     // disable SysTick during setup
        NVIC_ST_RELOAD_R = pitchPeriod - 1;     // reload value
        //		NVIC_ST_RELOAD_R = C;
        NVIC_ST_CURRENT_R = 0;                  // any write to current clears it
        SetSysTickPriTo(2);
        if (startPlaying) {
                NVIC_ST_CTRL_R = 0x0007;        // enable SysTick with core clock and interrupts
        }
}

void DacInitForMusic() {
        uint32_t pinsToActivate = PIN0 | PIN1 | PIN3;
        PortAttribs dacAttribs = {
                .dataDirection = DATA_OUT,
                .pullUpEnable = false,
                .pctl = SSI1,
                .alternateFunctionEnable = true,
                .analogEnable = false,
                .portCommitLock = false
        };

        PortDInit(pinsToActivate, dacAttribs);
        Ssi1Init(MUTE);
}*/
