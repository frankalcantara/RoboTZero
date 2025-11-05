#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"

#if DEBUG_LEVEL > 0
#define DEBUG_BEGIN(x) Serial.begin(x)
#define DEBUG_PRINT(x) Serial.print(F(x))
#define DEBUG_PRINTLN(x) Serial.println(F(x))
#define DEBUG_PRINT_VAL(x) Serial.print(x)
#define DEBUG_PRINTLN_VAL(x) Serial.println(x)
#else
#define DEBUG_BEGIN(x)
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT_VAL(x)
#define DEBUG_PRINTLN_VAL(x)
#endif

#endif // DEBUG_H