#include "CourseMarkers.h"
#include "config.h"
#include "debug.h"
#include "globals.h"
#include "MotorsDrivers.h"

#if DEBUG_LEVEL > 0
#include "FlashManager.h"
#endif

// Static member initialization
int CourseMarkers::speed = 0;
int CourseMarkers::lastMarkerState = 0;
int CourseMarkers::previousMarkerState = 0;
int CourseMarkers::oldMarkerState = 0;
int CourseMarkers::currentMarkerState = 0;
int16_t CourseMarkers::leftMarkerDetected = 0;
int16_t CourseMarkers::rightMarkerDetected = 0;
bool CourseMarkers::isTurning = false;
bool CourseMarkers::isExitingTurn = false;
uint8_t CourseMarkers::boostCountdown = 0;
Timer CourseMarkers::stopTimer;
Timer CourseMarkers::slowdownTimer;
uint32_t CourseMarkers::lastReadTime = 0;

void CourseMarkers::readCourseMarkers() {
  // Check if enough time has passed since last read
  uint32_t currentTime = millis();
  if (currentTime - lastReadTime < MARKER_READ_INTERVAL) {
    return;
  }
  lastReadTime = currentTime;

  // Read markers with immediate threshold check to avoid extra variables
  bool leftDetected = analogRead(PIN_MARKER_LEFT) <= MARKER_DETECTION_THRESHOLD;
  bool rightDetected = analogRead(PIN_MARKER_RIGHT) <= MARKER_DETECTION_THRESHOLD;

  // Direct state calculation without intermediate steps
  currentMarkerState = (leftDetected << 1) | rightDetected;

  // Update LED only if markers detected
  digitalWrite(PIN_STATUS_LED, leftDetected || rightDetected);

  DEBUG_PRINT("Marker State: ");
  DEBUG_PRINTLN_VAL(currentMarkerState);
}

int CourseMarkers::speedControl(int error) {
  // Track situation detection with optimized comparisons
  bool curve_detected = abs(error) > TURN_THRESHOLD;

  // Early return for curves to reduce processing time
  if (curve_detected) {
    isTurning = true;
    isExitingTurn = false;
    return TURN_SPEED;
  }

  bool straight_detected = abs(error) < STRAIGHT_THRESHOLD;
  int target_speed;

  if (straight_detected) {
    if (isTurning) {
      isExitingTurn = true;
      boostCountdown = BOOST_DURATION;
    }
    isTurning = false;
    target_speed = isPrecisionMode ? SPEED_SLOW : BASE_FAST;
  }
  else {
    target_speed = map(abs(error),
      STRAIGHT_THRESHOLD,
      TURN_THRESHOLD,
      (isPrecisionMode ? SPEED_SLOW : BASE_FAST),
      TURN_SPEED);
  }

  // Apply boost with bounds checking
  if (isExitingTurn && boostCountdown > 0 && !isPrecisionMode) {
    target_speed = min(255, target_speed + BOOST_INCREMENT);
    boostCountdown--;
  }

  // Optimize speed adjustment
  int step = (target_speed > currentSpeed) ? ACCELERATION_STEP : BRAKE_STEP;
  if (abs(target_speed - currentSpeed) <= step) {
    currentSpeed = target_speed;
  }
  else {
    currentSpeed += (target_speed > currentSpeed) ? step : -step;
  }

  return constrain(currentSpeed, TURN_SPEED, (isPrecisionMode ? SPEED_SLOW : BASE_FAST));
}

void CourseMarkers::processMarkerSignals() {
  readCourseMarkers();

  // Only process state changes
  if (lastMarkerState == currentMarkerState) {
    return;
  }

  // State machine for marker processing
  switch (currentMarkerState) {
  case 0: // No markers
    if (lastMarkerState == 2 && previousMarkerState == 0) {
      handleFinishLine();
    }
    else if (lastMarkerState == 1 && previousMarkerState == 0) {
      handleSpeedMode();
    }
    else if (lastMarkerState == 3 || previousMarkerState == 3 || oldMarkerState == 3) {
      handleIntersection();
    }
    break;
  }

  // Update state history
  oldMarkerState = previousMarkerState;
  previousMarkerState = lastMarkerState;
  lastMarkerState = currentMarkerState;

  // Handle stopping sequence
  if (isStopSequenceActive && !isRobotStopped) {
    if (!slowdownTimer.Expired() && currentSpeed > SPEED_BRAKE) {
      currentSpeed = SPEED_BRAKE;
    }
    else if (stopTimer.Expired()) {
      currentSpeed = 0;
      MotorDriver::setMotorsPower(0, 0);
      isRobotStopped = true;
    }
  }
}

void CourseMarkers::handleFinishLine() {
  lapCount++;
  if (lapCount == 2 && !isStopSequenceActive) {
    isStopSequenceActive = true;
    slowdownTimer.Start(50);
    stopTimer.Start(STOP_DELAY);
#if DEBUG_LEVEL > 0
    FlashManager::setLogReady();
#endif
  }
}

void CourseMarkers::handleSpeedMode() {
  isPrecisionMode = !isPrecisionMode;
  currentSpeed = isPrecisionMode ? SPEED_SLOW : BASE_FAST;
  isTurning = false;
  isExitingTurn = false;
  boostCountdown = 0;
  DEBUG_PRINTLN(isPrecisionMode ? "Slow mode activated" : "Fast mode activated");
}

void CourseMarkers::handleIntersection() {
  DEBUG_PRINTLN("Intersection detected");
}