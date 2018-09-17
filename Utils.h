//
// Created by Aria Pahlavan on 1/28/18.
//

#ifndef EE445L_UTILS_H
#define EE445L_UTILS_H
#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>
#include "ST7735.h"
#include "PLL.h"

//assuming only bus freq 80MHz and 50MHz are gonna be used
#define CyclePeriod BusFreq() == Bus80MHz ?  12.5 : 20

#define findCyclesFor(x, nanos) ((uint32_t) (((x) * (nanos)) / (CyclePeriod)))
#define SecToCycles(x)      findCyclesFor(x, 1000000000)
#define MillisecToCycles(x) findCyclesFor(x, 1000000)
#define MicrosecToCycles(x) findCyclesFor(x, 1000)
#define NanosecToCycles(x)  findCyclesFor(x, 1)

#define findTimeFor(x, nanos) ((uint32_t) ((x) * (CyclePeriod/(nanos))))
#define CyclesToSec(x)      findTimeFor(x,1000000000)
#define CyclesToMillisec(x) findTimeFor(x,1000000)
#define CyclesToMicrosec(x) findTimeFor(x,1000)
#define CyclesToNanosec(x)  findTimeFor(x,1)

typedef enum Colors565 {
        BLACK = ST7735_BLACK,
        BLUE = ST7735_BLUE,
        YELLOW = ST7735_YELLOW,
        MAGENTA = ST7735_MAGENTA,
        CYAN = ST7735_CYAN,
        RED = ST7735_RED,
        WHITE = ST7735_WHITE,
        GREEN = ST7735_GREEN,
        ORANGE = ST7735_ORANGE
} Colors565;

#define OneSec          1000
#define TenSec          10000

#define GPIO_LOCK_KEY   0x4C4F434B  // Unlocks the GPIO_CR register


uint32_t StringSizeOf(const char *s);

uint8_t DigitCountOf(uint32_t n);

void SleepFor(uint32_t millis);

void SleepForMicro(uint32_t micro);

void SleepAndClear(uint32_t delay);

#ifdef __cplusplus
}
#endif

#endif //EE445L_UTILS_H
