#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ====== Debugging ======
// Set to 1 for analysis mode, 2 for speed mode, or 0 for normal operation
#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 2  // Set to speed mode by default for better performance
#endif

#if DEBUG_LEVEL > 0
#include "DataStructures.h"  // Must be included before ProfileManager.h

// Debug configuration
static constexpr uint8_t DEBUG_LAPS_MODE1 = 5;    // Number of laps for analysis mode
static constexpr uint8_t DEBUG_LAPS_MODE2 = 3;    // Number of laps for speed mode

// Logging parameters - Optimized for performance
static constexpr uint16_t SAMPLE_RATE_STRAIGHT = 100;  // Reduced sampling in straight
static constexpr uint16_t SAMPLE_RATE_CURVE = 40;      // Reduced sampling in curves
static constexpr uint16_t LOG_BUFFER_SIZE = 64;        // Size of circular buffer

// Flash memory parameters
static constexpr uint32_t FLASH_LOG_START = 0x1000;    // Start address for logging
static constexpr uint16_t FLASH_PAGE_SIZE = 256;       // Flash page size for write operations
static constexpr uint32_t FLASH_CONTROL_BYTE = 0x0800; // Control byte address
static constexpr uint8_t FLASH_LOG_READY = 0xAA;       // Value indicating log is ready

// LED Pattern parameters
static constexpr uint16_t LED_SLOW_BLINK = 1000;       // Slow blink interval (ms)
static constexpr uint16_t LED_FAST_BLINK = 300;        // Fast blink interval (ms)
static constexpr uint16_t LED_PATTERN_SWITCH = 3000;   // Time to switch patterns (ms)
#endif

// ====== Pins ======
// Only modify if changing physical robot connections
static const uint8_t PIN_START_BUTTON = 11;      // Start/calibrate button
static const uint8_t PIN_STATUS_LED = 13;        // Status indicator LED
static const uint8_t PIN_MOTOR_LEFT_FWD = 7;     // Left Motor Direction
static const uint8_t PIN_MOTOR_LEFT_REV = 4;     // Left Motor Direction
static const uint8_t PIN_MOTOR_LEFT_PWM = 3;     // Left Motor Speed
static const uint8_t PIN_MOTOR_RIGHT_FWD = 8;    // Right Motor Direction
static const uint8_t PIN_MOTOR_RIGHT_REV = 9;    // Right Motor Direction
static const uint8_t PIN_MOTOR_RIGHT_PWM = 10;   // Right Motor Speed

// Optimized button debounce time
static const uint16_t DEBOUNCE_DELAY = 30;  // Reduced from 50ms for faster response

// Sensor pins - Only modify if changing physical connections
static const uint8_t PIN_LINE_LEFT_EDGE = A6;      // Leftmost sensor
static const uint8_t PIN_LINE_LEFT_MID = A5;
static const uint8_t PIN_LINE_CENTER_LEFT = A4;
static const uint8_t PIN_LINE_CENTER_RIGHT = A3;
static const uint8_t PIN_LINE_RIGHT_MID = A2;
static const uint8_t PIN_LINE_RIGHT_EDGE = A1;     // Rightmost sensor
static const uint8_t PIN_MARKER_LEFT = A7;         // Left marker
static const uint8_t PIN_MARKER_RIGHT = A0;        // Right marker

// Optimized marker reading interval
static constexpr uint16_t MARKER_READ_INTERVAL = 5;  // Increased from 2ms for better performance

// ====== Predefined Speeds ======
// Optimized speed settings for better performance
static constexpr uint8_t SPEED_STOP = 0;       // Stopped
static constexpr uint8_t SPEED_STARTUP = 140;  // Increased initial speed
static constexpr uint8_t SPEED_TURN = 160;     // Faster turns
static constexpr uint8_t SPEED_BRAKE = 180;    // Stronger braking
static constexpr uint8_t SPEED_CRUISE = 200;   // Higher cruising speed
static constexpr uint8_t SPEED_SLOW = 220;     // Faster precision mode
static constexpr uint8_t SPEED_FAST = 235;     // Higher fast speed
static constexpr uint8_t SPEED_BOOST = 245;    // Maximum boost
static constexpr uint8_t SPEED_MAX = 255;      // Maximum possible speed

// ====== Speed Control Parameters ======
static constexpr uint8_t ACCELERATION_STEP = 45;   // Increased acceleration
static constexpr uint8_t BRAKE_STEP = 90;         // Stronger braking
static constexpr uint8_t TURN_SPEED = 180;        // Faster turns
static constexpr uint8_t TURN_THRESHOLD = 35;     // Quicker turn detection
static constexpr uint8_t STRAIGHT_THRESHOLD = 12;  // Faster straight detection
static constexpr uint8_t BOOST_DURATION = 15;      // Longer boost
static constexpr uint8_t BOOST_INCREMENT = 35;     // Stronger boost

// ====== PID Control Parameters ======
static constexpr float K_PROPORTIONAL_DEFAULT = 7.5f;    // More aggressive proportional control
static constexpr float K_DERIVATIVE_DEFAULT = 900.0f;    // More aggressive derivative control
static constexpr float FILTER_COEFFICIENT_DEFAULT = 0.8f; // More responsive filtering

// Desired center position (don't change without recalibrating control)
static constexpr int16_t POSICION_IDEAL_DEFAULT = 0;

// ====== Marker Parameters ======
static constexpr int16_t MARKER_DETECTION_THRESHOLD = 180;
// Base speeds - Significantly increased
static constexpr uint8_t BASE_SLOW = 160;  // Increased from 115
static constexpr uint8_t BASE_FAST = 200;  // Increased from 115

// ====== Delays and Timings ======
// Optimized setup and calibration parameters
static const uint16_t SETUP_DELAY = 400;           // Reduced from 600
static const uint16_t CALIBRATION_SAMPLES = 300;   // Reduced from 400
static const uint8_t CALIBRATION_DELAY = 20;       // Reduced from 30
static const uint16_t STOP_DELAY = 200;            // Reduced from 300

// ====== Sensor Parameters ======
static const uint8_t NUM_SENSORES = 6;
static constexpr int16_t SENSOR_MAX_VALUE = 1023;
static constexpr int16_t SENSOR_MIN_VALUE = 0;
static constexpr int16_t SENSOR_THRESHOLD = 100;  // Reduced from 120 for faster response

// ====== Sensor Weights ======
// Optimized weights for better responsiveness
static constexpr float SENSOR_WEIGHT_S1 = -3.0f;  // Increased from -2.5f
static constexpr float SENSOR_WEIGHT_S2 = -1.5f;  // Increased from -1.2f
static constexpr float SENSOR_WEIGHT_S3 = -0.8f;  // Increased from -0.6f
static constexpr float SENSOR_WEIGHT_S4 = 0.8f;   // Increased from 0.6f
static constexpr float SENSOR_WEIGHT_S5 = 1.5f;   // Increased from 1.2f
static constexpr float SENSOR_WEIGHT_S6 = 3.0f;   // Increased from 2.5f

#endif // CONFIG_H