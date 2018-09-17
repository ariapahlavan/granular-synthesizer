// MicInterface.c
// Runs on TM4C123
// Microphone APIs for interfacing a microphone
// Aria Pahlavan, Mehrad Yousefi

#include "Utils.h"
#include "Adc.h"
#include "DisplayDraw.h"
#include "Dac.h"
#include "Logging.h"
#include "startup.h"
#include "MicInterface.h"
#include "LcdInterface.h"
#include "PotInterface.h"
#include "KeyboardInterface.h"

void DisarmSeq3() {
        TIMER0_CTL_R = 0x00000000;
        ADC0_IM_R &= ~0x08;
        ADC0_ACTSS_R &= ~0x08;
}

volatile uint16_t seq3Value;
uint8_t sampledSound[SOUND_ARRAY_LENGTH];


void RecordInput(uint16_t seq3Value) {
        static uint32_t counter = 0;

        if (counter != SOUND_ARRAY_LENGTH) {
                sampledSound[counter] = (seq3Value * 255) / 4095;
        } else {
                DisarmSeq3();
                ST7735_FillScreen(BLACK);
                DisableInterrupts();
                DrawSoundVisualizer(sampledSound, SOUND_ARRAY_LENGTH);
                PotInit();
                KeyboardInit();

                EnableInterrupts();
        }

        counter++;
}

uint8_t *GetSound(void) { return sampledSound; }

void MicInit(void) {
        Adc0Seq3(9, MicrosecToCycles(SAMPLE_TIME), RecordInput);
}
