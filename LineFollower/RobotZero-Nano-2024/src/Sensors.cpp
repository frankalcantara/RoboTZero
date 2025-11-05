#include <Arduino.h>
#include "Sensors.h"
#include "config.h"
#include "debug.h"

// Static member initialization
int16_t Sensors::sensorMin[NUM_SENSORES] = { SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE };
int16_t Sensors::sensorMax[NUM_SENSORES] = { SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE };
volatile int16_t Sensors::processedSensorValues[NUM_SENSORES];
boolean Sensors::isLineDetected;
int16_t Sensors::lastValidLinePosition;
int16_t Sensors::lastValidPosition;

void Sensors::calibration() {
  static Timer calibrationTimer;
  static uint16_t calibrationCount = 0;

  calibrationCount = 0;
  calibrationTimer.Start(CALIBRATION_DELAY);

  while (calibrationCount < CALIBRATION_SAMPLES) {
    if (calibrationTimer.Expired()) {
      int16_t v_s[NUM_SENSORES];

      // Read all sensors
      v_s[0] = analogRead(PIN_LINE_LEFT_EDGE);
      v_s[1] = analogRead(PIN_LINE_LEFT_MID);
      v_s[2] = analogRead(PIN_LINE_CENTER_LEFT);
      v_s[3] = analogRead(PIN_LINE_CENTER_RIGHT);
      v_s[4] = analogRead(PIN_LINE_RIGHT_MID);
      v_s[5] = analogRead(PIN_LINE_RIGHT_EDGE);

      // Update min and max values
      for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        DEBUG_PRINT_VAL(v_s[i]);
        DEBUG_PRINT("\t");

        if (v_s[i] < sensorMin[i]) sensorMin[i] = v_s[i];
        if (v_s[i] > sensorMax[i]) sensorMax[i] = v_s[i];
      }
      DEBUG_PRINTLN("");

      // Debug minimum values
      DEBUG_PRINT("Minimums\t");
      for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        DEBUG_PRINT_VAL(sensorMin[i]);
        DEBUG_PRINT("\t");
      }
      DEBUG_PRINTLN("");

      // Debug maximum values
      DEBUG_PRINT("Maximums\t");
      for (uint8_t i = 0; i < NUM_SENSORES; i++) {
        DEBUG_PRINT_VAL(sensorMax[i]);
        DEBUG_PRINT("\t");
      }
      DEBUG_PRINTLN("");

      calibrationCount++;
      calibrationTimer.Start(CALIBRATION_DELAY);
    }
  }
}

void Sensors::readSensors() {
  int16_t s[NUM_SENSORES];
  bool isOnline;
  int localSum = 0;

  // Read sensors
  s[0] = analogRead(PIN_LINE_LEFT_EDGE);
  s[1] = analogRead(PIN_LINE_LEFT_MID);
  s[2] = analogRead(PIN_LINE_CENTER_LEFT);
  s[3] = analogRead(PIN_LINE_CENTER_RIGHT);
  s[4] = analogRead(PIN_LINE_RIGHT_MID);
  s[5] = analogRead(PIN_LINE_RIGHT_EDGE);

  // Process values
  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    if (s[i] < sensorMin[i]) s[i] = sensorMin[i];
    if (s[i] > sensorMax[i]) s[i] = sensorMax[i];
    s[i] = map(s[i], sensorMin[i], sensorMax[i], 100, 0);
    localSum += s[i];
  }

  isOnline = (localSum > SENSOR_THRESHOLD);
  if (!isOnline) localSum = SENSOR_THRESHOLD;

  // Update global variables atomically
  noInterrupts();
  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    processedSensorValues[i] = s[i];
  }
  isLineDetected = isOnline;
  interrupts();
}

int16_t Sensors::calculateLinePosition() {
  readSensors();

  int16_t s_p_local[NUM_SENSORES];
  bool isOnline;

  // Copy values atomically
  noInterrupts();
  for (uint8_t i = 0; i < NUM_SENSORES; i++) {
    s_p_local[i] = processedSensorValues[i];
  }
  isOnline = isLineDetected;
  interrupts();

  // Calculate weighted average
  float avg = SENSOR_WEIGHT_S1 * s_p_local[0] + SENSOR_WEIGHT_S2 * s_p_local[1] +
    SENSOR_WEIGHT_S3 * s_p_local[2] + SENSOR_WEIGHT_S4 * s_p_local[3] +
    SENSOR_WEIGHT_S5 * s_p_local[4] + SENSOR_WEIGHT_S6 * s_p_local[5];

  int sum = s_p_local[0] + s_p_local[1] + s_p_local[2] +
    s_p_local[3] + s_p_local[4] + s_p_local[5];

// Update position based on sensor readings
  if (isOnline && sum > SENSOR_THRESHOLD) {
    if (sum != 0) {
      float position = constrain(100.0f * avg / sum, -100.0f, 100.0f);
      lastValidLinePosition = int16_t(position);
    }
    else {
      lastValidLinePosition = lastValidPosition;
    }
  }
  else {
    lastValidLinePosition = (lastValidPosition < 0) ? -100 : 100;
  }

  lastValidPosition = lastValidLinePosition;
  return lastValidLinePosition;
}