/**
 * @file artifact_base.hpp
 * @brief Base class for track artifacts with analytical optimization
 *
 * Defines interface for artifact-specific optimization strategies.
 * Each artifact type implements its own optimal strategy based on
 * racing theory and vehicle dynamics.
 */

#ifndef ARTIFACT_BASE_HPP
#define ARTIFACT_BASE_HPP

#include "../simulator.hpp"
#include "../pattern_recognizer.hpp"
#include <vector>

namespace LineFollower {
namespace Artifacts {

/**
 * @brief Strategy result for an artifact
 */
struct ArtifactStrategy {
    float entrySpeed;        // Optimal entry speed (m/s)
    float exitSpeed;         // Optimal exit speed (m/s)
    float maxSpeed;          // Maximum speed within artifact
    float averageSpeed;      // Average speed through artifact
    float estimatedTime;     // Estimated time to traverse
    std::string description; // Human-readable strategy description
};

/**
 * @brief Base class for artifact optimization
 */
class ArtifactBase {
public:
    /**
     * @brief Constructor
     * @param artifact Artifact description
     */
    explicit ArtifactBase(const Artifact& artifact)
        : artifact_(artifact) {}

    /**
     * @brief Virtual destructor
     */
    virtual ~ArtifactBase() = default;

    /**
     * @brief Calculate optimal strategy for this artifact
     * @param robotConfig Robot configuration
     * @param trackPoints Full track points
     * @param prevExitSpeed Speed from previous artifact
     * @return Optimal strategy
     */
    virtual ArtifactStrategy calculateOptimalStrategy(
        const RobotConfig& robotConfig,
        const std::vector<TrackPoint>& trackPoints,
        float prevExitSpeed
    ) = 0;

    /**
     * @brief Get artifact description
     */
    const Artifact& getArtifact() const { return artifact_; }

    /**
     * @brief Get artifact type
     */
    ArtifactType getType() const { return artifact_.type; }

protected:
    Artifact artifact_;

    /**
     * @brief Helper: Calculate maximum safe speed based on physics
     */
    float calculateMaxSafeSpeed(
        float curvature,
        const RobotConfig& config
    ) const;

    /**
     * @brief Helper: Calculate optimal racing line
     */
    std::vector<TrackPoint> calculateRacingLine(
        const std::vector<TrackPoint>& trackPoints,
        int startIndex,
        int endIndex
    ) const;
};

} // namespace Artifacts
} // namespace LineFollower

#endif // ARTIFACT_BASE_HPP
