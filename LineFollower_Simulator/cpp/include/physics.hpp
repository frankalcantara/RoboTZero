/**
 * @file physics.hpp
 * @brief Physics utilities and helper functions
 *
 * Provides helper functions for physics calculations, coordinate transformations,
 * and Box2D integration utilities.
 */

#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <cmath>

namespace LineFollower {
namespace Physics {

/**
 * @brief Constants
 */
constexpr float PI = 3.14159265359f;
constexpr float DEG_TO_RAD = PI / 180.0f;
constexpr float RAD_TO_DEG = 180.0f / PI;

/**
 * @brief 2D vector structure
 */
struct Vec2 {
    float x, y;

    Vec2() : x(0), y(0) {}
    Vec2(float x_, float y_) : x(x_), y(y_) {}

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vec2 normalized() const {
        float len = length();
        if (len > 0.0001f) {
            return Vec2(x / len, y / len);
        }
        return Vec2(0, 0);
    }

    Vec2 operator+(const Vec2& other) const {
        return Vec2(x + other.x, y + other.y);
    }

    Vec2 operator-(const Vec2& other) const {
        return Vec2(x - other.x, y - other.y);
    }

    Vec2 operator*(float scalar) const {
        return Vec2(x * scalar, y * scalar);
    }

    float dot(const Vec2& other) const {
        return x * other.x + y * other.y;
    }

    float cross(const Vec2& other) const {
        return x * other.y - y * other.x;
    }
};

/**
 * @brief Calculate distance between two points
 */
inline float distance(const Vec2& a, const Vec2& b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

/**
 * @brief Clamp value between min and max
 */
template <typename T>
inline T clamp(T value, T min, T max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

/**
 * @brief Linear interpolation
 */
inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

/**
 * @brief Normalize angle to [-PI, PI]
 */
inline float normalizeAngle(float angle) {
    while (angle > PI) angle -= 2.0f * PI;
    while (angle < -PI) angle += 2.0f * PI;
    return angle;
}

/**
 * @brief Calculate shortest angular difference
 */
inline float angleDifference(float from, float to) {
    float diff = to - from;
    return normalizeAngle(diff);
}

/**
 * @brief Calculate curvature at a point on a path
 * @param prev Previous point
 * @param curr Current point
 * @param next Next point
 * @return Curvature (1/radius)
 */
float calculateCurvature(const Vec2& prev, const Vec2& curr, const Vec2& next);

/**
 * @brief Calculate optimal speed for a curve based on friction and curvature
 * @param radius Curve radius (meters)
 * @param friction Friction coefficient
 * @param gravity Gravity (m/s²)
 * @return Maximum safe speed (m/s)
 */
inline float optimalCurveSpeed(float radius, float friction, float gravity) {
    return std::sqrt(friction * gravity * radius);
}

/**
 * @brief Calculate braking distance
 * @param initialSpeed Initial speed (m/s)
 * @param finalSpeed Final speed (m/s)
 * @param maxDeceleration Maximum deceleration (m/s²)
 * @return Braking distance (meters)
 */
inline float brakingDistance(float initialSpeed, float finalSpeed, float maxDeceleration) {
    return (initialSpeed * initialSpeed - finalSpeed * finalSpeed) / (2.0f * maxDeceleration);
}

/**
 * @brief Calculate motor power needed for differential drive
 * @param linearVel Desired linear velocity
 * @param angularVel Desired angular velocity
 * @param wheelbase Distance between wheels
 * @param[out] leftPower Left motor power
 * @param[out] rightPower Right motor power
 */
void calculateDifferentialDrive(
    float linearVel,
    float angularVel,
    float wheelbase,
    float& leftPower,
    float& rightPower
);

/**
 * @brief Apply environmental factors to friction
 * @param baseFriction Base friction coefficient
 * @param temperature Temperature in Celsius
 * @return Adjusted friction coefficient
 */
inline float adjustFrictionForTemperature(float baseFriction, float temperature) {
    // Simple model: friction decreases slightly with temperature
    const float tempEffect = 0.001f;
    return baseFriction * (1.0f - tempEffect * (temperature - 20.0f));
}

} // namespace Physics
} // namespace LineFollower

#endif // PHYSICS_HPP
