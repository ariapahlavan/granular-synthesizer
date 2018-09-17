//
// Created by Aria Pahlavan on 2/4/18.
//

#include "PortB.h"
#include "Logging.h"
#include "tm4c123gh6pm.h"
#include "TimerA.h"
#include "PortE.h"

#define PORTB_PRIORITY 1

static EdgeHandler edgeTasks[7] = {{false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0}};

static uint32_t activatedPins = 0;

uint32_t pctlOfPortB(uint32_t pinNumber, uint32_t mask);

void PortBInit(uint32_t pinNumber, PortAttribs attribs) {
        static bool isActivated = false;

        if (!isActivated) {
                // 1) activate clock for Port B
                SYSCTL_RCGCGPIO_R |= 0x02;
                while ((SYSCTL_PRGPIO_R & 0x02) == 0) {}; // allow time for clock to start
                isActivated = true;
        }

        // 2) no need to unlock
        if (!attribs.portCommitLock) {
                GPIO_PORTB_LOCK_R = GPIO_LOCK_KEY;
        }

        // 2b) enable or disable port commit register
        GPIO_PORTB_CR_R = CalculatedGpioValue(!attribs.portCommitLock, GPIO_PORTB_CR_R, pinNumber);

        // 3) regular or irregular GPIO
        GPIO_PORTB_PCTL_R = CalculatedGpioPctl(pctlOfPortB(pinNumber, attribs.pctl),
                                               GPIO_PORTB_PCTL_R,
                                               pinNumber);
        // 4) enable or disable analog function
        GPIO_PORTB_AMSEL_R = CalculatedGpioValue(attribs.analogEnable,
                                                 GPIO_PORTB_AMSEL_R,
                                                 pinNumber);
        // 5) enable or disable pullup
        GPIO_PORTB_PUR_R = CalculatedGpioValue(attribs.pullUpEnable,
                                               GPIO_PORTB_PUR_R,
                                               pinNumber);
        // 6) set direction to output or input
        GPIO_PORTB_DIR_R = CalculatedGpioValue(attribs.dataDirection == DATA_OUT,
                                               GPIO_PORTB_DIR_R,
                                               pinNumber);
        // 7) regular or alternate port function
        GPIO_PORTB_AFSEL_R = CalculatedGpioValue(attribs.alternateFunctionEnable,
                                                 GPIO_PORTB_AFSEL_R,
                                                 pinNumber);
        // 8) enable or disable digital port
        GPIO_PORTB_DEN_R = CalculatedGpioValue(!attribs.analogEnable,
                                               GPIO_PORTB_DEN_R,
                                               pinNumber);
}

uint32_t pctlOfPortB(uint32_t pinNumber, uint32_t mask) {
        uint32_t MAX_MULTI_PIN = 0xFF;
        if (pinNumber > MAX_MULTI_PIN) {
                ERROR2("Invalid Port B Pin", "Pin #", pinNumber);
        }

        return pctlOf(pinNumber, mask);
}

static uint32_t PinData(uint8_t pinNumber) {
        switch (pinNumber) {
                case PIN0: return PB0;
                case PIN1: return PB1;
                case PIN2: return PB2;
                case PIN3: return PB3;
                case PIN4: return PB4;
                case PIN5: return PB5;
                case PIN6: return PB6;
                default: return 0;
        }
}

void EdgeIntOnPortB(uint8_t pinNumber,
                    PortAttribs attribs,
                    void (*touchtask)(void),
                    void (*releasetask)(void))
{
        activatedPins |= pinNumber;

        PortBInit(pinNumber, attribs);

        edgeTasks[pinNumber].release = false;
        edgeTasks[pinNumber].touch = false;
        edgeTasks[pinNumber].last = PinData(pinNumber);
        edgeTasks[pinNumber].TouchHandler = touchtask;
        edgeTasks[pinNumber].ReleaseHandler = releasetask;

        GPIO_PORTB_IS_R = CalculatedGpioValue(false, GPIO_PORTB_IS_R, pinNumber);
        GPIO_PORTB_IBE_R = CalculatedGpioValue(true, GPIO_PORTB_IBE_R, pinNumber);
        GPIO_PORTB_ICR_R = CalculatedGpioValue(true, GPIO_PORTB_ICR_R, pinNumber);
        GPIO_PORTB_IM_R = CalculatedGpioValue(true, GPIO_PORTB_IM_R, pinNumber);
        SetPriorityOfPortB(PORTB_PRIORITY);
        EnableInterruptOnPortB;

        ActivateTimer3a; // used to debounce switches
}

void RearmPinsForPortB(void) {
        GPIO_PORTB_ICR_R = CalculatedGpioValue(true, GPIO_PORTB_ICR_R, activatedPins);
        GPIO_PORTB_IM_R = CalculatedGpioValue(true, GPIO_PORTB_IM_R, activatedPins);
        SetPriorityOfPortB(PORTB_PRIORITY);
        EnableInterruptOnPortB;
}

static void CheckPinForPortB(uint8_t pin) {
        if ((activatedPins & pin) == 0) return;
        if (PinData(pin) == edgeTasks[pin].last) return;

        if (edgeTasks[pin].last) { // non-zero means it was previously released
                edgeTasks[pin].release = true;
                (*edgeTasks[pin].ReleaseHandler)();
        } else {
                edgeTasks[pin].touch = true;
                (*edgeTasks[pin].TouchHandler)();
        }
}

void CheckPortB(void) {
        CheckPinForPortB(PIN0);
        CheckPinForPortB(PIN1);
        CheckPinForPortB(PIN2);
        CheckPinForPortB(PIN3);
}

void GPIOPortB_Handler(void) {
        GPIO_PORTB_IM_R = CalculatedGpioValue(false, GPIO_PORTB_IM_R, activatedPins); // disarm pins
        GPIO_PORTE_IM_R = CalculatedGpioValue(false, GPIO_PORTE_IM_R, activatedPins); // disarm Port E pins

        CheckPortB();
        CheckPortE(); // check port E pins just in case

        Timer3aInit(MillisecToCycles(10), &DebounceTimerHandler);
}

EdgeHandler *TasksForPortB() { return edgeTasks; }


/**------------------- DEPRECATED -----------------------------*/
void FallingEdgeOnPortB(uint32_t pinNumber, PortAttribs attribs) {
        ERROR("NO FALLING EDGE");
}

void FallingEdgeOnPortBWith(uint32_t pinNumber, PortAttribs attribs, void (*handler)(void)) {
        ERROR("NO FALLING EDGE");
}
