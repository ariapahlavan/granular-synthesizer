//
// Created by Aria Pahlavan on 2/5/18.
//

#ifndef EE445L_SWITCHES_H
#define EE445L_SWITCHES_H

#ifdef __cplusplus
extern "C" {
#endif

#define Switch1 PF4
#define Switch2 PF0

void Sw1Init(void);

void Sw2Init(void);

void WaitOnSw1(void);

void WaitOnSw2(void);

void WaitOnSw1AndClear(void);

void WaitOnSw2AndClear(void);

#ifdef __cplusplus
}
#endif

#endif //EE445L_SWITCHES_H
