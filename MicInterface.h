// MicInterface.h
// Runs on TM4C123
// Microphone APIs for interfacing a microphone
// Aria Pahlavan, Mehrad Yousefi

#ifndef EE445L_MICINTERFACE_H
#define EE445L_MICINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * Init microphone if not, and start sampling
 * values for the pre-specified amount, and store
 * in \code soundWave
 */
uint8_t *GetSound(void);

/**
 * @brief play the recorded values from microphone
 */
void MicInit(void);


#ifdef __cplusplus
}
#endif
#endif //EE445L_MICINTERFACE_H

