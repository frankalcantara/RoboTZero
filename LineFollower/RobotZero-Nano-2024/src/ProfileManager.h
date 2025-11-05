#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H

#include <Arduino.h>
#include "DataStructures.h"

#if DEBUG_LEVEL > 0

class ProfileManager {
public:
    // Initialize profile manager
    static void initialize(DebugMode mode);

    // Get current debug mode
    static DebugMode getCurrentMode();

    // Get speed value based on original speed constant
    static uint8_t getSpeedValue(uint8_t defaultSpeed);

    // Get PID parameters
    static float getKP(float defaultValue);
    static float getKD(float defaultValue);
    static float getFilterCoefficient(float defaultValue);

    // Get acceleration parameters
    static uint8_t getAccelerationStep();
    static uint8_t getBrakeStep();
    static uint8_t getTurnSpeed();
    static uint8_t getTurnThreshold();
    static uint8_t getStraightThreshold();
    static uint8_t getBoostDuration();
    static uint8_t getBoostIncrement();

    // Get active profile
    static const SpeedProfile* getActiveProfile();

private:
    static DebugMode currentMode;
    static const SpeedProfile* activeProfile;

    // Predefined profiles
    static const SpeedProfile ANALYSIS_PROFILE;
    static const SpeedProfile SPEED_PROFILE;

    // Private methods
    static void setActiveProfile(DebugMode mode);
    static uint8_t validateSpeed(uint8_t speed);
};

#endif // DEBUG_LEVEL > 0
#endif // PROFILEMANAGER_H