// PotInterface.h
// Runs on TM4C123
// Potentiometer APIs for interfacing slide and knob pots
// Aria Pahlavan, Mehrad Yousefi

#ifndef EE445L_POTINTERFACE_H
#define EE445L_POTINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

/** @return the upper bound of the sound arrays recorded */
int UpperBound(void);

/** @return the lower bound of the sound arrays recorded */
int LowerBound(void);

/**
 * Initialize both slide and knob potentiometers
 */
void PotInit(void);

/**
 * Based on the last value sampled from the slide,
 * if necessary, moves the bars to the appropriate
 * y-coordinates.
 */
void MoveBars(void);

/**
 * Based on the last value sampled from the knob,
 * if necessary, adjusts the distance between bars.
 */
void AdjustWindow(void);


#ifdef __cplusplus
}
#endif
#endif //EE445L_POTINTERFACE_H

