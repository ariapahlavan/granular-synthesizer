//
// Created by Aria Pahlavan on 2/25/18.
//

#include "SynchSerialInterface.h"
#include "inc/tm4c123gh6pm.h"

void Ssi1Init(uint16_t data) {
        volatile uint32_t delay;
        SYSCTL_RCGCSSI_R |= 0x02;
        delay = SYSCTL_RCGCSSI_R;

        uint8_t DSS = 0x0F;             // bit[3:0]  DSS = 16-bit data
        uint8_t FRF = 0x00;             // bit[5:4]
        uint8_t SPO = 0x00;             // bit[6]
        uint8_t SPH = 0x80;             // bit[7]
        uint16_t SCR = 0x0000;          // bit[15:8] //0x0F00

        uint8_t CPSDVSR = 0x64;


        SSI1_CR1_R = 0x0;               // disable SS, master mode
        SSI1_CPSR_R = 0x02;             // 8MHz SSICLK; 0x08: 10Mhz SSIClk Fs = Fbus / (CPSDVSR * (1 + SCR))
        SSI1_CR0_R = (SCR + SPH + SPO + FRF + DSS) | (SSI1_CR0_R & 0xFFFF0000);
        SSI1_DR_R = data;               // load data into transmit fifo
        SSI1_CR1_R |= 0x02;             // enable SSI
}
