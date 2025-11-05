#ifndef COURSEMARKERS_H
#define COURSEMARKERS_H

#include "Timer.h"
#include <Arduino.h>

class CourseMarkers {
private:
    // Timing control
    static uint32_t lastReadTime;
    static const uint16_t MARKER_READ_INTERVAL = 2;  // Read interval in ms

    // State variables
    static int speed;
    static int lastMarkerState;
    static int previousMarkerState;
    static int oldMarkerState;
    static int currentMarkerState;
    static int16_t leftMarkerDetected;   // Changed to int16_t for analog read
    static int16_t rightMarkerDetected;  // Changed to int16_t for analog read

    // Speed control variables
    static bool isTurning;
    static bool isExitingTurn;
    static uint8_t boostCountdown;

    // Timers
    static Timer stopTimer;
    static Timer slowdownTimer;

    // Internal methods
    static void readCourseMarkers();
    static void handleFinishLine();
    static void handleSpeedMode();
    static void handleIntersection();

public:
    // Public methods
    static void processMarkerSignals();
    static int speedControl(int error);
};

#endif // COURSEMARKERS_H