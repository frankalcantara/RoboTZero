/*
 * Button and LED Test
 * Press button to cycle through different LED patterns
 */
#include <Arduino.h>

const int PIN_STATUS_LED = 13;
const int PIN_START_BUTTON = 11;
const int DEBOUNCE_DELAY = 50;

int buttonState = HIGH;
int lastButtonState = HIGH;
int ledMode = 0;  // 0=off, 1=on, 2=slow blink, 3=fast blink
unsigned long lastDebounceTime = 0;
unsigned long lastBlinkTime = 0;

void setup() {
    pinMode(PIN_STATUS_LED, OUTPUT);
    pinMode(PIN_START_BUTTON, INPUT);
    Serial.begin(9600);
    Serial.println("Button and LED Test");
    Serial.println("Press button to cycle through LED modes");
}

void updateLED() {
    unsigned long currentMillis = millis();

    switch (ledMode) {
    case 0:  // Off
        digitalWrite(PIN_STATUS_LED, LOW);
        break;

    case 1:  // On
        digitalWrite(PIN_STATUS_LED, HIGH);
        break;

    case 2:  // Slow blink (1 Hz)
        if (currentMillis - lastBlinkTime >= 500) {
            digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
            lastBlinkTime = currentMillis;
        }
        break;

    case 3:  // Fast blink (5 Hz)
        if (currentMillis - lastBlinkTime >= 100) {
            digitalWrite(PIN_STATUS_LED, !digitalRead(PIN_STATUS_LED));
            lastBlinkTime = currentMillis;
        }
        break;
    }
}

void loop() {
    // Read button with debounce
    int reading = digitalRead(PIN_START_BUTTON);

    if (reading != lastButtonState) {
        lastDebounceTime = millis();
    }

    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
        if (reading != buttonState) {
            buttonState = reading;

            if (buttonState == HIGH) {  // Button released
                ledMode = (ledMode + 1) % 4;
                Serial.print("LED Mode: ");
                Serial.println(ledMode);
            }
        }
    }

    lastButtonState = reading;
    updateLED();
}