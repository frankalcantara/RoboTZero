#include "LedPattern.h"
#include "config.h"
#include <Arduino.h>

Timer LedPattern::patternTimer;
bool LedPattern::isPatternActive = false;
uint8_t LedPattern::patternState = 0;
uint32_t LedPattern::patternStartTime = 0;

void LedPattern::initialize() {
    pinMode(PIN_STATUS_LED, OUTPUT);
}

void LedPattern::startTransmissionPattern() {
    isPatternActive = true;
    patternState = 0;
    patternStartTime = millis();
    patternTimer.Start(1000); // Start with slow blink
}

void LedPattern::stopPattern() {
    isPatternActive = false;
    digitalWrite(PIN_STATUS_LED, LOW);
}

void LedPattern::process() {
    if (!isPatternActive) return;

    uint32_t elapsed = millis() - patternStartTime;

    // Switch pattern every 3 seconds
    if (elapsed >= 3000 && patternState == 0) {
        patternState = 1;
        patternTimer.Start(300);  // Switch to fast blink
    }
    else if (elapsed >= 6000) {
        patternState = 0;
        patternTimer.Start(1000); // Back to slow blink
        patternStartTime = millis();
    }

    if (patternTimer.Expired()) {
        digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
        patternTimer.Start(patternState == 0 ? 1000 : 300);
    }
}