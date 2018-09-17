// KeyboardInterface.c
// Runs on TM4C123
// Keyboard APIs for interfacing switches and piano keys
// Aria Pahlavan, Mehrad Yousefi

#include "ST7735.h"
#include "TimerA.h"
#include "Utils.h"
#include "PortUtils.h"
#include "PortB.h"
#include "PortE.h"
#include "SoundArrays.h"
#include "Dac.h"
#include "Adc.h"
#include "startup.h"
#include "KeyboardInterface.h"
#include "PotInterface.h"
#include "MicInterface.h"

#define ORIG_PITCH MicrosecToCycles(SAMPLE_TIME)


/**
 * @brief handler for switch 1 to play note C
 */
void riseE0(void) {
        PlayPitch(D);
}

/**
 * @brief handler for switch 1 to play original note
 */
void fallE0(void) {
        PlayPitch(ORIG_PITCH);
}

/**
 * @brief handler for switch 2 to play note D
 */
void riseE1(void) {
        PlayPitch(E);
}

/**
 * @brief handler for switch 2 to play original note
 */
void fallE1(void) {
        PlayPitch(ORIG_PITCH);
}

/**
 * @brief handler for switch 3 to play note E
 */
void riseE2(void) {
        PlayPitch(F);
}

/**
 * @brief handler for switch 4 to play original note
 */
void fallE2(void) {
        PlayPitch(ORIG_PITCH);

}

/**
 * @brief handler for switch 4 to play note F
 */
void riseE3(void) {
        PlayPitch(F1);
}

/**
 * @brief handler for switch 4 to play original note
 */
void fallE3(void) {
        PlayPitch(ORIG_PITCH);
}

/**
 * @brief handler for switch 5 to play note G
 */
void riseB0(void) {
        PlayPitch(D2);
}

/**
 * @brief handler for switch 5 to play original note
 */
void fallB0(void) {
        PlayPitch(ORIG_PITCH);
}

/**
 * @brief handler for switch 6 to play note A
 */
void riseB1(void) {
        PlayPitch(E2);
}

/**
 * @brief handler for switch 6 to play original note
 */
void fallB1(void) {
        PlayPitch(ORIG_PITCH);
}

/**
 * @brief handler for switch 7 to play note B
 */
void riseB2(void) {
        PlayPitch(F2);
}

/**
 * @brief handler for switch 7 to play original note
 */
void fallB2(void) {
        PlayPitch(ORIG_PITCH);
}

/**
 * @brief handler for switch 8 to play note C
 */
void riseB3(void) {
        PlayPitch(C1);
}

/**
 * @brief handler for switch 8 to play original note
 */
void fallB3(void) {
        PlayPitch(ORIG_PITCH);
}

uint8_t *voice;
void soundPlayback() {
        static int counter = 0;

        int lowerBound = LowerBound();
        int upperBound = UpperBound();

        counter = (counter + 1) % upperBound;

        if (counter < lowerBound || counter > upperBound)
                counter = lowerBound;

        uint16_t data = (voice[counter] * 4095) / 255;
        SSI1_DR_R = (data + (0xC << 12));
}

void KeyboardInit() {
        voice = GetSound();
        SystickInit(ORIG_PITCH, soundPlayback);
        DacInit(MUTE);

        PortAttribs attribs = SwitchAttribs(false);

        EdgeIntOnPortE(PIN0, attribs, &riseE0, &fallE0);
        EdgeIntOnPortE(PIN1, attribs, &riseE1, &fallE1);
        EdgeIntOnPortE(PIN2, attribs, &riseE2, &fallE2);

        EdgeIntOnPortB(PIN0, attribs, &riseB0, &fallB0);
        EdgeIntOnPortB(PIN1, attribs, &riseB1, &fallB1);
        EdgeIntOnPortB(PIN2, attribs, &riseB2, &fallB2);
}
