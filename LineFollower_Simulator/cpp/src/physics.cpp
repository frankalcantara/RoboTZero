/**
 * @file physics.cpp
 * @brief Implementation of physics utilities
 */

#include "../include/physics.hpp"

namespace LineFollower {
namespace Physics {

float calculateCurvature(const Vec2& prev, const Vec2& curr, const Vec2& next) {
    // Calculate curvature using Menger curvature formula
    Vec2 v1 = curr - prev;
    Vec2 v2 = next - curr;

    float area = std::abs(v1.cross(v2)) / 2.0f;
    float a = v1.length();
    float b = v2.length();
    float c = distance(prev, next);

    if (a * b * c < 0.0001f) {
        return 0.0f;
    }

    return 4.0f * area / (a * b * c);
}

void calculateDifferentialDrive(
    float linearVel,
    float angularVel,
    float wheelbase,
    float& leftPower,
    float& rightPower)
{
    // Differential drive kinematics
    // v_left = v - (L/2) * omega
    // v_right = v + (L/2) * omega

    float halfWheelbase = wheelbase / 2.0f;

    leftPower = linearVel - halfWheelbase * angularVel;
    rightPower = linearVel + halfWheelbase * angularVel;

    // Normalize to [-1, 1]
    float maxVal = std::max(std::abs(leftPower), std::abs(rightPower));
    if (maxVal > 1.0f) {
        leftPower /= maxVal;
        rightPower /= maxVal;
    }
}

} // namespace Physics
} // namespace LineFollower
