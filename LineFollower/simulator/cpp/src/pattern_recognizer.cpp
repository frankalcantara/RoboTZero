/**
 * @file pattern_recognizer.cpp
 * @brief Implementation of pattern recognition system
 */

#include "../include/pattern_recognizer.hpp"
#include "../include/physics.hpp"

namespace LineFollower {

PatternRecognizer::PatternRecognizer()
    : straightTolerance_(0.01f)
    , circleTolerance_(0.1f)
{
}

std::vector<Artifact> PatternRecognizer::recognizeArtifacts(
    const std::vector<TrackPoint>& trackPoints)
{
    std::vector<Artifact> artifacts;

    // TODO: Implement full pattern recognition in Phase 2
    // For Phase 1, create a single complex artifact for the entire track

    if (trackPoints.size() < 3) {
        return artifacts;
    }

    Artifact fullTrack;
    fullTrack.type = ArtifactType::COMPLEX;
    fullTrack.startIndex = 0;
    fullTrack.endIndex = static_cast<int>(trackPoints.size()) - 1;
    fullTrack.length = calculateSegmentLength(trackPoints, 0, fullTrack.endIndex);
    fullTrack.curvature = 0.0f; // Will be calculated properly later
    fullTrack.radius = 0.0f;
    fullTrack.description = "Full track (Phase 1 - no decomposition yet)";

    artifacts.push_back(fullTrack);

    return artifacts;
}

void PatternRecognizer::setTolerances(float straightTolerance, float circleTolerance) {
    straightTolerance_ = straightTolerance;
    circleTolerance_ = circleTolerance;
}

bool PatternRecognizer::isStraight(
    const std::vector<TrackPoint>& points,
    int start,
    int end) const
{
    // TODO: Implement in Phase 2
    return false;
}

bool PatternRecognizer::isCircularCurve(
    const std::vector<TrackPoint>& points,
    int start,
    int end,
    float& radius) const
{
    // TODO: Implement in Phase 2
    radius = 0.0f;
    return false;
}

bool PatternRecognizer::isSCurve(
    const std::vector<TrackPoint>& points,
    int start,
    int end) const
{
    // TODO: Implement in Phase 2
    return false;
}

bool PatternRecognizer::isHairpin(
    const std::vector<TrackPoint>& points,
    int start,
    int end) const
{
    // TODO: Implement in Phase 2
    return false;
}

float PatternRecognizer::calculateSegmentCurvature(
    const std::vector<TrackPoint>& points,
    int index) const
{
    if (index <= 0 || index >= static_cast<int>(points.size()) - 1) {
        return 0.0f;
    }

    Physics::Vec2 prev(points[index - 1].x, points[index - 1].y);
    Physics::Vec2 curr(points[index].x, points[index].y);
    Physics::Vec2 next(points[index + 1].x, points[index + 1].y);

    return Physics::calculateCurvature(prev, curr, next);
}

float PatternRecognizer::calculateSegmentLength(
    const std::vector<TrackPoint>& points,
    int start,
    int end) const
{
    float length = 0.0f;

    for (int i = start; i < end; i++) {
        Physics::Vec2 p1(points[i].x, points[i].y);
        Physics::Vec2 p2(points[i + 1].x, points[i + 1].y);
        length += Physics::distance(p1, p2);
    }

    return length;
}

} // namespace LineFollower
