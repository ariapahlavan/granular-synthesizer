//
// Created by Aria Pahlavan on 2/4/18.
//

#include "PortF.h"
#include "tm4c123gh6pm.h"
#include "PortUtils.h"
#include "ST7735.h"
#include "Logging.h"
#include <stdint.h>
#include <stdbool.h>

static bool isHandlerSpecified = false;

Handler handlers[5] = {{0}, {0}, {0}, {0}, {0}};

uint32_t pctlOfPortF(uint32_t pinNumber, uint32_t mask);

void PortFInit(uint32_t pinNumber, PortAttribs attribs) {
        static bool isActivated = false;

        if (!isActivated) {
                // 1) activate clock for Port F
                SYSCTL_RCGCGPIO_R |= 0x20;
                while ((SYSCTL_PRGPIO_R & 0x20) == 0) {}; // allow time for clock to start
                isActivated = true;
        }

        // 2) no need to unlock
        if (!attribs.portCommitLock) {
                GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
        }

        // 2b) enable or disable port commit register
        GPIO_PORTF_CR_R = CalculatedGpioValue(!attribs.portCommitLock, GPIO_PORTF_CR_R, pinNumber);

        // 3) regular or irregular GPIO
        GPIO_PORTF_PCTL_R = CalculatedGpioPctl(pctlOfPortF(pinNumber, attribs.pctl),
                                               GPIO_PORTF_PCTL_R,
                                               pinNumber);
        // 4) enable or disable analog function
        GPIO_PORTF_AMSEL_R = CalculatedGpioValue(attribs.analogEnable,
                                                 GPIO_PORTF_AMSEL_R,
                                                 pinNumber);
        // 5) enable or disable pullup
        GPIO_PORTF_PUR_R = CalculatedGpioValue(attribs.pullUpEnable,
                                               GPIO_PORTF_PUR_R,
                                               pinNumber);
        // 6) set direction to output or input
        GPIO_PORTF_DIR_R = CalculatedGpioValue(attribs.dataDirection == DATA_OUT,
                                               GPIO_PORTF_DIR_R,
                                               pinNumber);
        // 7) regular or alternate port function
        GPIO_PORTF_AFSEL_R = CalculatedGpioValue(attribs.alternateFunctionEnable,
                                                 GPIO_PORTF_AFSEL_R,
                                                 pinNumber);
        // 8) enable or disable digital port
        GPIO_PORTF_DEN_R = CalculatedGpioValue(!attribs.analogEnable,
                                               GPIO_PORTF_DEN_R,
                                               pinNumber);
}

uint32_t pctlOfPortF(uint32_t pinNumber, uint32_t mask) {
        int MAX_MULTI_PIN = 0x1F;
        if (pinNumber > MAX_MULTI_PIN) {
                ERROR2("Invalid Port F Pin", "Pin to activate: ", pinNumber);
        }

        return pctlOf(pinNumber, mask);
}

void FallingEdgeOnPortF(uint32_t pinNumber, PortAttribs attribs)
{
        isHandlerSpecified = false;

        PortFInit(pinNumber, attribs);
        GPIO_PORTF_IS_R = CalculatedGpioValue(false, GPIO_PORTF_IS_R, pinNumber);
        GPIO_PORTF_IBE_R = CalculatedGpioValue(false, GPIO_PORTF_IBE_R, pinNumber);
        GPIO_PORTF_IEV_R = CalculatedGpioValue(false, GPIO_PORTF_IEV_R, pinNumber);
        GPIO_PORTF_ICR_R = CalculatedGpioValue(true, GPIO_PORTF_ICR_R, pinNumber);
        GPIO_PORTF_IM_R = CalculatedGpioValue(true, GPIO_PORTF_IM_R, pinNumber);
        SetPriorityOfPortF(2);
        EnableInterruptOnPortF;
}

void FallingEdgeOnPortFWith(uint32_t pinNumber,
                            PortAttribs attribs,
                            void (*handler)(void))
{
        isHandlerSpecified = true;
        handlers[pinNumber].func = handler;

        PortFInit(pinNumber, attribs);
        GPIO_PORTF_IS_R = CalculatedGpioValue(false, GPIO_PORTF_IS_R, pinNumber);
        GPIO_PORTF_IBE_R = CalculatedGpioValue(false, GPIO_PORTF_IBE_R, pinNumber);
        GPIO_PORTF_IEV_R = CalculatedGpioValue(false, GPIO_PORTF_IEV_R, pinNumber);
        GPIO_PORTF_ICR_R = CalculatedGpioValue(true, GPIO_PORTF_ICR_R, pinNumber);
        GPIO_PORTF_IM_R = CalculatedGpioValue(true, GPIO_PORTF_IM_R, pinNumber);
        SetPriorityOfPortF(2);
        EnableInterruptOnPortF;
}

void GPIOPortF_Handler(void) {
        uint32_t pinsForSwitches = PIN0 | PIN1 | PIN2 | PIN3 | PIN4;

        uint32_t triggeredButtons = GPIO_PORTF_RIS_R & pinsForSwitches;
        if (isHandlerSpecified) {
                (*(handlers[triggeredButtons].func))();
        } else {
                switch (triggeredButtons) {
  //                      case PIN0 PIN4:break;
                        default:ERROR1("Invalid Handler", triggeredButtons);
                                break;
                }
        }

        GPIO_PORTF_ICR_R = CalculatedGpioValue(true, GPIO_PORTF_ICR_R, pinsForSwitches);
}

