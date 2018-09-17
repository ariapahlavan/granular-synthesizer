//
// Created by Aria Pahlavan on 2/4/18.
//

#include "PortC.h"
#include "PortUtils.h"
#include "Logging.h"
#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>

bool isPortCInitialized = false;

uint32_t pctlOfPortC(uint32_t pinNumber, uint32_t mask);

void PortCInit(uint32_t pinNumber, PortAttribs attribs) {
        if (!isPortCInitialized) {
                // 1) activate clock for Port C
                SYSCTL_RCGCGPIO_R |= 0x04;
                while ((SYSCTL_PRGPIO_R & 0x04) == 0) {}; // allow time for clock to start
                isPortCInitialized = true;
        }

        // 2) no need to unlock
        if (!attribs.portCommitLock) {
                GPIO_PORTC_LOCK_R = GPIO_LOCK_KEY;
        }

        // 2b) enable or disable port commit register
        GPIO_PORTC_CR_R = CalculatedGpioValue(!attribs.portCommitLock, GPIO_PORTC_CR_R, pinNumber);

        // 3) regular or irregular GPIO
        GPIO_PORTC_PCTL_R = CalculatedGpioPctl(pctlOfPortC(pinNumber, attribs.pctl),
                                               GPIO_PORTC_PCTL_R,
                                               pinNumber);
        // 4) enable or disable analog function
        GPIO_PORTC_AMSEL_R = CalculatedGpioValue(attribs.analogEnable,
                                                 GPIO_PORTC_AMSEL_R,
                                                 pinNumber);
        // 5) enable or disable pullup
        GPIO_PORTC_PUR_R = CalculatedGpioValue(attribs.pullUpEnable,
                                               GPIO_PORTC_PUR_R,
                                               pinNumber);
        // 6) set direction to output or input
        GPIO_PORTC_DIR_R = CalculatedGpioValue(attribs.dataDirection == DATA_OUT,
                                               GPIO_PORTC_DIR_R,
                                               pinNumber);
        // 7) regular or alternate port function
        GPIO_PORTC_AFSEL_R = CalculatedGpioValue(attribs.alternateFunctionEnable,
                                                 GPIO_PORTC_AFSEL_R,
                                                 pinNumber);
        // 8) enable or disable digital port
        GPIO_PORTC_DEN_R = CalculatedGpioValue(!attribs.analogEnable,
                                               GPIO_PORTC_DEN_R,
                                               pinNumber);
}

uint32_t pctlOfPortC(uint32_t pinNumber, uint32_t mask) {
        uint32_t MAX_MULTI_PIN = 0xFF;
        if (pinNumber > MAX_MULTI_PIN) {
                ERROR2("Invalid Port C Pin", "Pin #", pinNumber);
        }

        return pctlOf(pinNumber, mask);
}
