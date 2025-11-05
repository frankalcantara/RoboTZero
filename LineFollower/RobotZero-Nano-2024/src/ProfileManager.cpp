#include "ProfileManager.h"
#include "config.h"

#if DEBUG_LEVEL > 0

// Static member initialization
DebugMode ProfileManager::currentMode = DebugMode::NORMAL;
const SpeedProfile* ProfileManager::activeProfile = nullptr;

// Analysis mode profile
const SpeedProfile ProfileManager::ANALYSIS_PROFILE = {
    // Speed settings - Conservative for analysis
    .speedStop = 0,
    .speedStartup = 60,    // Slower startup
    .speedTurn = 80,       // Careful turns
    .speedBrake = 90,      // Gentle braking
    .speedCruise = 100,    // Moderate cruising
    .speedSlow = 120,      // Moderate slow speed
    .speedFast = 140,      // Moderate fast speed
    .speedBoost = 160,     // Moderate boost
    .speedMax = 180,       // Limited top speed

    // Control parameters - Smooth operation
    .accelerationStep = 15, // Gentle acceleration
    .brakeStep = 40,       // Moderate braking
    .turnSpeed = 80,       // Conservative turns
    .turnThreshold = 50,   // Earlier turn detection
    .straightThreshold = 25, // Stricter straight detection
    .boostDuration = 8,    // Short boost
    .boostIncrement = 15,  // Gentle boost

    // PID parameters - Stable control
    .kProportional = 4.0f,
    .kDerivative = 500.0f,
    .filterCoefficient = 0.5f
};

// Speed mode profile
const SpeedProfile ProfileManager::SPEED_PROFILE = {
    // Speed settings - Aggressive for performance
    .speedStop = 0,
    .speedStartup = 100,   // Quick startup
    .speedTurn = 120,      // Fast turns
    .speedBrake = 140,     // Strong braking
    .speedCruise = 160,    // Fast cruising
    .speedSlow = 180,      // Fast slow mode
    .speedFast = 200,      // High speed
    .speedBoost = 220,     // Strong boost
    .speedMax = 255,       // Maximum speed

    // Control parameters - Performance focused
    .accelerationStep = 35, // Quick acceleration
    .brakeStep = 70,       // Strong braking
    .turnSpeed = 140,      // Fast turns
    .turnThreshold = 40,   // Later turn detection
    .straightThreshold = 15, // Quicker straight detection
    .boostDuration = 12,   // Longer boost
    .boostIncrement = 25,  // Strong boost

    // PID parameters - Aggressive control
    .kProportional = 6.0f,
    .kDerivative = 700.0f,
    .filterCoefficient = 0.7f
};

void ProfileManager::initialize(DebugMode mode) {
    currentMode = mode;
    setActiveProfile(mode);
}

DebugMode ProfileManager::getCurrentMode() {
    return currentMode;
}

uint8_t ProfileManager::getSpeedValue(uint8_t defaultSpeed) {
    if (activeProfile == nullptr) {
        return defaultSpeed;
    }

    // Map original speed constants to profile values
    if (defaultSpeed == SPEED_STOP) return activeProfile->speedStop;
    if (defaultSpeed == SPEED_STARTUP) return activeProfile->speedStartup;
    if (defaultSpeed == SPEED_TURN) return activeProfile->speedTurn;
    if (defaultSpeed == SPEED_BRAKE) return activeProfile->speedBrake;
    if (defaultSpeed == SPEED_CRUISE) return activeProfile->speedCruise;
    if (defaultSpeed == SPEED_SLOW) return activeProfile->speedSlow;
    if (defaultSpeed == SPEED_FAST) return activeProfile->speedFast;
    if (defaultSpeed == SPEED_BOOST) return activeProfile->speedBoost;
    if (defaultSpeed == SPEED_MAX) return activeProfile->speedMax;

    return validateSpeed(defaultSpeed);
}

float ProfileManager::getKP(float defaultValue) {
    return activeProfile ? activeProfile->kProportional : defaultValue;
}

float ProfileManager::getKD(float defaultValue) {
    return activeProfile ? activeProfile->kDerivative : defaultValue;
}

float ProfileManager::getFilterCoefficient(float defaultValue) {
    return activeProfile ? activeProfile->filterCoefficient : defaultValue;
}

uint8_t ProfileManager::getAccelerationStep() {
    return activeProfile ? activeProfile->accelerationStep : ACCELERATION_STEP;
}

uint8_t ProfileManager::getBrakeStep() {
    return activeProfile ? activeProfile->brakeStep : BRAKE_STEP;
}

uint8_t ProfileManager::getTurnSpeed() {
    return activeProfile ? activeProfile->turnSpeed : TURN_SPEED;
}

uint8_t ProfileManager::getTurnThreshold() {
    return activeProfile ? activeProfile->turnThreshold : TURN_THRESHOLD;
}

uint8_t ProfileManager::getStraightThreshold() {
    return activeProfile ? activeProfile->straightThreshold : STRAIGHT_THRESHOLD;
}

uint8_t ProfileManager::getBoostDuration() {
    return activeProfile ? activeProfile->boostDuration : BOOST_DURATION;
}

uint8_t ProfileManager::getBoostIncrement() {
    return activeProfile ? activeProfile->boostIncrement : BOOST_INCREMENT;
}

const SpeedProfile* ProfileManager::getActiveProfile() {
    return activeProfile;
}

void ProfileManager::setActiveProfile(DebugMode mode) {
    switch (mode) {
    case DebugMode::ANALYSIS:
        activeProfile = &ANALYSIS_PROFILE;
        break;
    case DebugMode::SPEED:
        activeProfile = &SPEED_PROFILE;
        break;
    default:
        activeProfile = nullptr;
        break;
    }
}

uint8_t ProfileManager::validateSpeed(uint8_t speed) {
    return constrain(speed, 0, activeProfile ? activeProfile->speedMax : SPEED_MAX);
}

#endif // DEBUG_LEVEL > 0