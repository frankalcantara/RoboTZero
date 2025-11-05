#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "config.h"
#include "DataStructures.h"
#include "FlashManager.h"

#if DEBUG_LEVEL > 0

class Logger {
public:
    // Initialize logger
    static bool initialize();

    // Start a new session
    static bool startSession(DebugMode mode, uint8_t plannedLaps);

    // End current session
    static void endSession();

    // Log a performance record
    static void logPerformance(int16_t linePosition, int16_t error,
        int16_t correction, uint8_t leftSpeed,
        uint8_t rightSpeed, uint8_t state);

// Log an event
    static void logEvent(EventType type, uint16_t data = 0);

    // Log lap statistics
    static void logLapStats(const LapStats& stats);

    // Check if logging is active
    static bool isLogging();

    // Get current session statistics
    static void getSessionStats(uint32_t& duration, uint8_t& completedLaps,
        uint16_t& totalCurves, float& avgDeviation);

// Process pending operations (call in main loop)
    static void process();

private:
    static bool isInitialized;
    static bool loggingActive;
    static uint32_t sessionStartTime;
    static uint8_t currentLap;
    static uint16_t curveCount;
    static uint32_t lastSampleTime;
    static uint32_t lastFlushTime;

    // Internal methods
    static void writeSessionHeader();
    static void flushBuffers();
    static bool shouldSample();
    static uint8_t calculateChecksum(const void* data, uint16_t size);
    static void updateStats(const PerformanceRecord& record);

    // Running statistics
    static float totalDeviation;
    static uint32_t sampleCount;
    static uint8_t maxSpeed;
};

#endif // DEBUG_LEVEL > 0
#endif // LOGGER_H