// Timer.h
#ifndef TIMER_H
#define TIMER_H

class Timer {
private:
    unsigned long startTime;
    unsigned long duration;
    bool running;

public:
    Timer() : startTime(0), duration(0), running(false) {}

    // Inicia o timer com uma duração específica
    void start(unsigned long ms) {
        startTime = millis();
        duration = ms;
        running = true;
    }

    // Verifica se o tempo passou
    bool expired() {
        if (!running) return false;
        if (millis() - startTime >= duration) {
            running = false;
            return true;
        }
        return false;
    }

    // Para o timer
    void stop() {
        running = false;
    }
};

#endif