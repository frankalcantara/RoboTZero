 /*
 * Line Sensors Test
 * LED will blink when any sensor detects a transition from low to high
 */
#include <Arduino.h>

const int PIN_STATUS_LED = 13;
const int SENSORS[] = {A6, A5, A4, A3, A2, A1};  // Line sensors pins
const int NUM_SENSORS = 6;

int lastReadings[6] = {0, 0, 0, 0, 0, 0};
const int THRESHOLD = 500;  // Adjust based on your sensors

void setup() {
  pinMode(PIN_STATUS_LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Line Sensors Test");
}

void loop() {
  bool transition = false;
  
  // Check each sensor
  for(int i = 0; i < NUM_SENSORS; i++) {
    int currentReading = analogRead(SENSORS[i]);
    
    // Check for low to high transition
    if(currentReading > THRESHOLD && lastReadings[i] <= THRESHOLD) {
      transition = true;
      Serial.print("Sensor ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(currentReading);
    }
    
    lastReadings[i] = currentReading;
  }
  
  // Blink LED if transition detected
  if(transition) {
    digitalWrite(PIN_STATUS_LED, HIGH);
    delay(50);
    digitalWrite(PIN_STATUS_LED, LOW);
  }
  
  delay(10);  // Small delay to prevent too frequent readings
}