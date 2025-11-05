#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
private:
    unsigned long startTime;    // Start timestamp
    unsigned long duration;     // Timer duration
    bool running;              // Timer state

public:
    Timer() : startTime(0), duration(0), running(false) {}

    // Start timer with specific duration
    void Start(unsigned long ms) {
        startTime = millis();
        duration = ms;
        running = true;
    }

    // Check if timer has Expired
    bool Expired() {
        if (!running) return false;
        if (millis() - startTime >= duration) {
            running = false;
            return true;
        }
        return false;
    }

    // Stop timer
    void Stop() {
        running = false;
    }
};

#endif