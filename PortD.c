//
// Created by Aria Pahlavan on 2/4/18.
//

#include "PortD.h"

#include "PortUtils.h"
#include "Logging.h"
#include "tm4c123gh6pm.h"
#include <stdbool.h>
#include <stdint.h>

bool isPortDInitialized = false;

uint32_t pctlOfPortD(uint32_t pinNumber, uint32_t mask);

void PortDInit(uint32_t pinNumber, PortAttribs attribs) {
        if (!isPortDInitialized) {
                // 1) activate clock for Port D
                SYSCTL_RCGCGPIO_R |= 0x08;
                while ((SYSCTL_PRGPIO_R & 0x08) == 0) {}; // allow time for clock to start
                isPortDInitialized = true;
        }

        // 2) no need to unlock
        if (!attribs.portCommitLock) {
                GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;
        }

        // 2b) enable or disable port commit register
        GPIO_PORTD_CR_R = CalculatedGpioValue(!attribs.portCommitLock,
                                              GPIO_PORTD_CR_R,
                                              pinNumber);
        // 3) regular or irregular GPIO
        GPIO_PORTD_PCTL_R = CalculatedGpioPctl(pctlOfPortD(pinNumber, attribs.pctl),
                                               GPIO_PORTD_PCTL_R,
                                               pinNumber);
        // 4) enable or disable analog function
        GPIO_PORTD_AMSEL_R = CalculatedGpioValue(attribs.analogEnable,
                                                 GPIO_PORTD_AMSEL_R,
                                                 pinNumber);
        // 5) enable or disable pullup
        GPIO_PORTD_PUR_R = CalculatedGpioValue(attribs.pullUpEnable,
                                               GPIO_PORTD_PUR_R,
                                               pinNumber);
        // 6) set direction to output or input
        GPIO_PORTD_DIR_R = CalculatedGpioValue(attribs.dataDirection == DATA_OUT,
                                               GPIO_PORTD_DIR_R,
                                               pinNumber);
        // 7) regular or alternate port function
        GPIO_PORTD_AFSEL_R = CalculatedGpioValue(attribs.alternateFunctionEnable,
                                                 GPIO_PORTD_AFSEL_R,
                                                 pinNumber);
        // 8) enable or disable digital port
        GPIO_PORTD_DEN_R = CalculatedGpioValue(!attribs.analogEnable,
                                               GPIO_PORTD_DEN_R,
                                               pinNumber);
}

uint32_t pctlOfPortD(uint32_t pinNumber, uint32_t mask)
{
        uint32_t MAX_MULTI_PIN = 0xFF;
        if (pinNumber > MAX_MULTI_PIN) {
                ERROR2("Invalid Port D Pin", "Pin #", pinNumber);
        }

        return pctlOf(pinNumber, mask);
}
