/**
 * @file simulator.cpp
 * @brief Implementation of main simulator class
 */

#include "../include/simulator.hpp"
#include "../include/physics.hpp"
// #include <box2d/box2d.h> // Will be included when Box2D is integrated

namespace LineFollower {

Simulator::Simulator(const RobotConfig& config, const std::vector<TrackPoint>& trackPoints)
    : robotBody_(nullptr)
    , config_(config)
    , trackPoints_(trackPoints)
    , simulationTime_(0.0f)
    , completionTime_(-1.0f)
    , isComplete_(false)
    , hasFailed_(false)
    , prevError_(0.0f)
    , errorIntegral_(0.0f)
{
}

Simulator::~Simulator() {
    // Cleanup will be implemented with Box2D integration
}

bool Simulator::initialize() {
    // TODO: Initialize Box2D world in Phase 1
    // world_ = std::make_unique<b2World>(b2Vec2(0.0f, -config_.gravity));

    // TODO: Create robot body with proper physics
    // TODO: Create track boundaries
    // TODO: Initialize sensors

    // Placeholder initialization
    currentState_.posX = 0.0f;
    currentState_.posY = 0.0f;
    currentState_.velX = 0.0f;
    currentState_.velY = 0.0f;
    currentState_.heading = 0.0f;
    currentState_.angularVel = 0.0f;
    currentState_.sensorReadings.resize(config_.sensorCount, 0.0f);
    currentState_.leftMotor = 0.0f;
    currentState_.rightMotor = 0.0f;
    currentState_.lineError = 0.0f;
    currentState_.power = 0.0f;
    currentState_.time = 0.0f;

    return true;
}

void Simulator::step(float dt) {
    // TODO: Implement full physics step with Box2D

    // Placeholder implementation
    simulationTime_ += dt;
    currentState_.time = simulationTime_;

    // Update sensors
    updateSensors();

    // Calculate line error
    float error = calculateLineError();

    // Calculate PID control
    float control = calculatePID(error, dt);

    // Apply motor commands
    float leftPower = Physics::clamp(control + 0.5f, 0.0f, 1.0f);
    float rightPower = Physics::clamp(-control + 0.5f, 0.0f, 1.0f);
    applyMotorCommands(leftPower, rightPower);

    // Check completion and failure
    checkCompletion();
    checkFailure();
}

void Simulator::reset() {
    simulationTime_ = 0.0f;
    completionTime_ = -1.0f;
    isComplete_ = false;
    hasFailed_ = false;
    prevError_ = 0.0f;
    errorIntegral_ = 0.0f;

    // Reset state
    currentState_.posX = 0.0f;
    currentState_.posY = 0.0f;
    currentState_.velX = 0.0f;
    currentState_.velY = 0.0f;
    currentState_.heading = 0.0f;
    currentState_.angularVel = 0.0f;
    std::fill(currentState_.sensorReadings.begin(), currentState_.sensorReadings.end(), 0.0f);
    currentState_.leftMotor = 0.0f;
    currentState_.rightMotor = 0.0f;
    currentState_.lineError = 0.0f;
    currentState_.power = 0.0f;
    currentState_.time = 0.0f;
}

RobotState Simulator::getCurrentState() const {
    return currentState_;
}

bool Simulator::isComplete() const {
    return isComplete_;
}

bool Simulator::hasFailed() const {
    return hasFailed_;
}

float Simulator::getCompletionTime() const {
    return completionTime_;
}

void Simulator::updatePIDGains(float kp, float ki, float kd) {
    config_.kp = kp;
    config_.ki = ki;
    config_.kd = kd;

    // Reset PID state
    prevError_ = 0.0f;
    errorIntegral_ = 0.0f;
}

void Simulator::updateSensors() {
    // TODO: Implement actual sensor simulation with ray casting
    // Placeholder: simulate some sensor readings
    for (size_t i = 0; i < currentState_.sensorReadings.size(); i++) {
        currentState_.sensorReadings[i] = 0.5f; // Neutral reading
    }
}

float Simulator::calculatePID(float error, float dt) {
    // Proportional
    float P = config_.kp * error;

    // Integral with anti-windup
    errorIntegral_ += error * dt;
    errorIntegral_ = Physics::clamp(errorIntegral_, -1.0f, 1.0f);
    float I = config_.ki * errorIntegral_;

    // Derivative
    float D = config_.kd * (error - prevError_) / dt;
    prevError_ = error;

    return P + I + D;
}

void Simulator::applyMotorCommands(float leftPower, float rightPower) {
    // TODO: Apply forces/torques to Box2D body
    currentState_.leftMotor = leftPower;
    currentState_.rightMotor = rightPower;

    // Calculate power consumption (simplified)
    currentState_.power = (leftPower + rightPower) * 12.0f; // Assuming 12V system
}

float Simulator::calculateLineError() {
    // TODO: Calculate actual error based on sensor readings
    // Placeholder: weighted sum of sensor readings
    float error = 0.0f;
    int centerIndex = config_.sensorCount / 2;

    for (int i = 0; i < config_.sensorCount; i++) {
        float weight = static_cast<float>(i - centerIndex);
        error += weight * currentState_.sensorReadings[i];
    }

    currentState_.lineError = error;
    return error;
}

void Simulator::checkCompletion() {
    // TODO: Implement actual completion check
    // For now, complete after 60 seconds
    if (simulationTime_ > 60.0f && !isComplete_) {
        isComplete_ = true;
        completionTime_ = simulationTime_;
    }
}

void Simulator::checkFailure() {
    // TODO: Implement failure detection (off track, collision, timeout)
    // Placeholder: fail if timeout exceeds 120 seconds
    if (simulationTime_ > 120.0f && !isComplete_) {
        hasFailed_ = true;
    }
}

} // namespace LineFollower
