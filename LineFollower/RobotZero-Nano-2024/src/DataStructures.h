#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include <Arduino.h>

#if DEBUG_LEVEL > 0

// Operation modes for debug
enum class DebugMode : uint8_t {
    NORMAL = 0,    // No debugging (should never be set in debug)
    ANALYSIS = 1,  // Analysis mode with configurable speed
    SPEED = 2      // High speed performance analysis
};

// Speed and control parameters profile
struct SpeedProfile {
    // Speed settings
    uint8_t speedStop;      // Stopped
    uint8_t speedStartup;   // Initial movement
    uint8_t speedTurn;      // Turn speed
    uint8_t speedBrake;     // Braking speed
    uint8_t speedCruise;    // Medium speed
    uint8_t speedSlow;      // Precision mode speed
    uint8_t speedFast;      // High speed
    uint8_t speedBoost;     // Boost speed
    uint8_t speedMax;       // Maximum speed

    // Speed control parameters
    uint8_t accelerationStep;    // Acceleration rate
    uint8_t brakeStep;          // Braking rate
    uint8_t turnSpeed;          // Speed during turns
    uint8_t turnThreshold;      // Turn detection threshold
    uint8_t straightThreshold;  // Straight line threshold
    uint8_t boostDuration;      // Boost time after curves
    uint8_t boostIncrement;     // Boost acceleration

    // PID control parameters
    float kProportional;       // Proportional gain
    float kDerivative;        // Derivative gain
    float filterCoefficient;  // Error filter coefficient
};

// Log event types
enum class EventType : uint8_t {
    SESSION_START = 0x01,
    LAP_START = 0x02,
    LAP_END = 0x03,
    CURVE_ENTER = 0x04,
    CURVE_EXIT = 0x05,
    MODE_CHANGE = 0x06,
    SPEED_CHANGE = 0x07,
    ERROR_DETECTED = 0x08,
    SESSION_END = 0x09
};

// Event record structure (8 bytes)
struct EventRecord {
    uint32_t timestamp;    // Time since start
    EventType type;        // Event type
    uint16_t data;        // Event specific data
    uint8_t checksum;     // Data validation
};

// Performance record structure (12 bytes)
struct PerformanceRecord {
    uint32_t timestamp;     // Time since start
    int16_t linePosition;   // Current line position
    int16_t error;         // Current error
    int16_t correction;    // Applied correction
    uint8_t speedLeft;     // Left motor speed
    uint8_t speedRight;    // Right motor speed
    uint8_t state;         // Robot state flags
    uint8_t checksum;      // Data validation
};

// Session header structure (32 bytes)
struct SessionHeader {
    uint32_t startTime;          // Session start timestamp
    DebugMode mode;             // Operating mode
    uint8_t plannedLaps;        // Number of laps to run
    uint8_t configuredSpeed;    // Base speed setting
    float pidKp;                // PID proportional constant
    float pidKd;                // PID derivative constant
    float filterAlpha;          // Error filter coefficient
    uint16_t sensorCalibrationData[6]; // Calibration values
    uint32_t headerChecksum;    // Header validation
};

// Lap statistics structure (16 bytes)
struct LapStats {
    uint32_t startTime;      // Lap start time
    uint32_t duration;       // Lap duration
    uint16_t curves;         // Number of curves
    uint16_t maxDeviation;   // Maximum line deviation
    float avgDeviation;      // Average line deviation
    uint8_t maxSpeed;        // Maximum speed reached
    uint8_t checksum;        // Data validation
};

#endif // DEBUG_LEVEL > 0
#endif // DATASTRUCTURES_H