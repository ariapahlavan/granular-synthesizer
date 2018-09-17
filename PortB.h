//
// Created by Aria Pahlavan on 2/4/18.
//

#ifndef EE445L_PORTB_H
#define EE445L_PORTB_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PortUtils.h"

#define PB0        (*((volatile uint32_t *)0x40005004))
#define PB1        (*((volatile uint32_t *)0x40005008))
#define PB2        (*((volatile uint32_t *)0x40005010))
#define PB3        (*((volatile uint32_t *)0x40005020))
#define PB4        (*((volatile uint32_t *)0x40005040))
#define PB5        (*((volatile uint32_t *)0x40005080))
#define PB6        (*((volatile uint32_t *)0x40005200))

#define EnableInterruptOnPortB  NVIC_EN0_R = 1 << 1
#define SetPriorityOfPortB(pri) NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF1FFF) | (pri & 0x7) << 13
#define PriorityOfPortB         (uint8_t) ((NVIC_PRI0_R & 0xFFFF1FFF) >> 13)

void PortBInit(uint32_t pinNumber, PortAttribs attribs);

EdgeHandler *TasksForPortB(void);

void RearmPinsForPortB(void);

void CheckPortB(void);

void EdgeIntOnPortB(uint8_t pinNumber,
                    PortAttribs attribs,
                    void (*touchtask)(void),
                    void (releasetask)(void));


/**------------------- DEPRECATED -----------------------------*/
void FallingEdgeOnPortB(uint32_t pinNumber, PortAttribs attribs);

void FallingEdgeOnPortBWith(uint32_t pinNumber,
                            PortAttribs attribs,
                            void (*handler)(void));

#ifdef __cplusplus
}
#endif

#endif //EE445L_PORTB_H
