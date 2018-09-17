// KeyboardInterface.h
// Runs on TM4C123
// Keyboard APIs for interfacing switches and piano keys
// Aria Pahlavan, Mehrad Yousefi

#ifndef EE445L_KEYBOARDAPI_H
#define EE445L_KEYBOARDAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLE_TIME 30

/**
 * @brief initialize all switches corresponding to
 * piano keys as both-edge triggered GPIOs
 */
void KeyboardInit();

#ifdef __cplusplus
}
#endif
#endif //EE445L_KEYBOARDAPI_H
