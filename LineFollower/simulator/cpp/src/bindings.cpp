/**
 * @file bindings.cpp
 * @brief Embind bindings for JavaScript/WebAssembly interface
 *
 * Exposes C++ classes and functions to JavaScript using Emscripten's Embind.
 * This is the bridge between the Svelte frontend and C++ simulation engine.
 */

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "../include/simulator.hpp"
#include "../include/optimizer.hpp"
#include "../include/pattern_recognizer.hpp"
#include <vector>

using namespace emscripten;
using namespace LineFollower;

/**
 * @brief JavaScript-friendly wrapper for simulator
 */
class SimulatorWrapper {
public:
    SimulatorWrapper() : simulator_(nullptr) {}

    /**
     * @brief Initialize simulator with configuration
     */
    bool initialize(val configObj, val trackObj) {
        // Parse robot configuration from JavaScript object
        RobotConfig config;
        config.mass = configObj["mass"].as<float>();
        config.wheelbase = configObj["wheelbase"].as<float>();
        config.wheelDiameter = configObj["wheelDiameter"].as<float>();
        config.maxSpeed = configObj["maxSpeed"].as<float>();
        config.sensorCount = configObj["sensors"]["count"].as<int>();
        config.sensorSpacing = configObj["sensors"]["spacing"].as<float>();
        config.sensorHeight = configObj["sensors"]["height"].as<float>();
        config.kp = configObj["pid"]["kp"].as<float>();
        config.ki = configObj["pid"]["ki"].as<float>();
        config.kd = configObj["pid"]["kd"].as<float>();
        config.temperature = configObj["environment"]["temperature"].as<float>();
        config.frictionCoeff = configObj["environment"]["friction"].as<float>();
        config.gravity = configObj["environment"]["gravity"].as<float>();

        // Parse track points from JavaScript array
        std::vector<TrackPoint> trackPoints;
        val pointsArray = trackObj["points"];
        int numPoints = pointsArray["length"].as<int>();

        for (int i = 0; i < numPoints; i++) {
            val point = pointsArray[i];
            TrackPoint tp;
            tp.x = point["x"].as<float>();
            tp.y = point["y"].as<float>();
            trackPoints.push_back(tp);
        }

        // Create simulator
        simulator_ = std::make_unique<Simulator>(config, trackPoints);
        return simulator_->initialize();
    }

    /**
     * @brief Step simulation
     */
    void step(float dt) {
        if (simulator_) {
            simulator_->step(dt);
        }
    }

    /**
     * @brief Reset simulation
     */
    void reset() {
        if (simulator_) {
            simulator_->reset();
        }
    }

    /**
     * @brief Get current state as JavaScript object
     */
    val getCurrentState() {
        if (!simulator_) {
            return val::object();
        }

        RobotState state = simulator_->getCurrentState();

        // Create JavaScript object
        val stateObj = val::object();
        stateObj.set("posX", state.posX);
        stateObj.set("posY", state.posY);
        stateObj.set("velX", state.velX);
        stateObj.set("velY", state.velY);
        stateObj.set("heading", state.heading);
        stateObj.set("angularVel", state.angularVel);
        stateObj.set("leftMotor", state.leftMotor);
        stateObj.set("rightMotor", state.rightMotor);
        stateObj.set("lineError", state.lineError);
        stateObj.set("power", state.power);
        stateObj.set("time", state.time);

        // Sensor readings as array
        val sensorsArray = val::array();
        for (size_t i = 0; i < state.sensorReadings.size(); i++) {
            sensorsArray.set(i, state.sensorReadings[i]);
        }
        stateObj.set("sensors", sensorsArray);

        return stateObj;
    }

    /**
     * @brief Check if simulation is complete
     */
    bool isComplete() const {
        return simulator_ ? simulator_->isComplete() : false;
    }

    /**
     * @brief Check if simulation failed
     */
    bool hasFailed() const {
        return simulator_ ? simulator_->hasFailed() : false;
    }

    /**
     * @brief Get completion time
     */
    float getCompletionTime() const {
        return simulator_ ? simulator_->getCompletionTime() : -1.0f;
    }

    /**
     * @brief Update PID gains
     */
    void updatePIDGains(float kp, float ki, float kd) {
        if (simulator_) {
            simulator_->updatePIDGains(kp, ki, kd);
        }
    }

private:
    std::unique_ptr<Simulator> simulator_;
};

/**
 * @brief JavaScript-friendly wrapper for optimizer
 */
class OptimizerWrapper {
public:
    OptimizerWrapper() {
        OptimizationParams params;
        params.maxIterations = 100;
        params.tolerance = 0.001f;
        params.learningRate = 0.01f;
        params.useAnalytical = true;
        params.useNumerical = true;
        params.populationSize = 50;

        optimizer_ = std::make_unique<Optimizer>(params);
    }

    /**
     * @brief Optimize configuration
     */
    val optimize(val configObj, val trackObj) {
        // Parse configuration (similar to SimulatorWrapper)
        RobotConfig config;
        config.mass = configObj["mass"].as<float>();
        config.wheelbase = configObj["wheelbase"].as<float>();
        config.wheelDiameter = configObj["wheelDiameter"].as<float>();
        config.maxSpeed = configObj["maxSpeed"].as<float>();
        config.sensorCount = configObj["sensors"]["count"].as<int>();
        config.sensorSpacing = configObj["sensors"]["spacing"].as<float>();
        config.sensorHeight = configObj["sensors"]["height"].as<float>();
        config.kp = configObj["pid"]["kp"].as<float>();
        config.ki = configObj["pid"]["ki"].as<float>();
        config.kd = configObj["pid"]["kd"].as<float>();
        config.temperature = configObj["environment"]["temperature"].as<float>();
        config.frictionCoeff = configObj["environment"]["friction"].as<float>();
        config.gravity = configObj["environment"]["gravity"].as<float>();

        // Parse track points
        std::vector<TrackPoint> trackPoints;
        val pointsArray = trackObj["points"];
        int numPoints = pointsArray["length"].as<int>();

        for (int i = 0; i < numPoints; i++) {
            val point = pointsArray[i];
            TrackPoint tp;
            tp.x = point["x"].as<float>();
            tp.y = point["y"].as<float>();
            trackPoints.push_back(tp);
        }

        // Run optimization
        OptimizationResult result = optimizer_->optimize(config, trackPoints);

        // Convert result to JavaScript object
        val resultObj = val::object();
        resultObj.set("fitnessScore", result.fitnessScore);
        resultObj.set("completionTime", result.completionTime);
        resultObj.set("averageSpeed", result.averageSpeed);
        resultObj.set("iterations", result.iterations);
        resultObj.set("converged", result.converged);
        resultObj.set("strategy", result.strategy);

        // Optimal configuration
        val optimalConfigObj = val::object();
        optimalConfigObj.set("kp", result.optimalConfig.kp);
        optimalConfigObj.set("ki", result.optimalConfig.ki);
        optimalConfigObj.set("kd", result.optimalConfig.kd);
        resultObj.set("optimalConfig", optimalConfigObj);

        return resultObj;
    }

    /**
     * @brief Cancel optimization
     */
    void cancel() {
        if (optimizer_) {
            optimizer_->cancel();
        }
    }

private:
    std::unique_ptr<Optimizer> optimizer_;
};

/**
 * @brief Embind bindings
 */
EMSCRIPTEN_BINDINGS(line_follower_simulator) {
    // Simulator wrapper
    class_<SimulatorWrapper>("Simulator")
        .constructor<>()
        .function("initialize", &SimulatorWrapper::initialize)
        .function("step", &SimulatorWrapper::step)
        .function("reset", &SimulatorWrapper::reset)
        .function("getCurrentState", &SimulatorWrapper::getCurrentState)
        .function("isComplete", &SimulatorWrapper::isComplete)
        .function("hasFailed", &SimulatorWrapper::hasFailed)
        .function("getCompletionTime", &SimulatorWrapper::getCompletionTime)
        .function("updatePIDGains", &SimulatorWrapper::updatePIDGains);

    // Optimizer wrapper
    class_<OptimizerWrapper>("Optimizer")
        .constructor<>()
        .function("optimize", &OptimizerWrapper::optimize)
        .function("cancel", &OptimizerWrapper::cancel);
}
