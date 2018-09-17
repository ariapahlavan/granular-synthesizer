// TimerA.h
// Runs on TM4C123
// Use TimerA in periodic mode to request interrupts at a particular
// period.
// Aria Pahlavan, Mehrad Yousefi

#ifndef __TIMER0AINTS_H__ // do not include more than once
#define __TIMER0AINTS_H__

#include <inc/tm4c123gh6pm.h>

/** Profiling macros */
#define LEDS            (*((volatile uint32_t *)0x40025038))
#define RED_LED         0x02
#define BLUE_LED        0x04
#define GREEN_LED       0x08
#define WHEELSIZE       8

#define profileTimer0 LEDS ^= BLUE_LED
#define profileTimer1 LEDS ^= RED_LED
#define profileTimer2 LEDS ^= GREEN_LED

extern const uint32_t COLORWHEEL[WHEELSIZE];


/** Timer 0A macros */
#define AckTimer0a              TIMER0_ICR_R = TIMER_ICR_TATOCINT
#define ActivateTimer0a         SYSCTL_RCGCTIMER_R |= 0x01
#define DisableTimer0a          TIMER0_CTL_R = 0x00000000
#define EnableTimer0a           TIMER0_CTL_R = 0x00000001
#define ArmTimer0a              NVIC_EN0_R = 1 << 19
#define ReloadTimer0aTo(x)      TIMER0_TAILR_R = x - 1
#define SetTimer0aPriTo(x)      NVIC_PRI4_R = (NVIC_PRI4_R & 0x1FFFFFFF) | ((x) << 29)

/**
 * @brief Activate Timer0A interrupts to run user task periodically
 *
 * @param period
 *              period in units (1/clockfreq), 32 bits
 */
void Timer0aInit(uint32_t period, void(*task0)(void));


/** Timer 1A macros */
#define AckTimer1a              TIMER1_ICR_R = TIMER_ICR_TATOCINT
#define ActivateTimer1a         SYSCTL_RCGCTIMER_R |= 0x02
#define DisableTimer1a          TIMER1_CTL_R = 0x00000000
#define EnableTimer1a           TIMER1_CTL_R = 0x00000001
#define ArmTimer1a              NVIC_EN0_R = 1 << 21
#define ReloadTimer1aTo(x)      TIMER1_TAILR_R = x - 1
#define SetTimer1aPriTo(x)      NVIC_PRI5_R = (NVIC_PRI5_R & 0xFFFF1FFF) | ((x) << 13)

/**
 * @brief Activate Timer1A interrupts to run user task periodically
 *
 * @param period
 *              period in units (1/clockfreq), 32 bits
 */
void Timer1aInit(uint32_t period, void(*task1)(void));


/** Timer 2A macros */
#define AckTimer2a              TIMER2_ICR_R = TIMER_ICR_TATOCINT
#define ActivateTimer2a         SYSCTL_RCGCTIMER_R |= 0x04
#define DisableTimer2a          TIMER2_CTL_R &= ~TIMER_CTL_TAEN
#define EnableTimer2a           TIMER2_CTL_R |= TIMER_CTL_TAEN
#define ArmTimer2a              NVIC_EN0_R = 1 << 23
#define ReloadTimer2aTo(x)      TIMER2_TAILR_R = x - 1
#define SetTimer2aPriTo(x)      NVIC_PRI5_R = (NVIC_PRI5_R & 0x1FFFFFFF) | ((x) << 29)

/**
 * @brief Activate Timer2A interrupts to run user task periodically
 *
 * @param period
 *              period in units (1/clockfreq), 32 bits
 */
void Timer2aInit(uint32_t period, void(*task2)(void));


/** Timer 3A macros */
#define AckTimer3a              TIMER3_ICR_R = TIMER_ICR_TATOCINT
#define ActivateTimer3a         SYSCTL_RCGCTIMER_R |= 0x08
#define DisableTimer3a          TIMER3_CTL_R &= ~TIMER_CTL_TAEN
#define EnableTimer3a           TIMER3_CTL_R |= TIMER_CTL_TAEN
#define ArmTimer3a              NVIC_EN1_R = 1 << 3
#define DisarmTimer3a           TIMER3_IMR_R &= ~TIMER_IMR_TATOIM
#define ReloadTimer3aTo(x)      TIMER3_TAILR_R = x - 1
#define SetTimer3aPriTo(x)      NVIC_PRI8_R = (NVIC_PRI8_R & 0x1FFFFFFF) | ((x) << 29)

/**
 * @brief Activate Timer3A interrupts to run user task periodically
 *
 * @param period
 *              period in units (1/clockfreq), 32 bits
 */
void Timer3aInit(uint32_t period, void(*task3)(void));

#endif // __TIMER0AINTS_H__
