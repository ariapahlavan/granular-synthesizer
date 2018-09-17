//
// Created by Aria Pahlavan on 2/4/18.
//

#include "Logging.h"
#include "ST7735.h"
#include "Utils.h"
#include "Switches.h"
#include "startup.h"

#define DELIMETER   3
#define TITLE_INDEX 0
#define FUNC_INDEX  2
#define LINE_INDEX  (FUNC_INDEX + DELIMETER)
#define FILE_INDEX  (LINE_INDEX + DELIMETER)
#define MSG_INDEX   (FILE_INDEX + DELIMETER)
#define HEADING1    "- "
#define HEADING2    ""

void _restore()
{
    ST7735_SetTextColor(YELLOW);
    EnableInterrupts();
}

void _spin(_logType type)
{
    ST7735_SetCursor(0, 14);

    if (type == _DEBUG) {
        ST7735_OutString(">>> Press SW1...");
        WaitOnSw1();
        ST7735_FillScreen(BLACK);
        ST7735_SetCursor(0,0);
    } else {
        ST7735_OutString(">>> Start debugging!");
        while (1) {}
    }

    _restore();
}

void _logFileName(char *fileName)
{
    uint8_t numSkippedLetters = 0;
    if (fileName[0] == '.') numSkippedLetters++;
    if (fileName[1] == '.') numSkippedLetters++;
    if (fileName[2] == '/') numSkippedLetters++;
    if (fileName[2] == '\\') numSkippedLetters++;

    ST7735_OutString(fileName + numSkippedLetters);
}

void _logMetadata(uint32_t lineNum, char *funcName, char *fileName)
{
    ST7735_SetCursor(0,FUNC_INDEX);
    ST7735_OutString(HEADING1);
    ST7735_OutString("function:");
    ST7735_SetCursor(0,FUNC_INDEX+1);
    ST7735_OutString(HEADING2);
    ST7735_OutString(funcName);

    ST7735_SetCursor(0,LINE_INDEX);
    ST7735_OutString(HEADING1);
    ST7735_OutString("line:");
    ST7735_SetCursor(0,LINE_INDEX+1);
    ST7735_OutString(HEADING2);
    ST7735_OutUDec(lineNum);

    ST7735_SetCursor(0,FILE_INDEX);
    ST7735_OutString(HEADING1);
    ST7735_OutString("file:");
    ST7735_SetCursor(0,FILE_INDEX+1);
    ST7735_OutString(HEADING2);
    _logFileName(fileName);
}

void _logging(uint32_t lineNum, char *funcName, char *fileName, char *title, char *message, uint16_t color)
{
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(0,TITLE_INDEX);
    ST7735_SetTextColor(color);
    ST7735_OutString(title);

    _logMetadata(lineNum, funcName, fileName);

    ST7735_SetCursor(0,MSG_INDEX);
    ST7735_OutString(message);
}

void logging(uint32_t lineNum, char *funcName, char *fileName, char *title, char *message, uint16_t color,
             _logType type)
{
    DisableInterrupts();

    _logging(lineNum, funcName, fileName, title, message, color);
    _spin(type);
}

void loggingWithValue(uint32_t lineNum, char *funcName, char *fileName, char *title, char *message,
                      uint32_t value, uint16_t color, _logType type)
{
    DisableInterrupts();
    _logging(lineNum, funcName, fileName, title, message, color);

    uint8_t ROW_LIMIT = 19;
    uint32_t strSize = StringSizeOf(message);
    uint8_t numDigitsOfValue = DigitCountOf(value);
    uint8_t rowIncrease = 0;

    while (strSize > ROW_LIMIT)
    {
        rowIncrease++;
        strSize -= ROW_LIMIT;
    }

    uint8_t colIncrease = 0;
    if(numDigitsOfValue + strSize > ROW_LIMIT){
        rowIncrease++;
    } else {
        colIncrease = strSize;
    }

    ST7735_SetCursor(colIncrease, MSG_INDEX+rowIncrease);
    ST7735_OutUDec(value);

    _spin(type);
}

void loggingMsg(uint32_t lineNum, char *funcName, char *fileName, char *message, uint16_t color, _logType type) {
    DisableInterrupts();
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(0,0);
    ST7735_SetTextColor(color);

    ST7735_SetCursor(0,TITLE_INDEX);
    ST7735_OutString(message);

    _logMetadata(lineNum, funcName, fileName);
    _spin(type);
}

void loggingWihoutTitle(uint32_t lineNum, char *funcName, char *fileName, char *message, uint32_t value, uint16_t color,
                        _logType type)
{
    DisableInterrupts();
    ST7735_InitR(INITR_REDTAB);
    ST7735_FillScreen(ST7735_BLACK);
    ST7735_SetCursor(0,0);
    ST7735_SetTextColor(color);

    uint8_t ROW_LIMIT = 19;
    uint32_t strSize = StringSizeOf(message);
    uint8_t numDigitsOfValue = DigitCountOf(value);
    uint8_t rowIncrease = 0;

    while (strSize > ROW_LIMIT)
    {
        rowIncrease++;
        strSize -= ROW_LIMIT;
    }

    uint8_t colIncrease = 0;
    if(numDigitsOfValue + strSize > ROW_LIMIT){
        rowIncrease++;
    } else {
        colIncrease = strSize;
    }

    ST7735_SetCursor(0,TITLE_INDEX);
    ST7735_OutString(message);

    ST7735_SetCursor(colIncrease, TITLE_INDEX+rowIncrease);
    ST7735_OutUDec(value);

    _logMetadata(lineNum, funcName, fileName);
    _spin(type);
}
