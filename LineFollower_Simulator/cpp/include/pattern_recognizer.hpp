/**
 * @file pattern_recognizer.hpp
 * @brief Pattern recognition system for track artifact identification
 *
 * Analyzes track geometry to identify known patterns (artifacts) that can be
 * optimized using analytical or specialized numerical methods.
 */

#ifndef PATTERN_RECOGNIZER_HPP
#define PATTERN_RECOGNIZER_HPP

#include <vector>
#include <string>
#include "simulator.hpp"

namespace LineFollower {

/**
 * @brief Artifact types
 */
enum class ArtifactType {
    UNKNOWN,
    STRAIGHT,
    CIRCULAR_CURVE,
    TRANSITION,           // Straight-Curve-Straight
    S_CURVE,
    CHICANE,
    HAIRPIN,
    SPIRAL,
    COMPLEX               // Requires numerical optimization
};

/**
 * @brief Artifact description
 */
struct Artifact {
    ArtifactType type;
    int startIndex;       // Index of first track point
    int endIndex;         // Index of last track point
    float length;         // Total length in meters
    float curvature;      // Average curvature (0 for straight)
    float radius;         // Radius for circular curves
    std::string description;
};

/**
 * @brief Pattern recognizer class
 */
class PatternRecognizer {
public:
    /**
     * @brief Constructor
     * @param tolerances Recognition tolerances
     */
    PatternRecognizer();

    /**
     * @brief Analyze track and identify artifacts
     * @param trackPoints Track definition
     * @return Vector of identified artifacts
     */
    std::vector<Artifact> recognizeArtifacts(
        const std::vector<TrackPoint>& trackPoints
    );

    /**
     * @brief Set recognition tolerances
     * @param straightTolerance Maximum curvature for straight detection
     * @param circleTolerance Maximum radius variation for circle detection
     */
    void setTolerances(float straightTolerance, float circleTolerance);

private:
    float straightTolerance_;
    float circleTolerance_;

    /**
     * @brief Check if segment is straight
     */
    bool isStraight(
        const std::vector<TrackPoint>& points,
        int start,
        int end
    ) const;

    /**
     * @brief Check if segment is circular curve
     */
    bool isCircularCurve(
        const std::vector<TrackPoint>& points,
        int start,
        int end,
        float& radius
    ) const;

    /**
     * @brief Check if segment is S-curve
     */
    bool isSCurve(
        const std::vector<TrackPoint>& points,
        int start,
        int end
    ) const;

    /**
     * @brief Check if segment is hairpin
     */
    bool isHairpin(
        const std::vector<TrackPoint>& points,
        int start,
        int end
    ) const;

    /**
     * @brief Calculate curvature at segment
     */
    float calculateSegmentCurvature(
        const std::vector<TrackPoint>& points,
        int index
    ) const;

    /**
     * @brief Calculate segment length
     */
    float calculateSegmentLength(
        const std::vector<TrackPoint>& points,
        int start,
        int end
    ) const;
};

} // namespace LineFollower

#endif // PATTERN_RECOGNIZER_HPP
