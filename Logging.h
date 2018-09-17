//
// Created by Aria Pahlavan on 2/4/18.
//

#ifndef EE445L_LOGGING_H
#define EE445L_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Utils.h"
#include <stdint.h>
#include <stdbool.h>

typedef enum {
        _DEBUG = 0,
        _ERROR = 1
} _logType;

#define IsDebugEnable true

#define _string(t) (char *)t

#define __info__ __LINE__,_string(__FUNCTION__),_string(__FILE__)

#define ERROR(message) loggingMsg(__info__, _string(message), RED, _ERROR);
#define ERROR1(message, value) \
        loggingWihoutTitle(__info__, _string(message), value, RED, _ERROR);
#define ERROR2(title, message, value) loggingWithValue(__info__, _string(title), _string(message), value, RED, _ERROR);
#define ERROR3(title, message) logging(__info__, _string(title), _string(message), RED, _ERROR);

#define DEBUG(message) if (IsDebugEnable) loggingMsg(__info__, _string(message), ORANGE, _DEBUG);
#define DEBUG1(message, value) if (IsDebugEnable) loggingWihoutTitle(__info__, _string(message), value, ORANGE, _DEBUG);
#define DEBUG2(title, message, value) if (IsDebugEnable) \
        loggingWithValue(__info__, _string(title), _string(message), value, ORANGE, _DEBUG);
#define DEBUG3(title, message) if (IsDebugEnable) logging(__info__, _string(title), _string(message), ORANGE, _DEBUG);


void loggingMsg(uint32_t lineNum, char *funcName, char *fileName, char *message, uint16_t color, _logType type);

void logging(uint32_t lineNum, char *funcName, char *fileName, char *title, char *message, uint16_t color,
             _logType type);

void loggingWihoutTitle(uint32_t lineNum, char *funcName, char *fileName, char *message, uint32_t value, uint16_t color,
                        _logType type);

void loggingWithValue(uint32_t lineNum, char *funcName, char *fileName, char *title, char *message,
                      uint32_t value, uint16_t color, _logType type);


#ifdef __cplusplus
}
#endif
#endif //EE445L_LOGGING_H
