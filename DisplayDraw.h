//
// Created by Aria Pahlavan on 2/6/18.
//

#ifndef EE445L_DISPLAYDRAW_H
#define EE445L_DISPLAYDRAW_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "ST7735.h"

/**
 * @brief draw a rectangle
 */
void DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);

/**
 * @brief draw a line
 */
void DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);

/**
 * @brief draw a circle
 * @param xMin top-left x coordinate
 * @param yMin top-left y coordinate
 */
void DrawCircle(int16_t radius, int16_t xMin, int16_t yMin, uint16_t color);

/**
 * @brief draw a solid circle
 * @param xMin top-left x coordinate
 * @param yMin top-left y coordinate
 */
void FillCircle(uint8_t radius, uint8_t xMin, uint8_t yMin, uint16_t color);

/**
 * Plots the next point on the graph in sweep-plot fashion
 * @param data value to be plotted
 */
void SweepPlot(uint16_t data, uint32_t samplingFreq, uint32_t N);

/**
 * Initialize the plot
 * @param min lower bound of plot range
 * @param max upper bound of plot range
 */
void PlotInit(uint16_t min, uint16_t max);



#ifdef __cplusplus
}
#endif
#endif //EE445L_DISPLAYDRAW_H
