//
// Created by Aria Pahlavan on 2/4/18.
//
#include "PortUtils.h"
#include <stdbool.h>

PortAttribs RegularDigitalInput(bool isPullup) {
        PortAttribs attribs = {
                .dataDirection = DATA_IN,
                .pullUpEnable = isPullup,
                .pctl = REGULAR_IO,
                .alternateFunctionEnable = false,
                .analogEnable = false,
                .portCommitLock = true
        };

        return attribs;
}

PortAttribs RegularAnalogInput(bool isPullup) {
        PortAttribs attribs = {
                .dataDirection = DATA_IN,
                .pullUpEnable = isPullup,
                .pctl = REGULAR_IO,
                .alternateFunctionEnable = false,
                .analogEnable = true,
                .portCommitLock = true
        };

        return attribs;
}

PortAttribs AdcInput() {
        PortAttribs attribs = {
                .dataDirection = DATA_IN,
                .pullUpEnable = false,
                .pctl = REGULAR_IO,
                .alternateFunctionEnable = true,
                .analogEnable = true,
                .portCommitLock = true
        };

        return attribs;
}

PortAttribs PwmAttribs() {
        PortAttribs attribs = {
                .dataDirection = DATA_IN,
                .pullUpEnable = false,
                .pctl = PWM0,
                .alternateFunctionEnable = true,
                .analogEnable = false,
                .portCommitLock = true
        };

        return attribs;
}

PortAttribs RegularDigitalOutput(bool isPullup) {
        PortAttribs attribs = {
                .dataDirection = DATA_OUT,
                .pullUpEnable = isPullup,
                .pctl = REGULAR_IO,
                .alternateFunctionEnable = false,
                .analogEnable = false,
                .portCommitLock = true
        };

        return attribs;
}

PortAttribs SwitchAttribs(bool isPullup) {
        PortAttribs attribs = {
                .dataDirection = DATA_IN,
                .pullUpEnable = isPullup,
                .pctl = REGULAR_IO,
                .alternateFunctionEnable = false,
                .analogEnable = false,
                .portCommitLock = true
        };

        return attribs;
}

PortAttribs RegularAnalogOutput(bool isPullup) {
        PortAttribs attribs = {
                .dataDirection = DATA_OUT,
                .pullUpEnable = isPullup,
                .pctl = REGULAR_IO,
                .alternateFunctionEnable = false,
                .analogEnable = true,
                .portCommitLock = true
        };

        return attribs;
}

uint32_t pctlOf(uint32_t pinNumber, uint32_t mask)
{
        switch (pinNumber) {
                case PIN0: return pin0Mask(mask);
                case PIN1: return pin1Mask(mask);
                case PIN2: return pin2Mask(mask);
                case PIN3: return pin3Mask(mask);
                case PIN4: return pin4Mask(mask);
                case PIN5: return pin5Mask(mask);
                case PIN6: return pin6Mask(mask);
                case PIN7: return pin7Mask(mask);
                case 0: return 0;
                default: break;
        }
        if (pinNumber <= ALL_PINS) {
                return pctlOf(extractPin0(pinNumber), mask) |
                       pctlOf(extractPin1(pinNumber), mask) |
                       pctlOf(extractPin2(pinNumber), mask) |
                       pctlOf(extractPin3(pinNumber), mask) |
                       pctlOf(extractPin4(pinNumber), mask) |
                       pctlOf(extractPin5(pinNumber), mask) |
                       pctlOf(extractPin6(pinNumber), mask) |
                       pctlOf(extractPin7(pinNumber), mask);
        }
}
