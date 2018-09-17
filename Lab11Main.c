// Lab11Main.c
// Runs on TM4C123
// This is the main program for lab 11
// Aria Pahlavan, Mehrad Yousefi

#include "PortF.h"
#include "PLL.h"
#include "Adc.h"
#include "Dac.h"
#include "Utils.h"
#include "SoundArrays.h"
#include "PortB.h"
#include "startup.h"
#include "MicInterface.h"
#include "PotInterface.h"
#include "KeyboardInterface.h"

void RecordSwitchInit() {
        PortBInit(PIN3, RegularDigitalInput(true));
}

void WaitOnRecordSwitch(void) {
        RecordSwitchInit();
        while (PB3 == 0x00) { SleepFor(10); }
        while (PB3 == 0x10) { SleepFor(10); }

        SleepFor(250);
}

void DrawLoading() {

        static uint8_t y = 129;
        if (y < 30) return;
        ST7735_DrawFastHLine(60, y--, 8, GREEN);
}

int main(void) {
        DisableInterrupts();
        PLL_Init(Bus50MHz); /**@NOTE: must equal busFreq in Music.h*/
        LcdInit();
        uint32_t ledPins = PIN1 | PIN2 | PIN3;
        PortFInit(ledPins, RegularDigitalOutput(false));

        WaitOnRecordSwitch();
        ST7735_FillScreen(BLACK);

        MicInit();
        ST7735_FillRect(59, 29, 10, 102, WHITE);
        SystickInit(MicrosecToCycles(9500), DrawLoading);

        EnableInterrupts();



        while (1) {
                WaitForInterrupt();
        }
}
