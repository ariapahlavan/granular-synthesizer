//
// Created by Aria Pahlavan on 2/6/18.
//

#include "DisplayDraw.h"
#include "DisplayWrite.h"
#include <stdlib.h>
#include <stdbool.h>

// 180 points on a circle of radius 2.000
const int32_t CircleXbuf[180] = {
        2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798,
        1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176,
        1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70,
        0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939,
        -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576,
        -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941,
        -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970,
        -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658,
        -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060,
        -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140,
        -70, 0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000,
        1060, 1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696,
        1732, 1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995,
        1999
};
const int32_t CircleYbuf[180] = {
        0, 70, 140, 209, 278, 347, 416, 484, 551, 618, 684, 749, 813, 877, 939, 1000, 1060,
        1118, 1176, 1231, 1286, 1338, 1389, 1439, 1486, 1532, 1576, 1618, 1658, 1696, 1732,
        1766, 1798, 1827, 1854, 1879, 1902, 1923, 1941, 1956, 1970, 1981, 1989, 1995, 1999,
        2000, 1999, 1995, 1989, 1981, 1970, 1956, 1941, 1923, 1902, 1879, 1854, 1827, 1798,
        1766, 1732, 1696, 1658, 1618, 1576, 1532, 1486, 1439, 1389, 1338, 1286, 1231, 1176,
        1118, 1060, 1000, 939, 877, 813, 749, 684, 618, 551, 484, 416, 347, 278, 209, 140, 70,
        0, -70, -140, -209, -278, -347, -416, -484, -551, -618, -684, -749, -813, -877, -939,
        -1000, -1060, -1118, -1176, -1231, -1286, -1338, -1389, -1439, -1486, -1532, -1576,
        -1618, -1658, -1696, -1732, -1766, -1798, -1827, -1854, -1879, -1902, -1923, -1941,
        -1956, -1970, -1981, -1989, -1995, -1999, -2000, -1999, -1995, -1989, -1981, -1970,
        -1956, -1941, -1923, -1902, -1879, -1854, -1827, -1798, -1766, -1732, -1696, -1658,
        -1618, -1576, -1532, -1486, -1439, -1389, -1338, -1286, -1231, -1176, -1118, -1060,
        -1000, -939, -877, -813, -749, -684, -618, -551, -484, -416, -347, -278, -209, -140,
        -70
};

void DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
        ST7735_DrawFastHLine(x, y, w, color);
        ST7735_DrawFastHLine(x, y + h - 1, w, color);
        ST7735_DrawFastVLine(x, y, h, color);
        ST7735_DrawFastVLine(x + w - 1, y, h, color);
}

void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
        int16_t startingX = x1 > x2 ? x2 : x1;
        int16_t endingX = x1 == startingX ? x2 : x1;
        int16_t startingY = x1 == startingX ? y1 : y2;
        int16_t endingY = startingY == y1 ? y2 : y1;


        if (x1 == x2) { // vertical line
                startingY = y1 > y2 ? y2 : y1;
                endingY = y1 < y2 ? y2 : y1;
                ST7735_DrawFastVLine(startingX, startingY, endingY - startingY, color);
                return;
        }

        if (y1 == y2) { // horizontal line
                ST7735_DrawFastHLine(startingX, startingY, endingX - startingX, color);
                return;
        }

        int16_t resolution = 160;

        uint16_t xSlope = abs(x1 - x2);
        uint16_t ySlope = abs(y1 - y2);


        bool isDownward = startingY < endingY;

        int16_t curX = startingX * resolution,
                curY = startingY * resolution;

        while (startingX != endingX) {
                startingX = (int16_t) (curX / resolution);
                startingY = (int16_t) (curY / resolution);

                ST7735_DrawPixel(startingX, startingY, color);
                curX += xSlope;
                curY = isDownward ? curY + ySlope : curY - ySlope;
        }
}

void DrawCircle(int16_t radius, int16_t xMin, int16_t yMin, uint16_t color) {
        uint16_t num = 180;
        int32_t minVal = -2500;
        int32_t maxVal = 2500;

        for (uint16_t i = 0; i < num; ++i) {
                uint32_t x = (uint32_t) (xMin + (2 * radius * (CircleXbuf[i] - minVal)) / (maxVal - minVal));
                uint32_t y = (uint32_t) (yMin + (2 * radius * (maxVal - CircleYbuf[i])) / (maxVal - minVal));
                ST7735_DrawPixel(x, y, color);
                ST7735_DrawPixel(x + 1, y + 1, color);
                ST7735_DrawPixel(x + 1, y, color);
                ST7735_DrawPixel(x, y + 1, color);
        }
}

void FillCircle(uint8_t radius, uint8_t xMin, uint8_t yMin, uint16_t color) {
        uint8_t centerX = xMin + radius, centerY = yMin + radius;
        for (int i = 0; i < radius; i += 2) {
                DrawCircle(i, centerX - i, centerY - i, color);
        }
}

void SweepPlot(uint16_t data, uint32_t samplingFreq, uint32_t N) {
        static uint32_t j = 0;
        const uint32_t fs = samplingFreq;

        ST7735_PlotPoint(data);
        if ((j & (N - 1)) == 0) { // fs sampling, fs/N samples plotted per second
                ST7735_PlotNextErase(); // overwrites N points on same line
        }

        if ((j % fs) == 0) { // fs sampling, 1 Hz display of numerical data
                ST7735_SetCursor(0, 0);
                ST7735_sDecOut2(data); // 0 to 4095 (%.2f)

                ST7735_SetCursor(0, 1);
                ST7735_OutUDec(data); // 0 to 4095
        }

        j++; // counts the number of samples
}

void PlotInit(uint16_t min, uint16_t max) {
        ST7735_SetCursor(0, 0);
        ST7735_OutString("My awesome thermometer");
        ST7735_PlotClear(min, max); // range from 0 to 4095
}