// Lab11Main.c
// Runs on TM4C123
// APIs for drawing sound visualizer on the LCV
// Aria Pahlavan, Mehrad Yousefi


#ifndef EE445L_LCDINTERFACE_H
#define EE445L_LCDINTERFACE_H

#include <stdint.h>
#include <stdlib.h>

#define MID_LINE_X              (128/2)
#define __mirrorOf(x)             (-1 * (x))

#define Actual(x)               ((x) + MID_LINE_X)
#define Mirrored(x)             (__mirrorOf(x) + MID_LINE_X)

#define TOTAL_BAR_COUNT         40
#define BAR_PX_SIZE             ((160 / TOTAL_BAR_COUNT) -1)
#define GAP_PX_SIZE             1

extern uint16_t soundBarColor;

/**
 * @brief Draw the sounds bars specified in the range
 *
 * @param min min value in the range
 * @param max max  value in the range
 * @param color color to draw the bars with
 */
void DrawRange(uint8_t min, uint8_t max, uint16_t color);

/**
 * @brief Draws the recorded sound as a sound visualizer
 *
 * @param soundArray the array containing the recorded sound value
 * @param length the length of the {@code soundArray}
 */
void DrawSoundVisualizer(uint8_t *soundArray, uint32_t length);

#endif //EE445L_LCDINTERFACE_H
