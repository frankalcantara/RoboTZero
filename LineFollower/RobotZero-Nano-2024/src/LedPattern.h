#ifndef LEDPATTERN_H
#define LEDPATTERN_H

#include "Timer.h"

class LedPattern {
public:
    static void initialize();
    static void startTransmissionPattern();
    static void stopPattern();
    static void process();

private:
    static Timer patternTimer;
    static bool isPatternActive;
    static uint8_t patternState;  // 0 = slow blink, 1 = fast blink
    static uint32_t patternStartTime;
};

#endif