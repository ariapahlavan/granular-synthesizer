//
// Created by Aria Pahlavan on 3/4/18.
//

#include <stdint.h>
#include "FixedPoint.h"

FixedPoint mulByFixed(FixedPoint f1, FixedPoint f2) {
        FixedPoint result = {
                .value = f1.value * f2.value,
                .res   = f1.res * f2.res
        };

        return result;
}

FixedPoint mulByDecimal(FixedPoint f, uint32_t d) {
        FixedPoint result = {
                .value = d * f.value,
                .res   = f.res
        };

        return result;
}

FixedPoint divFixedByFixed(FixedPoint f1, FixedPoint f2) {

        uint16_t  multRes = f1.res * (f2.res/f1.res);
        uint16_t finalRes = f1.res;

        if (f1.res > f2.res) {
                finalRes = multRes = f1.res * (f1.res/f2.res);
        }

        FixedPoint result = {
                .value = (f1.value * multRes) / f2.value,
                .res   = finalRes
        };

        return result;
}

FixedPoint divFixedByDecimal(FixedPoint f, uint32_t d, uint16_t dRes) {
        FixedPoint dFixedPoint = {
                .value = d,
                .res   = dRes
        };

        return divFixedByFixed(f, dFixedPoint);
}

FixedPoint divDecimalByDecimal(uint32_t d1, uint32_t d2, uint16_t res) {
        FixedPoint d1FixedPoint = {
                .value = d1 * res,
                .res   = res
        };

        FixedPoint d2FixedPoint = {
                .value = d2 * res,
                .res   = res
        };

        return divFixedByFixed(d1FixedPoint, d2FixedPoint);
}

FixedPoint addToFixed(FixedPoint f1, FixedPoint f2) {
        // 3.5 + 54.321
        // = 57.821
        // (35, 10) + (54321, 1000)
        // = 3500 + 54321 = 57821
        FixedPoint *lessPrecise = f1.res > f2.res
                                  ? &f2
                                  : &f1;
        FixedPoint *morePrecise = lessPrecise == &f1
                                  ? &f2
                                  : &f1;

        lessPrecise->value *= (morePrecise->res / lessPrecise->res);
        lessPrecise->res = morePrecise->res;

        FixedPoint result = {
                .value = lessPrecise->value + morePrecise->value,
                .res   = lessPrecise->res
        };

        return result;
}

FixedPoint addToDecimal(FixedPoint f, uint32_t d) {
        // 3 + 54.321
        // = 57.321
        // (3, 1) + (54321, 1000)
        // = 3000 + 54321 = 57321
        FixedPoint dFixedPoint = {
                .value = d * f.res,
                .res = f.res
        };

        FixedPoint result = {
                .value = dFixedPoint.value * f.value,
                .res   = f.res
        };

        return result;
}

FixedPoint absSubtractFromFixed(FixedPoint f1, FixedPoint f2) {
        // 3.5 - 54.321
        // = 50.821
        // (35, 10) - (54321, 1000)
        // = 3500 - 54321 = 50821

        FixedPoint *lessPrecise = f1.res > f2.res
                                  ? &f2
                                  : &f1;
        FixedPoint *morePrecise = lessPrecise == &f1
                                  ? &f2
                                  : &f1;

        lessPrecise->value *= (morePrecise->res / lessPrecise->res);
        uint16_t commonRes = morePrecise->res;
        lessPrecise->res = commonRes;

        FixedPoint *smaller = lessPrecise->value > morePrecise->value
                              ? morePrecise
                              : lessPrecise;
        FixedPoint *larger = smaller == lessPrecise
                             ? morePrecise
                             : lessPrecise;

        FixedPoint result = {
                .value = larger->value - smaller->value,
                .res   = commonRes
        };

        return result;
}

FixedPoint absSubtractFromDecimal(FixedPoint f, uint32_t d) {
        // 3 - 54.321
        // = 51.321
        // (3, 1) - (54321, 1000)
        // = 3000 - 54321 = 51321
        FixedPoint dFixedPoint = {
                .value = d * f.res,
                .res = f.res
        };

        FixedPoint *smaller = f.value > dFixedPoint.value
                              ? &dFixedPoint
                              : &f;
        FixedPoint *larger = smaller == &f
                             ? &dFixedPoint
                             : &f;

        FixedPoint result = {
                .value = larger->value - smaller->value,
                .res   = f.res
        };

        return result;
}
