//
// Created by Aria Pahlavan on 2/4/18.
//

#ifndef EE445L_PORTUTILS_H
#define EE445L_PORTUTILS_H

#include <stdbool.h>
#include <stdint.h>

#define nibble 4

#define PIN0      0x01
#define PIN1      0x02
#define PIN2      0x04
#define PIN3      0x08
#define PIN4      0x10
#define PIN5      0x20
#define PIN6      0x40
#define PIN7      0x80
#define ALL_PINS  0xFF

#define PORTA      0x01
#define PORTB      0x02
#define PORTC      0x04
#define PORTD      0x08
#define PORTE      0x10
#define PORTF      0x20

#define _activate_port()

#define pin0Mask(mask) mask << (0 * nibble)
#define pin1Mask(mask) mask << (1 * nibble)
#define pin2Mask(mask) mask << (2 * nibble)
#define pin3Mask(mask) mask << (3 * nibble)
#define pin4Mask(mask) mask << (4 * nibble)
#define pin5Mask(mask) mask << (5 * nibble)
#define pin6Mask(mask) mask << (6 * nibble)
#define pin7Mask(mask) mask << (7 * nibble)

#define extractPin0(val) val & PIN0
#define extractPin1(val) val & PIN1
#define extractPin2(val) val & PIN2
#define extractPin3(val) val & PIN3
#define extractPin4(val) val & PIN4
#define extractPin5(val) val & PIN5
#define extractPin6(val) val & PIN6
#define extractPin7(val) val & PIN7

/**
 * @brief Port ConTrL
 *
 * @details For digital functions, each pin also has four bits in the GPIOPCTL register,
 * which we set to specify the alternative function for that pin (0 means regular I/O port).
 */
typedef enum {
        REGULAR_IO = 0,
        SSI1 = 2,
        PWM0 = 4
} Pctl;

typedef enum {
        DATA_IN,
        DATA_OUT,
} DataDirection;

typedef struct {
        Pctl pctl;
        bool pullUpEnable;
        DataDirection dataDirection;
        bool analogEnable;            // Analog Mode Select (AMSEL)
        bool alternateFunctionEnable; //Alternate Function SELect (AFSEL)
        bool portCommitLock;          // lock: true, unlock: false
} PortAttribs;

typedef struct {
        void (*func)(void);
} Handler;

typedef struct {
        bool touch;
        bool release;
        uint32_t last;
        void (*TouchHandler)(void);
        void (*ReleaseHandler)(void);
} EdgeHandler;

PortAttribs RegularDigitalInput(bool isPullup);

PortAttribs RegularDigitalOutput(bool isPullup);

PortAttribs RegularAnalogOutput(bool isPullup);

PortAttribs RegularAnalogInput(bool isPullup);

PortAttribs AdcInput(void);

PortAttribs PwmAttribs(void);

PortAttribs SwitchAttribs(bool isPullup);

uint32_t pctlOf(uint32_t pinNumber, uint32_t mask);

#define CalculatedGpioValue(condition, gpioRegister, n) (condition ? (gpioRegister | n) : (gpioRegister & ~n))

#define CalculatedGpioPctl(pctl, gpioRegister, n) ((gpioRegister & ~maskOf(n)) | pctl)

#define maskOf(pinNumber) (pctlOf(pinNumber, 0xF))

#endif //EE445L_PORTUTILS_H
