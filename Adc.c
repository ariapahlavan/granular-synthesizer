// Lab11Main.c
// Runs on TM4C123
// APIs for interfacing ADC0 using Timer hardware triggering
// Aria Pahlavan, Mehrad Yousefi


#include "Adc.h"
#include <startup.h>
#include <stdbool.h>
#include <math.h>
#include "KeyboardInterface.h"
#include "PotInterface.h"
#include "LcdInterface.h"
#include "tm4c123gh6pm.h"
#include "Dac.h"
#include "ST7735.h"
#include "Logging.h"

void (*Seq1Task)(uint16_t);
void (*Seq2Task)(uint16_t);
void (*Seq3Task)(uint16_t);

void DisarmSeq3();

/**
 * @brief Initialize the GPIO port corresponding to {@code channelNum}
 * @param channelNum channel number used for ADC sampling
 */
void InitGpio(uint8_t channelNum) {
        volatile uint32_t delay;
        switch (channelNum) {             // 1) activate clock
                case 0:
                case 1:
                case 2:
                case 3:
                case 8:
                case 9:                       //    these are on GPIO_PORTE
                        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
                        break;
                case 4:
                case 5:
                case 6:
                case 7:                       //    these are on GPIO_PORTD
                        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
                        break;
                case 10:
                case 11:                      //    these are on GPIO_PORTB
                        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
                        break;
                default:
                        return;              //    0 to 11 are valid channels on the LM4F120
        }
        delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
        delay = SYSCTL_RCGCGPIO_R;
        switch (channelNum) {
                case 0:                       //      Ain0 is on PE3
                        GPIO_PORTE_DIR_R &= ~0x08;  // 3.0) make PE3 input
                        GPIO_PORTE_AFSEL_R |= 0x08; // 4.0) enable alternate function on PE3
                        GPIO_PORTE_DEN_R &= ~0x08;  // 5.0) disable digital I/O on PE3
                        GPIO_PORTE_AMSEL_R |= 0x08; // 6.0) enable analog functionality on PE3
                        break;
                case 1:                       //      Ain1 is on PE2
                        GPIO_PORTE_DIR_R &= ~0x04;  // 3.1) make PE2 input
                        GPIO_PORTE_AFSEL_R |= 0x04; // 4.1) enable alternate function on PE2
                        GPIO_PORTE_DEN_R &= ~0x04;  // 5.1) disable digital I/O on PE2
                        GPIO_PORTE_AMSEL_R |= 0x04; // 6.1) enable analog functionality on PE2
                        break;
                case 2:                       //      Ain2 is on PE1
                        GPIO_PORTE_DIR_R &= ~0x02;  // 3.2) make PE1 input
                        GPIO_PORTE_AFSEL_R |= 0x02; // 4.2) enable alternate function on PE1
                        GPIO_PORTE_DEN_R &= ~0x02;  // 5.2) disable digital I/O on PE1
                        GPIO_PORTE_AMSEL_R |= 0x02; // 6.2) enable analog functionality on PE1
                        break;
                case 3:                       //      Ain3 is on PE0
                        GPIO_PORTE_DIR_R &= ~0x01;  // 3.3) make PE0 input
                        GPIO_PORTE_AFSEL_R |= 0x01; // 4.3) enable alternate function on PE0
                        GPIO_PORTE_DEN_R &= ~0x01;  // 5.3) disable digital I/O on PE0
                        GPIO_PORTE_AMSEL_R |= 0x01; // 6.3) enable analog functionality on PE0
                        break;
                case 4:                       //      Ain4 is on PD3
                        GPIO_PORTD_DIR_R &= ~0x08;  // 3.4) make PD3 input
                        GPIO_PORTD_AFSEL_R |= 0x08; // 4.4) enable alternate function on PD3
                        GPIO_PORTD_DEN_R &= ~0x08;  // 5.4) disable digital I/O on PD3
                        GPIO_PORTD_AMSEL_R |= 0x08; // 6.4) enable analog functionality on PD3
                        break;
                case 5:                       //      Ain5 is on PD2
                        GPIO_PORTD_DIR_R &= ~0x04;  // 3.5) make PD2 input
                        GPIO_PORTD_AFSEL_R |= 0x04; // 4.5) enable alternate function on PD2
                        GPIO_PORTD_DEN_R &= ~0x04;  // 5.5) disable digital I/O on PD2
                        GPIO_PORTD_AMSEL_R |= 0x04; // 6.5) enable analog functionality on PD2
                        break;
                case 6:                       //      Ain6 is on PD1
                        GPIO_PORTD_DIR_R &= ~0x02;  // 3.6) make PD1 input
                        GPIO_PORTD_AFSEL_R |= 0x02; // 4.6) enable alternate function on PD1
                        GPIO_PORTD_DEN_R &= ~0x02;  // 5.6) disable digital I/O on PD1
                        GPIO_PORTD_AMSEL_R |= 0x02; // 6.6) enable analog functionality on PD1
                        break;
                case 7:                       //      Ain7 is on PD0
                        GPIO_PORTD_DIR_R &= ~0x01;  // 3.7) make PD0 input
                        GPIO_PORTD_AFSEL_R |= 0x01; // 4.7) enable alternate function on PD0
                        GPIO_PORTD_DEN_R &= ~0x01;  // 5.7) disable digital I/O on PD0
                        GPIO_PORTD_AMSEL_R |= 0x01; // 6.7) enable analog functionality on PD0
                        break;
                case 8:                       //      Ain8 is on PE5
                        GPIO_PORTE_DIR_R &= ~0x20;  // 3.8) make PE5 input
                        GPIO_PORTE_AFSEL_R |= 0x20; // 4.8) enable alternate function on PE5
                        GPIO_PORTE_DEN_R &= ~0x20;  // 5.8) disable digital I/O on PE5
                        GPIO_PORTE_AMSEL_R |= 0x20; // 6.8) enable analog functionality on PE5
                        break;
                case 9:                       //      Ain9 is on PE4
                        GPIO_PORTE_DIR_R &= ~0x10;  // 3.9) make PE4 input
                        GPIO_PORTE_AFSEL_R |= 0x10; // 4.9) enable alternate function on PE4
                        GPIO_PORTE_DEN_R &= ~0x10;  // 5.9) disable digital I/O on PE4
                        GPIO_PORTE_AMSEL_R |= 0x10; // 6.9) enable analog functionality on PE4
                        break;
                case 10:                      //       Ain10 is on PB4
                        GPIO_PORTB_DIR_R &= ~0x10;  // 3.10) make PB4 input
                        GPIO_PORTB_AFSEL_R |= 0x10; // 4.10) enable alternate function on PB4
                        GPIO_PORTB_DEN_R &= ~0x10;  // 5.10) disable digital I/O on PB4
                        GPIO_PORTB_AMSEL_R |= 0x10; // 6.10) enable analog functionality on PB4
                        break;
                case 11:                      //       Ain11 is on PB5
                        GPIO_PORTB_DIR_R &= ~0x20;  // 3.11) make PB5 input
                        GPIO_PORTB_AFSEL_R |= 0x20; // 4.11) enable alternate function on PB5
                        GPIO_PORTB_DEN_R &= ~0x20;  // 5.11) disable digital I/O on PB5
                        GPIO_PORTB_AMSEL_R |= 0x20; // 6.11) enable analog functionality on PB5
                        break;
        }
}

static uint8_t PRI = 2;

/**
 * Sample sequencer 3 generates a raw interrupt when the conversion
 * is complete, and it is then promoted to an ADC0 controller
 * interrupt.  Hardware Timer0A triggers the ADC0 conversion at
 * the programmed interval, and software handles the interrupt to
 * process the measurement when it is complete.
 *
 * Timer0A: enabled
 * Mode: 32-bit, down counting
 * One-shot or periodic: periodic
 * Interval value: programmable using 32-bit period
 * Sample time is busPeriod*period
 * Max sample rate: <=125,000 samples/second
 * Sequencer 0 priority: 1st (highest)
 * Sequencer 1 priority: 2nd
 * Sequencer 2 priority: 3rd
 * Sequencer 3 priority: 4th (lowest)
 * SS3 triggering event: Timer0A
 * SS3 1st sample source: programmable using variable 'channelNum' [0:11]
 * SS3 interrupts: enabled and promoted to controller
*/
void Adc0Seq3(uint8_t channelNum, uint32_t period, void (*task)(uint16_t)) {
        volatile uint32_t delay;
        InitGpio(channelNum);

        Seq3Task = task;

        DisableInterrupts();
        SYSCTL_RCGCADC_R |= 0x01;     // activate ADC0
        SYSCTL_RCGCTIMER_R |= 0x01;   // activate timer0
        delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating

        TIMER0_CTL_R = 0x00000000;    // disable timer0A during setup
        TIMER0_CTL_R |= 0x00000020;   // enable timer0A trigger to ADC
        TIMER0_CFG_R = 0;             // configure for 32-bit timer mode
        TIMER0_TAMR_R = 0x00000002;   // configure for periodic mode, default down-count settings
        TIMER0_TAPR_R = 0;            // prescale value for trigger
        TIMER0_TAILR_R = period - 1;    // start value for trigger
        TIMER0_IMR_R = 0x00000000;    // disable all interrupts
        TIMER0_CTL_R |= 0x00000001;   // enable timer0A 32-b, periodic, no interrupts

        ADC0_PC_R = 0x01;         // configure for 125K samples/sec
        ADC0_SSPRI_R = 0x1110;    // sequencer 0 is highest, sequencer 3 is lowest
        ADC0_ACTSS_R &= ~0x08;    // disable sample sequencer 3
        ADC0_EMUX_R = (ADC0_EMUX_R & 0xFFFF0FFF) + 0x5000; // timer trigger event
        ADC0_SSMUX3_R = channelNum;
        ADC0_SSCTL3_R = 0x06;          // set flag and end
        ADC0_IM_R |= 0x08;             // enable SS3 interrupts
        ADC0_ACTSS_R |= 0x08;          // enable sample sequencer 3
        NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFF1FFF) | (PRI << 13); //SS3 pri
        NVIC_EN0_R = 1 << 17;              // enable interrupt 17 in NVIC
}

void Adc0Seq2(uint8_t channelNum, uint32_t period, void (*task)(uint16_t)) {
        volatile uint32_t delay;
        InitGpio(channelNum);

        Seq2Task = task;

        DisableInterrupts();
        SYSCTL_RCGCADC_R |= 0x01;     // activate ADC0
        SYSCTL_RCGCTIMER_R |= 0x02;   // activate timer1
        delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating

        TIMER1_CTL_R = 0x00000000;    // disable timer0A during setup
        TIMER1_CTL_R |= 0x00000020;   // enable timer0A trigger to ADC
        TIMER1_CFG_R = 0;             // configure for 32-bit timer mode
        TIMER1_TAMR_R = 0x00000002;   // configure for periodic mode, default down-count settings
        TIMER1_TAPR_R = 0;            // prescale value for trigger
        TIMER1_TAILR_R = period - 1;    // start value for trigger
        TIMER1_IMR_R = 0x00000000;    // disable all interrupts
        TIMER1_CTL_R |= 0x00000001;   // enable timer0A 32-b, periodic, no interrupts

        ADC0_PC_R = 0x01;         // configure for 125K samples/sec
        ADC0_SSPRI_R = 0x1110;    // sequencer 0 is highest, sequencer 2 is lowest
        ADC0_ACTSS_R &= ~0x04;    // disable sample sequencer 2
        ADC0_EMUX_R = (ADC0_EMUX_R & 0xFFFFF0FF) + 0x0500; // timer trigger event
        ADC0_SSMUX2_R = channelNum;
        ADC0_SSCTL2_R = 0x06;          // set flag and end
        ADC0_IM_R |= 0x04;             // enable SS2 interrupts
        ADC0_ACTSS_R |= 0x04;          // enable sample sequencer 2

        NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF1F) | (PRI << 5);  //SS2 pri
        NVIC_EN0_R = 1 << 16;              // enable interrupt 16 in NVIC
}

void Adc0Seq1(uint8_t channelNum, uint32_t period, void (*task)(uint16_t)) {
        volatile uint32_t delay;
        InitGpio(channelNum);

        Seq1Task = task;

        DisableInterrupts();
        SYSCTL_RCGCADC_R |= 0x01;     // activate ADC0
        SYSCTL_RCGCTIMER_R |= 0x04;   // activate timer2
        delay = SYSCTL_RCGCTIMER_R;   // allow time to finish activating

        TIMER2_CTL_R = 0x00000000;    // disable timer2A during setup
        TIMER2_CTL_R |= 0x00000020;   // enable timer2A trigger to ADC
        TIMER2_CFG_R = 0;             // configure for 32-bit timer mode
        TIMER2_TAMR_R = 0x00000002;   // configure for periodic mode, default down-count settings
        TIMER2_TAPR_R = 0;            // prescale value for trigger
        TIMER2_TAILR_R = period - 1;    // start value for trigger
        TIMER2_IMR_R = 0x00000000;    // disable all interrupts
        TIMER2_CTL_R |= 0x00000001;   // enable timer2A 32-b, periodic, no interrupts

        ADC0_PC_R = 0x01;         // configure for 125K samples/sec
        ADC0_SSPRI_R = 0x1110;    // sequencer 0 is highest, sequencer 1 is lowest
        ADC0_ACTSS_R &= ~0x02;    // disable sample sequencer 1
        ADC0_EMUX_R = (ADC0_EMUX_R & 0xFFFFFF0F) + 0x0050; // timer trigger event
        ADC0_SSMUX1_R = channelNum;
        ADC0_SSCTL1_R = 0x06;          // set flag and end
        ADC0_IM_R |= 0x02;             // enable SS2 interrupts
        ADC0_ACTSS_R |= 0x02;          // enable sample sequencer 1

        NVIC_PRI3_R = (NVIC_PRI3_R & 0x1FFFFFFF) | (PRI << 29); //SS1 pri
        NVIC_EN0_R = 1 << 15;              // enable interrupt 15 in NVIC
}


volatile uint16_t seq3Value;
void ADC0Seq3_Handler(void) {
        ADC0_ISC_R = 0x08;          // acknowledge ADC sequence 3 completion
        seq3Value = ADC0_SSFIFO3_R;  // 12-bit result

        (*Seq3Task)(seq3Value);
}

uint16_t Seq3Value(void) { return seq3Value; }



volatile uint16_t seq2Value;

void ADC0Seq2_Handler(void) {
        ADC0_ISC_R = 0x04;              // acknowledge ADC sequence 2 completion
        seq2Value = ADC0_SSFIFO2_R;     // 12-bit result

        (*Seq2Task)(seq2Value);
}

uint32_t Seq2Value(void) { return seq2Value; }


volatile uint16_t seq1Value;

void ADC0Seq1_Handler(void) {
        ADC0_ISC_R = 0x02;              // acknowledge ADC sequence 1 completion
        seq1Value = ADC0_SSFIFO1_R;     // 12-bit result

        (*Seq1Task)(seq1Value);
}

uint32_t Seq1Value(void) { return seq1Value; }