#include <Arduino.h>

// Debug system includes - must come first
#if DEBUG_LEVEL > 0
#include "DataStructures.h"
#include "ProfileManager.h"
#include "FlashManager.h"
#include "Logger.h"
#include "LedPattern.h"
#endif

// Core system includes
#include "config.h"
#include "debug.h"
#include "Timer.h"
#include "globals.h"
#include "MotorsDrivers.h"
#include "Sensors.h"
#include "Peripherals.h"
#include "CourseMarkers.h"

// Global variables initialization
int currentSpeed = 0;
bool isRobotStopped = false;
bool isStopSequenceActive = false;
int lapCount = 0;
bool isPrecisionMode = false;

// Setup states
enum SetupState {
    SETUP_INIT,
    SETUP_WAIT1,
    SETUP_WAIT2,
    SETUP_BUTTON1,
    SETUP_CALIBRATION,
    SETUP_BUTTON2,
    SETUP_COMPLETE
};

#if DEBUG_LEVEL > 0
DebugMode currentDebugMode = (DEBUG_LEVEL == 1) ? DebugMode::ANALYSIS : DebugMode::SPEED;
uint8_t plannedLaps = (DEBUG_LEVEL == 1) ? DEBUG_LAPS_MODE1 : DEBUG_LAPS_MODE2;
#endif

// Control parameters
int targetLinePosition = POSICION_IDEAL_DEFAULT;
float kProportional = K_PROPORTIONAL_DEFAULT;
float kDerivative = K_DERIVATIVE_DEFAULT;
float filterCoefficient = FILTER_COEFFICIENT_DEFAULT;
int filteredErrorRate = 0;
int previousError = 0;

void setup() {
    // Initialize serial if in debug mode
#if DEBUG_LEVEL > 0
    Serial.begin(115200);
    // Initialize profile manager with appropriate mode
    ProfileManager::initialize(currentDebugMode);

    // Update control parameters from profile
    kProportional = ProfileManager::getKP(K_PROPORTIONAL_DEFAULT);
    kDerivative = ProfileManager::getKD(K_DERIVATIVE_DEFAULT);
    filterCoefficient = ProfileManager::getFilterCoefficient(FILTER_COEFFICIENT_DEFAULT);

    // Initialize logger and flash
    FlashManager::initialize();
    Logger::initialize();
#endif

    DEBUG_PRINT(DEBUG_SETUP_START);

    // Hardware initialization
    Peripherals::initialize();
    MotorDriver::initializeMotorDriver();
    pinMode(PIN_STATUS_LED, OUTPUT);

    // Non-blocking setup loop
    SetupState setupState = SETUP_INIT;
    Timer setupTimer;

    while (setupState != SETUP_COMPLETE) {
        switch (setupState) {
        case SETUP_INIT:
            setupTimer.Start(SETUP_DELAY);
            setupState = SETUP_WAIT1;
            break;

        case SETUP_WAIT1:
            if (setupTimer.Expired()) {
                digitalWrite(PIN_STATUS_LED, LOW);
                MotorDriver::setMotorsPower(0, 0);
                setupTimer.Start(SETUP_DELAY);
                setupState = SETUP_WAIT2;
            }
            break;

        case SETUP_WAIT2:
            if (setupTimer.Expired()) {
                setupState = SETUP_BUTTON1;
            }
            break;

        case SETUP_BUTTON1:
            Peripherals::waitForButtonPress();
            digitalWrite(PIN_STATUS_LED, HIGH);
            setupState = SETUP_CALIBRATION;
            break;

        case SETUP_CALIBRATION:
            Sensors::calibration();
            digitalWrite(PIN_STATUS_LED, LOW);
            setupState = SETUP_BUTTON2;
            break;

        case SETUP_BUTTON2:
            Peripherals::waitForButtonPress();
            digitalWrite(PIN_STATUS_LED, HIGH);
            setupState = SETUP_COMPLETE;
            break;

        default:
            break;
        }
    }

    // Initialize control variables
    previousError = 0;
    filteredErrorRate = 0;

    // Set initial speed based on mode
#if DEBUG_LEVEL > 0
    currentSpeed = ProfileManager::getSpeedValue(BASE_FAST);
    // Start logging session
    Logger::startSession(currentDebugMode, plannedLaps);
#else
    currentSpeed = BASE_FAST;
#endif

    lapCount = 0;
    DEBUG_PRINTLN(DEBUG_SETUP_COMPLETE);
}

void loop() {
#if DEBUG_LEVEL > 0
    LedPattern::process();
#endif

    // Process marker signals with optimized timing
    CourseMarkers::processMarkerSignals();

    // Skip control if robot is stopped
    if (isRobotStopped) {
        MotorDriver::setMotorsPower(0, 0);
#if DEBUG_LEVEL > 0
        if (Logger::isLogging()) {
            Logger::endSession();
        }
#endif
        return;
    }

    // Get current position and calculate error
    int linePosition = Sensors::calculateLinePosition();
    int error = linePosition - targetLinePosition;

    // Update current speed using new control interface
#if DEBUG_LEVEL > 0
    currentSpeed = ProfileManager::getSpeedValue(CourseMarkers::speedControl(error));
#else
    currentSpeed = CourseMarkers::speedControl(error);
#endif

    // Calculate error derivative and filter
    int d_error = error - previousError;
    filteredErrorRate = filterCoefficient * d_error + (1 - filterCoefficient) * filteredErrorRate;

    // Calculate PID correction
    float proportional_term = kProportional * error;
    float derivative_term = kDerivative * filteredErrorRate;
    float total_correction = proportional_term + derivative_term;

    // Adjust correction gain based on speed
    float speed_factor = (currentSpeed > 200) ? 0.85f : 1.0f;
    int correction_power = constrain(int(total_correction * speed_factor), -255, 255);

    // Apply correction to motors
    int left_power = constrain(currentSpeed + correction_power, -255, 255);
    int right_power = constrain(currentSpeed - correction_power, -255, 255);

    MotorDriver::setMotorsPower(left_power, right_power);
    previousError = error;

#if DEBUG_LEVEL > 0
    // Log performance data
    if (Logger::isLogging()) {
        uint8_t state = 0;
        if (isPrecisionMode) state |= 0x01;
        if (abs(error) > TURN_THRESHOLD) state |= 0x02;

        Logger::logPerformance(linePosition, error, correction_power,
            left_power, right_power, state);

        // Process logger (handles buffer flushing)
        Logger::process();
    }

    // Debug output
    DEBUG_PRINT(DEBUG_BASE);
    DEBUG_PRINT_VAL(currentSpeed);
    DEBUG_PRINT(DEBUG_ERROR);
    DEBUG_PRINT_VAL(error);
    DEBUG_PRINT(DEBUG_CORRECTION);
    DEBUG_PRINTLN_VAL(correction_power);
#endif
}   