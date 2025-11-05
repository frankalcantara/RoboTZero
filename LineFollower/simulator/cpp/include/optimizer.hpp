/**
 * @file optimizer.hpp
 * @brief Optimization system for finding optimal robot configurations
 *
 * Implements artifact-based hierarchical decomposition approach:
 * 1. Pattern recognition to identify track artifacts
 * 2. Analytical solutions for known patterns
 * 3. Numerical optimization for complex sections
 * 4. Transition velocity optimization
 */

#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

#include <vector>
#include <memory>
#include <functional>
#include "simulator.hpp"

namespace LineFollower {

/**
 * @brief Optimization parameters
 */
struct OptimizationParams {
    int maxIterations;       // Maximum optimization iterations
    float tolerance;         // Convergence tolerance
    float learningRate;      // For gradient-based methods
    bool useAnalytical;      // Use analytical solutions when possible
    bool useNumerical;       // Use numerical optimization for complex sections
    int populationSize;      // For stochastic methods (if used)
};

/**
 * @brief Optimization result
 */
struct OptimizationResult {
    RobotConfig optimalConfig;
    float fitnessScore;      // 0-1, higher is better
    float completionTime;
    float averageSpeed;
    int iterations;
    bool converged;
    std::string strategy;    // Description of strategy applied
};

/**
 * @brief Main optimizer class
 */
class Optimizer {
public:
    /**
     * @brief Constructor
     * @param params Optimization parameters
     */
    explicit Optimizer(const OptimizationParams& params);

    /**
     * @brief Destructor
     */
    ~Optimizer();

    /**
     * @brief Optimize robot configuration for given track
     * @param initialConfig Starting configuration
     * @param trackPoints Track definition
     * @param progressCallback Optional callback for progress updates (0-100)
     * @return Optimization result
     */
    OptimizationResult optimize(
        const RobotConfig& initialConfig,
        const std::vector<TrackPoint>& trackPoints,
        std::function<void(float)> progressCallback = nullptr
    );

    /**
     * @brief Optimize only PID gains (faster than full optimization)
     * @param config Robot configuration with fixed physical parameters
     * @param trackPoints Track definition
     * @return Optimized configuration
     */
    RobotConfig optimizePID(
        const RobotConfig& config,
        const std::vector<TrackPoint>& trackPoints
    );

    /**
     * @brief Cancel ongoing optimization
     */
    void cancel();

private:
    OptimizationParams params_;
    bool cancelled_;

    /**
     * @brief Evaluate fitness of a configuration
     * @param config Configuration to evaluate
     * @param trackPoints Track definition
     * @return Fitness score (0-1)
     */
    float evaluateFitness(
        const RobotConfig& config,
        const std::vector<TrackPoint>& trackPoints
    );

    /**
     * @brief Run single simulation and extract metrics
     * @param config Configuration
     * @param trackPoints Track definition
     * @return Simulation result metrics
     */
    struct SimulationMetrics {
        float completionTime;
        float averageSpeed;
        float trackErrors;
        float energyConsumption;
        bool completed;
    };

    SimulationMetrics runSimulation(
        const RobotConfig& config,
        const std::vector<TrackPoint>& trackPoints
    );

    /**
     * @brief Gradient descent optimization
     */
    OptimizationResult gradientDescent(
        const RobotConfig& initialConfig,
        const std::vector<TrackPoint>& trackPoints,
        std::function<void(float)> progressCallback
    );

    /**
     * @brief Calculate numerical gradient
     * @param config Current configuration
     * @param trackPoints Track definition
     * @return Gradient vector
     */
    std::vector<float> calculateGradient(
        const RobotConfig& config,
        const std::vector<TrackPoint>& trackPoints
    );
};

} // namespace LineFollower

#endif // OPTIMIZER_HPP
