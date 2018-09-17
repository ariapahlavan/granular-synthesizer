//
// Created by Aria Pahlavan on 1/26/18.
//

#include "DisplayWrite.h"
#include <stdbool.h>
#include <stdlib.h>

bool isInitialized = false; //note: need to figure out a fix for this

void SetTextColor(DisplayWrite *self, Colors565 newTextColor);

void DestroyDisplayWrite(DisplayWrite *self) {
}

void Out(DisplayWrite *self, char *string) {
        ST7735_DrawString(self->colomnNumber, self->lineNumber, string, self->textColor);
}

uint32_t powersOfTen(int power) {
        uint32_t result = 1;
        for (int i = 0; i < power; ++i) result *= 10;

        return result;
}

bool numValidator(uint32_t n, uint32_t treshold) {
        if (n > treshold) {
                DisplayWrite err = CreateDisplayWriter();
                SetTextColor(&err, RED);
                Out(&err, "[ERROR] Invalid Column Number");
                return false;
        }

        return true;
}

bool isValidColomnNum(uint32_t n) {
        return numValidator(n, 20);
}

bool isValidLineNum(uint32_t n) {
        return numValidator(n, 15);
}

uint8_t digitCountOf(uint32_t n) {
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

void countCharAndSet(DisplayWrite *self, char *string) {
        uint32_t charCount = 0;
        while (*string != '\0') {
                charCount++;
                string++;
        }
        self->colomnNumber += charCount;
}

void countDigitAndSet(DisplayWrite *self, int32_t num) {
        self->colomnNumber += digitCountOf(num);
}

void changeTextColor(DisplayWrite *self) { ST7735_SetTextColor(self->textColor); }

void SetLineNumber(DisplayWrite *self, uint8_t newNumber) {
        if (!isValidLineNum(newNumber))
                return;

        self->lineNumber = newNumber;
}

void SetColomnNumber(DisplayWrite *self, uint8_t newNumber) {
        if (!isValidColomnNum(newNumber))
                return;

        self->colomnNumber = newNumber;
}

void SetTextColor(DisplayWrite *self, Colors565 newTextColor) { self->textColor = newTextColor; }

void
SetBackgroundColor(DisplayWrite *self, Colors565 newBackgroundColor) { self->backgroundColor = newBackgroundColor; }

void NewLine(DisplayWrite *self) { SetLineNumber(self, self->lineNumber + 1); }

void OutAtColomn(DisplayWrite *self, char *string, uint8_t colomnNum) {
        changeTextColor(self);
        ST7735_DrawString(colomnNum, self->lineNumber, string, self->textColor);
}

void OutAtLine(DisplayWrite *self, char *string, uint8_t colomnNum) {
        changeTextColor(self);
        ST7735_DrawString(colomnNum, self->lineNumber, string, self->textColor);
}

void OutAt(DisplayWrite *self, char *string, uint8_t lineNum, uint8_t colomnNum) {
        changeTextColor(self);
        ST7735_DrawString(colomnNum, lineNum, string, self->textColor);
}

void OutString(DisplayWrite *self, char *string) {
        changeTextColor(self);
        ST7735_DrawString(self->colomnNumber, self->lineNumber, string, self->textColor);
        countCharAndSet(self, string);
}

void OutDecimal(DisplayWrite *self, int32_t n) {
        changeTextColor(self);
        ST7735_SetCursor(self->colomnNumber, self->lineNumber);
        ST7735_OutUDec(n);
        countDigitAndSet(self, n);
}

void OutFixed(DisplayWrite *self, uint32_t dec, uint32_t frac) {
        changeTextColor(self);

        uint32_t decDigitCount = digitCountOf(dec);
        uint32_t fracDigitCount = digitCountOf(frac);

        if (!isValidColomnNum(self->colomnNumber + decDigitCount + 1 + fracDigitCount)) /* 1 is for decimal point '.' */
                return;

        ST7735_SetCursor(self->colomnNumber, self->lineNumber);
        ST7735_OutUDec(dec);
        ST7735_OutChar('.');
        ST7735_OutUDec(frac);
        //todo: update cursor
        ST7735_SetCursor(self->colomnNumber, self->lineNumber);
}

bool isRangeValid(uint32_t dec, uint32_t range) {
        uint32_t limit = powersOfTen(range * 2) - 1;

        if (dec > limit) {
                uint32_t n = range;
                char invalidFormatMsg[n * 2 + 2];
                for (uint32_t i = 0; i < n; ++i) invalidFormatMsg[i] = '*';
                invalidFormatMsg[n] = '.';
                for (uint32_t i = n + 1; i < (2 * n + 1); ++i) invalidFormatMsg[i] = '*';
                invalidFormatMsg[n * 2 + 1] = '\0';
                ST7735_OutString((char *) invalidFormatMsg);
                return false;
        }

        return true;
}

bool isFixedValid(uint32_t dec, uint32_t range, uint32_t accuracy) {
        uint32_t limit = powersOfTen(range) - 1;

        if (dec > limit) {
                char invalidFormatMsg[range + accuracy + 2];
                for (uint32_t i = 0; i < range; ++i) invalidFormatMsg[i] = '*';
                invalidFormatMsg[range] = '.';
                for (uint32_t i = range + 1; i < (range + accuracy + 1); ++i) invalidFormatMsg[i] = '*';
                invalidFormatMsg[range + accuracy + 1] = '\0';
                ST7735_OutString((char *) invalidFormatMsg);
                return false;
        }

        return true;
}

void OutRangedUFixed(DisplayWrite *self, uint32_t dec, uint32_t range) {
        changeTextColor(self);

        if (!isRangeValid(dec, range))
                return;

        uint32_t tensPlace = powersOfTen(range);

        // ("%d.%02d", abs(n / 100), abs(n % 100));
        ST7735_OutUDec(dec / tensPlace);
        ST7735_OutChar('.');
        uint32_t frac = dec % tensPlace;

        for (int i = 0; i < (range - digitCountOf(frac)); ++i)
                ST7735_OutChar('0');

        ST7735_OutUDec(frac);
}

void OutRangedSFixed(DisplayWrite *self, int32_t dec, uint32_t range) {
        changeTextColor(self);
        bool isNegative = dec < 0 ? true : false;
        dec = abs(dec);

        if (isNegative) {
                ST7735_OutChar('-');
        } else {
                ST7735_OutChar(' ');
        }

        OutRangedUFixed(self, dec, range);
}

void OutUFixed(DisplayWrite *self, int32_t dec, uint32_t range, uint32_t accuracy) {
        changeTextColor(self);
        bool isNegative = dec < 0 ? true : false;
        dec = abs(dec);

        if (isNegative) {
                ST7735_OutChar('-');
        } else {
                ST7735_OutChar(' ');
        }


        uint32_t tensPlace = powersOfTen(accuracy);

        // ("%d.%02d", abs(n / 100), abs(n % 100));
        uint32_t fixedVal = dec / tensPlace;

        if (!isFixedValid(fixedVal, range, accuracy)) {
                return;
        }

        ST7735_OutUDec(fixedVal);
        ST7735_OutChar('.');
        uint32_t frac = dec % tensPlace;

        for (int i = 0; i < (accuracy - digitCountOf(frac)); ++i)
                ST7735_OutChar('0');

        ST7735_OutUDec(frac);
}

void initDisplay(DisplayWrite *self) {
        if (!isInitialized) {
                ST7735_InitR(INITR_REDTAB);
                ST7735_FillScreen(ST7735_BLACK);
                isInitialized = true;
        }

        self->backgroundColor = BLACK;
        self->lineNumber = 0;
        self->colomnNumber = 0;
}

void InitDisplay() {
        ST7735_InitR(INITR_REDTAB);
        ST7735_FillScreen(ST7735_BLACK);
        ST7735_SetCursor(0, 0);
        isInitialized = true;
}

DisplayWrite CreateDisplayWriter() {
        DisplayWrite display;
        initDisplay(&display);
        display.textColor = YELLOW;

        return display;
}

void ResetDisplay() {
        ST7735_FillScreen(BLACK);
        ST7735_SetCursor(0, 0);
}

void ST7735_sDecOut2(int32_t n) {
        DisplayWrite writer = CreateDisplayWriter();
        OutRangedSFixed(&writer, n, 2);
}

