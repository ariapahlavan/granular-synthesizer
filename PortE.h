//
// Created by Aria Pahlavan on 2/4/18.
//

#ifndef EE445L_PORTE_H
#define EE445L_PORTE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PortUtils.h"

#define PE0 (*((volatile uint32_t *)0x40024004))
#define PE1 (*((volatile uint32_t *)0x40024008))
#define PE2 (*((volatile uint32_t *)0x40024010))
#define PE3 (*((volatile uint32_t *)0x40024020))
#define PE4 (*((volatile uint32_t *)0x40024020))
#define PE5 (*((volatile uint32_t *)0x40024020))

#define EnableInterruptOnPortE  NVIC_EN0_R = 1 << 4
#define SetPriorityOfPortE(pri) NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF1F) | (pri & 0x7) << 5
#define PriorityOfPortE         (uint8_t) ((NVIC_PRI1_R & 0xFFFFFF1F) >> 5)

void PortEInit(uint32_t pinNumber, PortAttribs attribs);

EdgeHandler *TasksForPortE(void);

void RearmPinsForPortE(void);

void CheckPortE(void);

void DebounceTimerHandler(void);

void EdgeIntOnPortE(uint8_t pinNumber,
                    PortAttribs attribs,
                    void (*touchtask)(void),
                    void (*releasetask)(void));


/**------------------- DEPRECATED -----------------------------*/
void FallingEdgeOnPortE(uint32_t pinNumber, PortAttribs attribs);

void FallingEdgeOnPortEWith(uint32_t pinNumber,
                            PortAttribs attribs,
                            void (*handler)(void));

#ifdef __cplusplus
}
#endif
#endif //EE445L_PORTE_H
