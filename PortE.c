//
// Created by Aria Pahlavan on 2/4/18.
//

#include "PortE.h"
#include "Logging.h"
#include "tm4c123gh6pm.h"
#include "SoundArrays.h"
#include "startup.h"
#include "TimerA.h"
#include "PortB.h"

#define PORTE_PRIORITY 1

static EdgeHandler edgeTasks[6] = {{false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0},
                                   {false, false, 0, 0, 0}};

static uint32_t activatedPins = 0;

uint32_t pctlOfPortE(uint32_t pinNumber, uint32_t mask);

void PortEInit(uint32_t pinNumber, PortAttribs attribs) {
        static bool isActivated = false;

        if (!isActivated) {
                // 1) activate clock for Port E
                SYSCTL_RCGCGPIO_R |= 0x10;
                while ((SYSCTL_PRGPIO_R & 0x10) == 0) {}; // allow time for clock to start
                isActivated = true;
        }

        // 2) no need to unlock
        if (!attribs.portCommitLock) {
                GPIO_PORTE_LOCK_R = GPIO_LOCK_KEY;
        }

        // 2b) enable or disable port commit register
        GPIO_PORTE_CR_R = CalculatedGpioValue(!attribs.portCommitLock, GPIO_PORTE_CR_R, pinNumber);

        // 3) regular or irregular GPIO
        GPIO_PORTE_PCTL_R = CalculatedGpioPctl(pctlOfPortE(pinNumber, attribs.pctl),
                                               GPIO_PORTE_PCTL_R,
                                               pinNumber);
        // 4) enable or disable analog function
        GPIO_PORTE_AMSEL_R = CalculatedGpioValue(attribs.analogEnable,
                                                 GPIO_PORTE_AMSEL_R,
                                                 pinNumber);
        // 5) enable or disable pullup
        GPIO_PORTE_PUR_R = CalculatedGpioValue(attribs.pullUpEnable,
                                               GPIO_PORTE_PUR_R,
                                               pinNumber);
        // 6) set direction to output or input
        GPIO_PORTE_DIR_R = CalculatedGpioValue(attribs.dataDirection == DATA_OUT,
                                               GPIO_PORTE_DIR_R,
                                               pinNumber);
        // 7) regular or alternate port function
        GPIO_PORTE_AFSEL_R = CalculatedGpioValue(attribs.alternateFunctionEnable,
                                                 GPIO_PORTE_AFSEL_R,
                                                 pinNumber);
        // 8) enable or disable digital port
        GPIO_PORTE_DEN_R = CalculatedGpioValue(!attribs.analogEnable,
                                               GPIO_PORTE_DEN_R,
                                               pinNumber);
}

uint32_t pctlOfPortE(uint32_t pinNumber, uint32_t mask) {
        uint32_t MAX_MULTI_PIN = 0x3F;
        if (pinNumber > MAX_MULTI_PIN) {
                ERROR2("Invalid Port E Pin", "Pin #", pinNumber);
        }

        return pctlOf(pinNumber, mask);
}

static uint32_t PinData(uint8_t pinNumber) {
        switch (pinNumber) {
                case PIN0: return PE0;
                case PIN1: return PE1;
                case PIN2: return PE2;
                case PIN3: return PE3;
                case PIN4: return PE4;
                case PIN5: return PE5;
                default: return 0;
        }
}

void EdgeIntOnPortE(uint8_t pinNumber,
                    PortAttribs attribs,
                    void (*touchtask)(void),
                    void (*releasetask)(void)) {
        activatedPins |= pinNumber;

        PortEInit(pinNumber, attribs);

        edgeTasks[pinNumber].release = false;
        edgeTasks[pinNumber].touch = false;
        edgeTasks[pinNumber].last = PinData(pinNumber);
        edgeTasks[pinNumber].TouchHandler = touchtask;
        edgeTasks[pinNumber].ReleaseHandler = releasetask;

        GPIO_PORTE_IS_R = CalculatedGpioValue(false, GPIO_PORTE_IS_R, pinNumber);
        GPIO_PORTE_IBE_R = CalculatedGpioValue(true, GPIO_PORTE_IBE_R, pinNumber);
        GPIO_PORTE_ICR_R = CalculatedGpioValue(true, GPIO_PORTE_ICR_R, pinNumber);
        GPIO_PORTE_IM_R = CalculatedGpioValue(true, GPIO_PORTE_IM_R, pinNumber);
        SetPriorityOfPortE(PORTE_PRIORITY);
        EnableInterruptOnPortE;

        ActivateTimer3a; // used to debounce switches
}

void RearmPinsForPortE(void) {
        GPIO_PORTE_ICR_R = CalculatedGpioValue(true, GPIO_PORTE_ICR_R, activatedPins);
        GPIO_PORTE_IM_R = CalculatedGpioValue(true, GPIO_PORTE_IM_R, activatedPins);
        SetPriorityOfPortE(PORTE_PRIORITY);
        EnableInterruptOnPortE;
}

void DebounceTimerHandler(void) {
        DisarmTimer3a;

        EdgeHandler *portBTasks = TasksForPortB();
        EdgeHandler *portETasks = TasksForPortE();

        portBTasks[PIN0].last = PB0;
        portBTasks[PIN1].last = PB1;
        portBTasks[PIN2].last = PB2;
        portBTasks[PIN3].last = PB3;

        portETasks[PIN0].last = PE0;
        portETasks[PIN1].last = PE1;
        portETasks[PIN2].last = PE2;
        portETasks[PIN3].last = PE3;

        RearmPinsForPortB();
        RearmPinsForPortE();
}

static void CheckPinForPortE(uint8_t pin) {
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

void CheckPortE(void) {
        CheckPinForPortE(PIN0);
        CheckPinForPortE(PIN1);
        CheckPinForPortE(PIN2);
        CheckPinForPortE(PIN3);
}

void GPIOPortE_Handler(void) {
        GPIO_PORTE_IM_R = CalculatedGpioValue(false, GPIO_PORTE_IM_R, activatedPins); // disarm pins
        GPIO_PORTB_IM_R = CalculatedGpioValue(false, GPIO_PORTB_IM_R, activatedPins); // disarm Port B pins

        CheckPortE();
        CheckPortB(); // check port B pins just in case

        Timer3aInit(MillisecToCycles(10), &DebounceTimerHandler);
}

EdgeHandler *TasksForPortE() { return edgeTasks; }


/**------------------- DEPRECATED -----------------------------*/
void FallingEdgeOnPortE(uint32_t pinNumber, PortAttribs attribs) {
        ERROR("NO FALLING EDGE");
}

void FallingEdgeOnPortEWith(uint32_t pinNumber,
                            PortAttribs attribs,
                            void (*handler)(void))
{
        ERROR("NO FALLING EDGE");
}
