// PotInterface.c
// Runs on TM4C123
// Potentiometer APIs for interfacing slide and knob pots
// Aria Pahlavan, Mehrad Yousefi

#include "Utils.h"
#include "Adc.h"
#include <stdlib.h>
#include "Logging.h"
#include "startup.h"
#include "PotInterface.h"
#include "LcdInterface.h"

int MIN_INDEX;
int MAX_INDEX;

int prevMin = -1;
int prevMax = -1;

#define ADC_DIFF (4096 / TOTAL_BAR_COUNT)

static uint16_t barColor = RED;
static uint16_t barShade = ORANGE;

uint8_t MapArrIndexToBarRange(int index) {
        // [0, SOUND_ARRAY_LENGTH] -> [0, TOTAL_BARS_COUNT] (i.e. 40)
        return (uint16_t) ((index * TOTAL_BAR_COUNT) / SOUND_ARRAY_LENGTH);
}

void FillBarWith(uint16_t bc, uint16_t sbc, bool isClearing) {
        uint8_t minBar;
        uint8_t maxBar;

        if (isClearing) {
                minBar = MapArrIndexToBarRange(SOUND_ARRAY_LENGTH - prevMin);
                maxBar = MapArrIndexToBarRange(SOUND_ARRAY_LENGTH - prevMax);
        } else {
                minBar = MapArrIndexToBarRange(SOUND_ARRAY_LENGTH - MIN_INDEX);
                maxBar = MapArrIndexToBarRange(SOUND_ARRAY_LENGTH - MAX_INDEX);
        }

        ST7735_FillRect(0, (BAR_PX_SIZE + GAP_PX_SIZE) * minBar, 128, BAR_PX_SIZE, bc);
        ST7735_FillRect(0, (BAR_PX_SIZE + GAP_PX_SIZE) * maxBar, 128, BAR_PX_SIZE, bc);
        DrawRange(maxBar, minBar, sbc);
}

/**
 * @brief Update the sound array bounds
 * based on the new slide position
 *
 * @param position current position of the slide pot [0, 4095]
 */
void SlideListener(uint16_t position) {
        // [0, 4095] -> [0, SOUND_ARRAY_LENGTH - (MAX_INDEX - MIN_INDEX)]
        static uint16_t prev = 0xffff;
        static uint16_t drawingPrev = 0xffff;

        if (prev != 0xffff && abs(position - prev) < 5)
                return;


        uint16_t drawingDiff = (uint16_t) abs(position - drawingPrev);

        if (drawingPrev == 0xffff || drawingDiff > ADC_DIFF)
                FillBarWith(BLACK, soundBarColor, true);


        int windowLength = MAX_INDEX - MIN_INDEX;
        int newLowerBound = ((SOUND_ARRAY_LENGTH - windowLength) * position) / 4095;

        MIN_INDEX = newLowerBound;
        MAX_INDEX = newLowerBound + windowLength;


        if (drawingPrev == 0xffff || drawingDiff  > ADC_DIFF) {
                FillBarWith(barColor, barShade, false);

                prevMin = MIN_INDEX;
                prevMax = MAX_INDEX;
                drawingPrev = position;
        }

        prev = position;
}

/**
 * @brief Update the sound array bounds
 * based on the new knob position
 *
 * @param position current position of the rotary pot [0, 4095]
 */
void KnobListener(uint16_t position) {
        // [0, 4095] -> [0, SOUND_ARRAY_LENGTH]
        static uint16_t prev = 0xffff;
        static uint16_t drawingPrev = 0xffff;

        if (prev != 0xffff && abs(position - prev) < 5)
                return;

        prev = position;

        uint16_t drawingDiff = (uint16_t) abs(position - drawingPrev);

        if (drawingPrev == 0xffff || drawingDiff > ADC_DIFF)
                FillBarWith(BLACK, soundBarColor, true);


int newWindowLength = (SOUND_ARRAY_LENGTH * position) / 4095;
        int curWindowLength = MAX_INDEX - MIN_INDEX;

        int windowLengthDiff = newWindowLength - curWindowLength;
        int diff = windowLengthDiff / 2;

        MAX_INDEX += diff;
        MIN_INDEX -= diff;

        if (MAX_INDEX >= SOUND_ARRAY_LENGTH) {
                MIN_INDEX -= (MAX_INDEX - SOUND_ARRAY_LENGTH);
                MAX_INDEX = SOUND_ARRAY_LENGTH - 1;
        }

        if (MIN_INDEX < 0) {
                MAX_INDEX += (0 - MIN_INDEX);
                MIN_INDEX = 0;
        }

        if (MAX_INDEX >= SOUND_ARRAY_LENGTH) MAX_INDEX = SOUND_ARRAY_LENGTH - 1;

        if (MIN_INDEX < 0) MIN_INDEX = 0;

        if (drawingPrev == 0xffff || drawingDiff  > ADC_DIFF) {
                FillBarWith(barColor, barShade, false);

                prevMin = MIN_INDEX;
                prevMax = MAX_INDEX;
                drawingPrev = position;
        }
}

void PotInit(void) {
        MIN_INDEX = 0;
        MAX_INDEX = SOUND_ARRAY_LENGTH;

        barColor = ST7735_Color565(120, 29, 39);
        barShade = ST7735_Color565(224, 59, 79);

        uint32_t TIME = 100;

        Adc0Seq1(5, MillisecToCycles(TIME), &SlideListener);
        Adc0Seq2(8, MillisecToCycles(TIME), &KnobListener);
}

int LowerBound(void) {
        return MIN_INDEX;
}

int UpperBound(void) {
        return MAX_INDEX;
}

