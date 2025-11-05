/*
 * Marker Sensors Test
 * LED will blink when marker sensors detect a transition
 */
#include <Arduino.h>

const int PIN_STATUS_LED = 13;
const int PIN_MARKER_LEFT = A7;
const int PIN_MARKER_RIGHT = A0;
const int MARKER_THRESHOLD = 180;  // Adjust based on your markers

int lastLeftMarker = 0;
int lastRightMarker = 0;

void setup() {
    pinMode(PIN_STATUS_LED, OUTPUT);
    Serial.begin(9600);
    Serial.println("Marker Sensors Test");
}

void loop() {
    int leftMarker = analogRead(PIN_MARKER_LEFT);
    int rightMarker = analogRead(PIN_MARKER_RIGHT);

    // Check for transitions
    bool leftTransition = (leftMarker <= MARKER_THRESHOLD && lastLeftMarker > MARKER_THRESHOLD);
    bool rightTransition = (rightMarker <= MARKER_THRESHOLD && lastRightMarker > MARKER_THRESHOLD);

    if (leftTransition || rightTransition) {
        digitalWrite(PIN_STATUS_LED, HIGH);

        if (leftTransition) {
            Serial.print("Left Marker: ");
            Serial.println(leftMarker);
        }
        if (rightTransition) {
            Serial.print("Right Marker: ");
            Serial.println(rightMarker);
        }

        delay(50);
        digitalWrite(PIN_STATUS_LED, LOW);
    }

    lastLeftMarker = leftMarker;
    lastRightMarker = rightMarker;

    delay(10);
}