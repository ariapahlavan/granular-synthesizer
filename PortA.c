//
// Created by Aria Pahlavan on 2/4/18.
//

#include "PortA.h"
#include "Logging.h"
#include "inc/tm4c123gh6pm.h"

bool isPortAInitialized = false;

uint32_t pctlOfPortA(uint32_t pinNumber, uint32_t mask);

void PortAInit(uint32_t pinNumber, PortAttribs attribs) {
        volatile uint32_t delay;
        if (!isPortAInitialized) {
                // 1) activate clock for Port A
                SYSCTL_RCGCGPIO_R |= 0x01;
                delay = SYSCTL_PRGPIO_R;
                isPortAInitialized = true;
        }

        // 2) no need to unlock
        if (!attribs.portCommitLock) {
                GPIO_PORTA_LOCK_R = GPIO_LOCK_KEY;
        }

        // 2b) enable or disable port commit register
        GPIO_PORTA_CR_R = CalculatedGpioValue(!attribs.portCommitLock, GPIO_PORTA_CR_R, pinNumber);

        // 3) regular or irregular GPIO
        GPIO_PORTA_PCTL_R = CalculatedGpioPctl(pctlOfPortA(pinNumber, attribs.pctl),
                                               GPIO_PORTA_PCTL_R,
                                               pinNumber);
        // 4) enable or disable analog function
        GPIO_PORTA_AMSEL_R = CalculatedGpioValue(attribs.analogEnable,
                                                 GPIO_PORTA_AMSEL_R,
                                                 pinNumber);
        // 5) enable or disable pullup
        GPIO_PORTA_PUR_R = CalculatedGpioValue(attribs.pullUpEnable,
                                               GPIO_PORTA_PUR_R,
                                               pinNumber);
        // 6) set direction to output or input
        GPIO_PORTA_DIR_R = CalculatedGpioValue(attribs.dataDirection == DATA_OUT,
                                               GPIO_PORTA_DIR_R,
                                               pinNumber);
        // 7) regular or alternate port function
        GPIO_PORTA_AFSEL_R = CalculatedGpioValue(attribs.alternateFunctionEnable,
                                                 GPIO_PORTA_AFSEL_R,
                                                 pinNumber);
        // 8) enable or disable digital port
        GPIO_PORTA_DEN_R = CalculatedGpioValue(!attribs.analogEnable,
                                               GPIO_PORTA_DEN_R,
                                               pinNumber);
}

uint32_t pctlOfPortA(uint32_t pinNumber, uint32_t mask) {

        uint32_t MAX_MULTI_PIN = 0xFF;
        if (pinNumber > MAX_MULTI_PIN) {
                ERROR2("Invalid Port A Pin", "Pin #", pinNumber);
        }
        return pctlOf(pinNumber, mask);
}
