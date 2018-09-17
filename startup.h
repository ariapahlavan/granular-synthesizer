//
// Created by Aria Pahlavan on 2/5/18.
//

#ifndef EE445L_STARTUP_H
#define EE445L_STARTUP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Disable interrupts
 */
void DisableInterrupts(void);

/**
 * Enable interrupts
 */
void EnableInterrupts(void);

/**
 * previous I bit, disable interrupts
 */
long StartCritical(void);

/**
 * restore I bit to previous value
 * @param sr the value grabbed at the
 *        start of critical section
 */
void EndCritical(long sr);

/**
 * low power mode
 */
void WaitForInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif //EE445L_STARTUP_H
