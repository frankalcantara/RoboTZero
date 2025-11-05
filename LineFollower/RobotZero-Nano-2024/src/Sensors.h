#ifndef SENSORS_H
#define SENSORS_H

#include "config.h"
#include "Timer.h"

class Sensors {
private:
    static int16_t sensorMin[NUM_SENSORES];
    static int16_t sensorMax[NUM_SENSORES];
    static volatile int16_t processedSensorValues[NUM_SENSORES];
    static boolean isLineDetected;
    static int16_t lastValidLinePosition;
    static int16_t lastValidPosition;

    // Helper method
    static void readSensors();

public:
    // Calibration method
    static void calibration();

    // Line position calculation
    static int16_t calculateLinePosition();
};

#endif // SENSORS_H