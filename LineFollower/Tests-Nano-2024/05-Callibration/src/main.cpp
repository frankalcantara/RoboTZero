/*
 * Line Sensors Calibration Test
 * Shows min/max values for each sensor and helps with calibration
 */
#include <Arduino.h>

const int PIN_STATUS_LED = 13;
const int PIN_START_BUTTON = 11;
const int SENSORS[] = { A6, A5, A4, A3, A2, A1 };  // Line sensors pins
const int NUM_SENSORS = 6;

// Arrays to store calibration values
int sensorMin[NUM_SENSORS];
int sensorMax[NUM_SENSORS];
int sensorCurrent[NUM_SENSORS];

bool isCalibrating = false;
unsigned long calibrationStartTime = 0;
const unsigned long CALIBRATION_TIME = 3000;  // 3 seconds calibration

void setup() {
    pinMode(PIN_STATUS_LED, OUTPUT);
    pinMode(PIN_START_BUTTON, INPUT);

    Serial.begin(9600);
    Serial.println("Line Sensors Calibration Test");
    Serial.println("Press button to start calibration");
    Serial.println("Move robot over line during calibration");

    // Initialize min/max values
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensorMin[i] = 1023;  // Start with maximum possible
        sensorMax[i] = 0;     // Start with minimum possible
    }
}

void printSensorValues() {
    Serial.println("\nCurrent Readings:");
    for (int i = 0; i < NUM_SENSORS; i++) {
        Serial.print("Sensor ");
        Serial.print(i);
        Serial.print(": ");
        Serial.print(sensorCurrent[i]);
        Serial.print(" (Min: ");
        Serial.print(sensorMin[i]);
        Serial.print(", Max: ");
        Serial.print(sensorMax[i]);
        Serial.println(")");
    }
}

void readSensors() {
    for (int i = 0; i < NUM_SENSORS; i++) {
        sensorCurrent[i] = analogRead(SENSORS[i]);

        if (isCalibrating) {
            // Update min/max values during calibration
            if (sensorCurrent[i] < sensorMin[i]) sensorMin[i] = sensorCurrent[i];
            if (sensorCurrent[i] > sensorMax[i]) sensorMax[i] = sensorCurrent[i];
        }
    }
}

void calibrationBlink() {
    static unsigned long lastBlink = 0;
    const int blinkRate = 100;  // Fast blink during calibration

    if (millis() - lastBlink >= blinkRate) {
        digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
        lastBlink = millis();
    }
}

void normalBlink() {
    static unsigned long lastBlink = 0;
    const int blinkRate = 500;  // Slow blink during normal operation

    if (millis() - lastBlink >= blinkRate) {
        digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
        lastBlink = millis();
    }
}

void loop() {
    // Check button for calibration start
    if (digitalRead(PIN_START_BUTTON) == HIGH && !isCalibrating) {
        isCalibrating = true;
        calibrationStartTime = millis();
        Serial.println("Calibration started - Move robot over line!");

        // Reset min/max values
        for (int i = 0; i < NUM_SENSORS; i++) {
            sensorMin[i] = 1023;
            sensorMax[i] = 0;
        }
    }

    // Read sensors
    readSensors();

    // Handle calibration
    if (isCalibrating) {
        calibrationBlink();

        // Check if calibration time is over
        if (millis() - calibrationStartTime >= CALIBRATION_TIME) {
            isCalibrating = false;
            Serial.println("\nCalibration complete!");
            Serial.println("Final calibration values:");
            printSensorValues();
        }
    }
    else {
        normalBlink();

        // Print values every 500ms
        static unsigned long lastPrint = 0;
        if (millis() - lastPrint >= 500) {
            printSensorValues();
            lastPrint = millis();
        }
    }

    // Small delay to prevent serial buffer overflow
    delay(10);
}