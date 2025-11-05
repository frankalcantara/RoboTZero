/**
 * @file simulator.hpp
 * @brief Main simulator class - handles physics simulation and state management
 *
 * This class integrates Box2D physics engine to simulate realistic robot dynamics
 * including differential drive kinematics, sensor readings, and environmental factors.
 */

#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <vector>
#include <memory>

// Forward declarations
class b2World;
class b2Body;

namespace LineFollower {

/**
 * @brief Robot configuration structure
 */
struct RobotConfig {
    float mass;              // kg
    float wheelbase;         // meters
    float wheelDiameter;     // meters
    float maxSpeed;          // m/s
    int sensorCount;         // number of sensors
    float sensorSpacing;     // meters between sensors
    float sensorHeight;      // meters above ground

    // PID controller gains
    float kp, ki, kd;

    // Environmental factors
    float temperature;       // Celsius
    float frictionCoeff;     // surface friction
    float gravity;           // m/s²
};

/**
 * @brief Track point structure
 */
struct TrackPoint {
    float x, y;
};

/**
 * @brief Robot state at a given time
 */
struct RobotState {
    float posX, posY;        // position
    float velX, velY;        // velocity
    float heading;           // radians
    float angularVel;        // rad/s
    std::vector<float> sensorReadings;  // 0-1 values
    float leftMotor;         // PWM value
    float rightMotor;        // PWM value
    float lineError;         // current line following error
    float power;             // current power consumption (W)
    float time;              // simulation time
};

/**
 * @brief Main simulator class
 */
class Simulator {
public:
    /**
     * @brief Constructor
     * @param config Robot configuration
     * @param trackPoints Track definition points
     */
    Simulator(const RobotConfig& config, const std::vector<TrackPoint>& trackPoints);

    /**
     * @brief Destructor
     */
    ~Simulator();

    /**
     * @brief Initialize simulation
     * @return true if successful
     */
    bool initialize();

    /**
     * @brief Step simulation forward by timestep
     * @param dt Time step in seconds
     */
    void step(float dt);

    /**
     * @brief Reset simulation to initial state
     */
    void reset();

    /**
     * @brief Get current robot state
     * @return Current state
     */
    RobotState getCurrentState() const;

    /**
     * @brief Check if robot completed the track
     * @return true if completed
     */
    bool isComplete() const;

    /**
     * @brief Check if robot failed (off track, collision, etc.)
     * @return true if failed
     */
    bool hasFailed() const;

    /**
     * @brief Get completion time
     * @return Time in seconds, or -1 if not completed
     */
    float getCompletionTime() const;

    /**
     * @brief Update PID gains (for online tuning)
     * @param kp Proportional gain
     * @param ki Integral gain
     * @param kd Derivative gain
     */
    void updatePIDGains(float kp, float ki, float kd);

private:
    // Physics world
    std::unique_ptr<b2World> world_;

    // Robot body
    b2Body* robotBody_;

    // Configuration
    RobotConfig config_;
    std::vector<TrackPoint> trackPoints_;

    // State tracking
    RobotState currentState_;
    float simulationTime_;
    float completionTime_;
    bool isComplete_;
    bool hasFailed_;

    // PID controller state
    float prevError_;
    float errorIntegral_;

    /**
     * @brief Update sensor readings based on current position
     */
    void updateSensors();

    /**
     * @brief Calculate PID control output
     * @param error Current line error
     * @param dt Time step
     * @return Control output
     */
    float calculatePID(float error, float dt);

    /**
     * @brief Apply motor commands to robot
     * @param leftPower Left motor power (-1 to 1)
     * @param rightPower Right motor power (-1 to 1)
     */
    void applyMotorCommands(float leftPower, float rightPower);

    /**
     * @brief Calculate line following error
     * @return Error value
     */
    float calculateLineError();

    /**
     * @brief Check track completion
     */
    void checkCompletion();

    /**
     * @brief Check failure conditions
     */
    void checkFailure();
};

} // namespace LineFollower

#endif // SIMULATOR_HPP
