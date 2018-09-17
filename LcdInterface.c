// Lab11Main.c
// Runs on TM4C123
// APIs for drawing sound visualizer on the LCV
// Aria Pahlavan, Mehrad Yousefi

#include "ST7735.h"
#include "LcdInterface.h"


uint16_t soundBarColor = ST7735_CYAN;

static int8_t visualizer[TOTAL_BAR_COUNT];

void DrawRange(uint8_t min, uint8_t max, uint16_t color) {
        for (uint8_t i = min; i <= max; ++i) {
                uint8_t actual = Actual(visualizer[i]);
                uint8_t mirrored = Mirrored(visualizer[i]);

                uint8_t x = actual < mirrored ? actual : mirrored;
                uint8_t w = (uint8_t) abs(actual - mirrored);
                uint8_t y = (BAR_PX_SIZE+GAP_PX_SIZE) * i;
                ST7735_FillRect(x, y, w, BAR_PX_SIZE, color);
        }
}

void DrawSoundVisualizer(uint8_t *soundArray, uint32_t length) {
        uint32_t sectionSize = length / TOTAL_BAR_COUNT;


        for (uint32_t i = 0; i < TOTAL_BAR_COUNT; ++i) {
                uint32_t sum = 0;
                uint32_t sectionIndex = (i * sectionSize);

                sum += ((soundArray[sectionIndex + 12] * 4095) / 255);


                uint16_t avg = (uint16_t) (sum);                        // [0, 4095]
                uint8_t avgMapped = (uint8_t) ((127 * avg) / 4095);     // -> [0, 127]
                int8_t adjustedAvg = (int8_t) (avgMapped - 64);         // -> [-64, 64]

                visualizer[i] = adjustedAvg;
        }


        soundBarColor = ST7735_Color565(214, 224, 227);
        DrawRange(0, TOTAL_BAR_COUNT+1, soundBarColor);
}
