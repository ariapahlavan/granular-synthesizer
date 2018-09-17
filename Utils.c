//
// Created by Aria Pahlavan on 2/4/18.
//

#include "Utils.h"

uint32_t StringSizeOf(const char *s) {
        uint32_t charCount = 0;
        while (*s != '\0') {
                charCount++;
                s++;
        }

        return charCount;
}

uint8_t DigitCountOf(uint32_t n) {
        uint8_t countDigits = 1;
        for (uint32_t i = 10; i < 1000000000; i *= 10) {
                if (n / i != 0) {
                        countDigits++;
                } else {
                        break;
                }
        }

        return countDigits;
}

void SleepFor(uint32_t millis) {
        uint32_t volatile time;
        while (millis) {
                time = 72724 * 8 / 89;  // 1msec, tuned at 80 MHz
                while (time) {
                        time--;
                }
                millis--;
        }
}

void SleepForMicro(uint32_t micro) {
        uint32_t volatile time;
        while (micro) {
                time = 72724 * 8 / 89000;  // 1msec, tuned at 80 MHz
                while (time) {
                        time--;
                }
                micro--;
        }
}

void SleepAndClear(uint32_t delay) {
        SleepFor(delay);
        ST7735_FillScreen(0);
}
