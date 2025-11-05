#include "Logger.h"
#include "CircularBuffer.h"
#include "Sensors.h"
#include "ProfileManager.h"  // Added include for ProfileManager

#if DEBUG_LEVEL > 0

// Define buffer sizes
static constexpr uint8_t PERFORMANCE_BUFFER_SIZE = 32;
static constexpr uint8_t EVENT_BUFFER_SIZE = 16;
static constexpr uint8_t STATS_BUFFER_SIZE = 8;

// Buffers for different record types
static CircularBuffer<PerformanceRecord, PERFORMANCE_BUFFER_SIZE> performanceBuffer;
static CircularBuffer<EventRecord, EVENT_BUFFER_SIZE> eventBuffer;
static CircularBuffer<LapStats, STATS_BUFFER_SIZE> statsBuffer;

// Static member initialization
bool Logger::isInitialized = false;
bool Logger::loggingActive = false;
uint32_t Logger::sessionStartTime = 0;
uint8_t Logger::currentLap = 0;
uint16_t Logger::curveCount = 0;
uint32_t Logger::lastSampleTime = 0;
uint32_t Logger::lastFlushTime = 0;
float Logger::totalDeviation = 0;
uint32_t Logger::sampleCount = 0;
uint8_t Logger::maxSpeed = 0;

bool Logger::initialize() {
    if (isInitialized) return true;

    // Initialize flash manager
    if (!FlashManager::initialize()) {
        return false;
    }

    // Reset buffers
    performanceBuffer.clear();
    eventBuffer.clear();
    statsBuffer.clear();

    // Reset statistics
    totalDeviation = 0;
    sampleCount = 0;
    maxSpeed = 0;
    currentLap = 0;
    curveCount = 0;

    isInitialized = true;
    return true;
}

bool Logger::startSession(DebugMode mode, uint8_t plannedLaps) {
    if (!isInitialized) {
        if (!initialize()) return false;
    }

    // Reset flash position
    FlashManager::reset();

    // Create and write session header
    SessionHeader header;
    header.startTime = millis();
    header.mode = mode;
    header.plannedLaps = plannedLaps;
    header.pidKp = ProfileManager::getKP(K_PROPORTIONAL_DEFAULT);
    header.pidKd = ProfileManager::getKD(K_DERIVATIVE_DEFAULT);
    header.filterAlpha = ProfileManager::getFilterCoefficient(FILTER_COEFFICIENT_DEFAULT);

    // Store calibration values
    for (uint8_t i = 0; i < 6; i++) {
        // TODO: Get actual calibration values from Sensors class
        header.sensorCalibrationData[i] = 0;
    }

    header.headerChecksum = calculateChecksum(&header, sizeof(SessionHeader) - sizeof(uint32_t));

    if (!FlashManager::writeBlock(&header, sizeof(SessionHeader))) {
        return false;
    }

    sessionStartTime = header.startTime;
    loggingActive = true;
    lastSampleTime = sessionStartTime;
    lastFlushTime = sessionStartTime;

    // Log session start event
    logEvent(EventType::SESSION_START);

    return true;
}

void Logger::endSession() {
    if (!loggingActive) return;

    // Log session end event
    logEvent(EventType::SESSION_END);

    // Force flush of remaining data
    flushBuffers();

    loggingActive = false;
}

void Logger::logPerformance(int16_t linePosition, int16_t error, int16_t correction,
    uint8_t leftSpeed, uint8_t rightSpeed, uint8_t state) {
    if (!loggingActive || !shouldSample()) return;

    PerformanceRecord record;
    record.timestamp = millis() - sessionStartTime;
    record.linePosition = linePosition;
    record.error = error;
    record.correction = correction;
    record.speedLeft = leftSpeed;
    record.speedRight = rightSpeed;
    record.state = state;
    record.checksum = calculateChecksum(&record, sizeof(PerformanceRecord) - sizeof(uint8_t));

    // Update statistics
    updateStats(record);

    // Add to buffer
    if (!performanceBuffer.push(record)) {
        // Buffer full, force flush
        flushBuffers();
        performanceBuffer.push(record);
    }
}

void Logger::logEvent(EventType type, uint16_t data) {
    if (!loggingActive) return;

    EventRecord record;
    record.timestamp = millis() - sessionStartTime;
    record.type = type;
    record.data = data;
    record.checksum = calculateChecksum(&record, sizeof(EventRecord) - sizeof(uint8_t));

    if (!eventBuffer.push(record)) {
        // Buffer full, force flush
        flushBuffers();
        eventBuffer.push(record);
    }
}

void Logger::logLapStats(const LapStats& stats) {
    if (!loggingActive) return;

    if (!statsBuffer.push(stats)) {
        // Buffer full, force flush
        flushBuffers();
        statsBuffer.push(stats);
    }

    currentLap++;
}

bool Logger::isLogging() {
    return loggingActive;
}

void Logger::getSessionStats(uint32_t& duration, uint8_t& completedLaps,
    uint16_t& totalCurves, float& avgDeviation) {
    duration = millis() - sessionStartTime;
    completedLaps = currentLap;
    totalCurves = curveCount;
    avgDeviation = sampleCount > 0 ? totalDeviation / sampleCount : 0;
}

void Logger::process() {
    if (!loggingActive) return;

    // Check if it's time to flush buffers
    uint32_t currentTime = millis();
    if (currentTime - lastFlushTime >= 1000) { // Flush every second or when in straight line
        if (abs(Sensors::calculateLinePosition()) < STRAIGHT_THRESHOLD) {
            flushBuffers();
            lastFlushTime = currentTime;
        }
    }
}

void Logger::flushBuffers() {
    // Only flush if we can write
    if (!FlashManager::canWrite(Sensors::calculateLinePosition())) {
        return;
    }

    // Write performance records
    if (!performanceBuffer.isEmpty()) {
        uint8_t count = performanceBuffer.getCount();
        PerformanceRecord records[count];
        performanceBuffer.popMultiple(records, count);
        FlashManager::writeBlock(records, count * sizeof(PerformanceRecord));
    }

    // Write event records
    if (!eventBuffer.isEmpty()) {
        uint8_t count = eventBuffer.getCount();
        EventRecord records[count];
        eventBuffer.popMultiple(records, count);
        FlashManager::writeBlock(records, count * sizeof(EventRecord));
    }

    // Write lap stats
    if (!statsBuffer.isEmpty()) {
        uint8_t count = statsBuffer.getCount();
        LapStats records[count];
        statsBuffer.popMultiple(records, count);
        FlashManager::writeBlock(records, count * sizeof(LapStats));
    }
}

bool Logger::shouldSample() {
    uint32_t currentTime = millis();
    int16_t position = Sensors::calculateLinePosition();
    uint32_t sampleInterval;

    // Adjust sample rate based on position
    if (abs(position) > TURN_THRESHOLD) {
        sampleInterval = SAMPLE_RATE_CURVE;  // Higher rate in curves
    }
    else {
        sampleInterval = SAMPLE_RATE_STRAIGHT;  // Lower rate in straight lines
    }

    if (currentTime - lastSampleTime >= sampleInterval) {
        lastSampleTime = currentTime;
        return true;
    }

    return false;
}

uint8_t Logger::calculateChecksum(const void* data, uint16_t size) {
    uint8_t checksum = 0;
    const uint8_t* bytes = (const uint8_t*)data;

    for (uint16_t i = 0; i < size; i++) {
        checksum += bytes[i];
    }

    return checksum;
}

void Logger::updateStats(const PerformanceRecord& record) {
    totalDeviation += abs(record.linePosition);
    sampleCount++;
    maxSpeed = max(maxSpeed, max(record.speedLeft, record.speedRight));
}

#endif // DEBUG_LEVEL > 0