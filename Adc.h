// Lab11Main.c
// Runs on TM4C123
// APIs for interfacing ADC0 using Timer hardware triggering
// Aria Pahlavan, Mehrad Yousefi


#ifndef EE445L_ADC2_H
#define EE445L_ADC2_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define SOUND_ARRAY_LENGTH (31700)


void Adc0Seq3(uint8_t channelNum, uint32_t period, void (*task)(uint16_t));

void Adc0Seq2(uint8_t channelNum, uint32_t period, void (*task)(uint16_t));

void Adc0Seq1(uint8_t channelNum, uint32_t period, void (*task)(uint16_t));

uint16_t Seq3Value(void);

uint16_t *GetData();

uint32_t Seq2Value(void);

uint32_t Seq1Value(void);


#ifdef __cplusplus
}
#endif
#endif //EE445L_ADC2_H
