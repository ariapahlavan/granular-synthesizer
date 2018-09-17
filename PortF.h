//
// Created by Aria Pahlavan on 2/4/18.
//

#ifndef EE445L_PORTF_H
#define EE445L_PORTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "PortUtils.h"

#define PF0   (*((volatile uint32_t *)0x40025004))
#define PF1   (*((volatile uint32_t *)0x40025008))
#define PF2   (*((volatile uint32_t *)0x40025010))
#define PF3   (*((volatile uint32_t *)0x40025020))
#define PF4   (*((volatile uint32_t *)0x40025040))

#define EnableInterruptOnPortF  NVIC_EN0_R = 1 << 30
#define PriorityOfPortF         (uint8_t) ((NVIC_PRI7_R & 0xFF1FFFFF) >> 21)
#define SetPriorityOfPortF(pri) uint8_t maskedPri = (pri & 0x7) << 21; \
                                NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) | maskedPri

void PortFInit(uint32_t pinNumber, PortAttribs attribs);

void FallingEdgeOnPortF(uint32_t pinNumber, PortAttribs attribs);

void FallingEdgeOnPortFWith(uint32_t pinNumber,
                            PortAttribs attribs,
                            void (*handler)(void));

#ifdef __cplusplus
}
#endif
#endif //EE445L_PORTF_H
