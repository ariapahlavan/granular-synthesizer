/// \file TimerA.c
///
/// \note Runs on TM4C123
///
/// \brielf Use TimerA in periodic mode to request
/// interrupts at a particular period.
///
/// \Authors Aria Pahlavan, Mehrad Yousefi

#include <stdint.h>
#include <shared/startup.h>
#include "inc/tm4c123gh6pm.h"
#include "TimerA.h"


const uint32_t COLORWHEEL[WHEELSIZE] = {
        RED_LED, RED_LED + GREEN_LED, GREEN_LED, GREEN_LED + BLUE_LED, BLUE_LED,
        BLUE_LED + RED_LED, RED_LED + GREEN_LED + BLUE_LED, 0};

/** user functions to be executed by timer handlers */
void (*timer0Task)(void);

void (*timer1Task)(void);

void (*timer2Task)(void);

void (*timer3Task)(void);


/** Timer 0 A */
void Timer0aInit(uint32_t period, void(*task0)(void)) {
        volatile uint32_t delay;
        long sr = StartCritical();
        timer0Task = task0;

        // activation
        ActivateTimer0a;
        delay = SYSCTL_RCGCTIMER_R;
        DisableTimer0a;
        TIMER0_CFG_R = 0x00000000;      // 2) configure for 32-bit mode

        // configure for periodic mode
        TIMER0_TAMR_R = 0x00000002;     // 3) configure for periodic mode, default down-count settings
        ReloadTimer0aTo(period);
        TIMER0_TAPR_R = 0;              // 5) bus clock resolution
        TIMER0_ICR_R = 0x00000001;      // 6) clear TIMER0A timeout flag
        TIMER0_IMR_R = 0x00000001;      // 7) arm timeout interrupt

        // interrupt initialization
        SetTimer0aPriTo(2);
        ArmTimer0a;
        EnableTimer0a;
        EndCritical(sr);
}

void Timer0A_Handler(void) {
        AckTimer0a;
//        profileTimer0;
//        profileTimer0;
        (*timer0Task)();
//        profileTimer0;
}


/** Timer 1 A */
void Timer1aInit(uint32_t period, void(*task1)(void)) {
        volatile uint32_t delay;
        long sr = StartCritical();
        if (task1 != 0)
                timer1Task = task1;

        // activation
        ActivateTimer1a;
        delay = SYSCTL_RCGCTIMER_R;
        DisableTimer1a;
        TIMER1_CFG_R = 0;                // configure for 32-bit timer mode

        // configure for periodic mode
        TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
        ReloadTimer1aTo(period);
        TIMER1_IMR_R |= TIMER_IMR_TATOIM;  // enable timeout (rollover) interrupt
        TIMER1_ICR_R = TIMER_ICR_TATOCINT; // clear timer2A timeout flag
        TIMER1_CTL_R |= TIMER_CTL_TAEN;    // enable timer2A 32-b, periodic, interrupts

        // interrupt initialization
        SetTimer1aPriTo(2);
        ArmTimer1a;
        EnableTimer1a;
        EndCritical(sr);
}

void Timer1A_Handler(void) {
        AckTimer1a;
//        profileTimer1;
//        profileTimer1;
        (*timer1Task)();
        profileTimer1;
}


/** Timer 2 A */
void Timer2aInit(uint32_t period, void(*task2)(void)) {
        volatile uint32_t delay;
        long sr = StartCritical();
        timer2Task = task2;


        // activation
        ActivateTimer2a;
        delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
        DisableTimer2a;
        TIMER2_CFG_R = 0;                // configure for 32-bit timer mode

        // configure for periodic mode
        TIMER2_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
        ReloadTimer2aTo(period);
        TIMER2_IMR_R |= TIMER_IMR_TATOIM;  // enable timeout (rollover) interrupt
        TIMER2_ICR_R = TIMER_ICR_TATOCINT; // clear timer2A timeout flag
        TIMER2_CTL_R |= TIMER_CTL_TAEN;    // enable timer2A 32-b, periodic, interrupts

        // interrupt initialization
        SetTimer2aPriTo(2);
        ArmTimer2a;
        EnableTimer2a;
        EndCritical(sr);
}

void Timer2A_Handler(void) {
        AckTimer2a;
//        profileTimer2;
        (*timer2Task)();
//        profileTimer2;
        profileTimer2;
}


/** Timer 3 A */
void Timer3aInit(uint32_t period, void (*task3)(void)) {
        volatile uint32_t delay;
        long sr = StartCritical();
        timer3Task = task3;

        static uint8_t isActivated = 0;

        if (isActivated == 0) {
                // activation
                ActivateTimer3a;
                delay = SYSCTL_RCGCTIMER_R;      // allow time to finish activating
                isActivated = 1;
        }
        DisableTimer3a;
        TIMER3_CFG_R = 0;                // configure for 32-bit timer mode

        // configure for periodic mode
        TIMER3_TAMR_R = TIMER_TAMR_TAMR_PERIOD;
        ReloadTimer3aTo(period);
        TIMER3_IMR_R |= TIMER_IMR_TATOIM;  // enable timeout (rollover) interrupt
        TIMER3_ICR_R = TIMER_ICR_TATOCINT; // clear timer3A timeout flag
        TIMER3_CTL_R |= TIMER_CTL_TAEN;    // enable timer3A 32-b, periodic, interrupts

        // interrupt initialization
        SetTimer3aPriTo(2);
        ArmTimer3a;
        EnableTimer3a;
        EndCritical(sr);
}

void Timer3A_Handler(void) {
        AckTimer3a;
//        profileTimer0;
        (*timer3Task)();
//        profileTimer0;
//        profileTimer0;
}

