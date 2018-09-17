//
// Created by Aria Pahlavan on 3/4/18.
//

#ifndef EE445L_FIXEDPOINT_H
#define EE445L_FIXEDPOINT_H

#include <stdint.h>

typedef struct {
    uint32_t value;
    uint16_t res;
} FixedPoint;

FixedPoint mulByFixed(FixedPoint f1, FixedPoint f2);

FixedPoint mulByDecimal(FixedPoint f, uint32_t d);

FixedPoint divFixedByFixed(FixedPoint f1, FixedPoint f2);

FixedPoint divFixedByDecimal(FixedPoint f, uint32_t d, uint16_t dRes);

FixedPoint divDecimalByDecimal(uint32_t d1, uint32_t d2, uint16_t res);

FixedPoint addToFixed(FixedPoint f1, FixedPoint f2);

FixedPoint addToDecimal(FixedPoint f, uint32_t d);

FixedPoint absSubtractFromFixed(FixedPoint f1, FixedPoint f2);

FixedPoint absSubtractFromDecimal(FixedPoint f, uint32_t d);

#endif //EE445L_FIXEDPOINT_H
