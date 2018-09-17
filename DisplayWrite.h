//
// Created by Aria Pahlavan on 1/26/18.
//

#ifndef EE445L_DISPLAY_H
#define EE445L_DISPLAY_H

#include "ST7735.h"
#include "Utils.h"
#include <stdint.h>

typedef struct DisplayWrite DisplayWrite;
struct DisplayWrite
{
    uint16_t backgroundColor, textColor;
    uint8_t lineNumber, colomnNumber;
};

void SetLineNumber(DisplayWrite *self, uint8_t newNumber);

void SetColomnNumber(DisplayWrite *self, uint8_t newNumber);

void SetTextColor(DisplayWrite *self, Colors565 newTextColor);

void SetBackgroundColor(DisplayWrite *self, Colors565 newBackgroundColor);

void NewLine(DisplayWrite *self);

void OutString(DisplayWrite *self, char *string);

void OutAt(DisplayWrite *self, char *string, uint8_t lineNum, uint8_t colomnNum);

void OutAtLine(DisplayWrite *self, char *string, uint8_t lineNum);

void OutAtColomn(DisplayWrite *self, char *string, uint8_t colomnNum);

void OutDecimal(DisplayWrite *self, int32_t n);

void OutFixed(DisplayWrite *self, uint32_t dec, uint32_t frac);

void OutRangedUFixed(DisplayWrite *self, uint32_t dec, uint32_t range);

void OutRangedSFixed(DisplayWrite *self, int32_t dec, uint32_t range);

void OutUFixed(DisplayWrite *self, int32_t dec, uint32_t range, uint32_t accuracy);

DisplayWrite CreateDisplayWriter(void);

void InitDisplayWrite(DisplayWrite *self);

void DestroyDisplayWrite(DisplayWrite *self);

void Out(DisplayWrite *self, char *string);

void ResetDisplay(void);

void InitDisplay(void);

/****************ST7735_sDecOut2***************
 @brief converts fixed point number to LCD
 format signed 32-bit with resolution 0.01
 range -99.99 to +99.99

 @param n signed 32-bit integer part of fixed-point number
 */
void ST7735_sDecOut2(int32_t n);

#endif //EE445L_DISPLAY_H
