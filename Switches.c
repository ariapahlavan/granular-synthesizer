 //
// Created by Aria Pahlavan on 2/5/18.
//

#include "PortF.h"
#include "Utils.h"
#include "Switches.h"
#include "PortUtils.h"

bool isPf4Initialized = false;
bool isPf0Initialized = false;

#define SW1       0x10
#define SW2       0x01

void Sw1Init() {
    if (!isPf4Initialized) {
        PortFInit(SW1, RegularDigitalInput(true));
        isPf4Initialized = true;
    }
}

void Sw2Init() {
    if (!isPf0Initialized) {
        PortAttribs attribs = RegularDigitalInput(true);
        attribs.portCommitLock = false; //unlock PF0
        PortFInit(SW2, attribs);
        isPf0Initialized = true;
    }
}

void WaitOnSw1(void) {
    Sw1Init();
    while (PF4 == 0x00) { SleepFor(10); }
    while (PF4 == 0x10) { SleepFor(10); }
}

void WaitOnSw1AndClear(void) {
    Sw1Init();
    while (PF4 == 0x00) { SleepFor(10); }
    while (PF4 == 0x10) { SleepFor(10); }

    ST7735_FillScreen(0);
}

void WaitOnSw2(void) {
    Sw2Init();
    while (PF0 == 0x00) { SleepFor(10); }
    while (PF0 == 0x01) { SleepFor(10); }
}

void WaitOnSw2AndClear(void) {
    Sw2Init();
    while (PF0 == 0x00) { SleepFor(10); }
    while (PF0 == 0x01) { SleepFor(10); }

    ST7735_FillScreen(0);
}
