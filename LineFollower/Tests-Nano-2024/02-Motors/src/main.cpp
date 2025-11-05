/*
 * Motors Test
 * Performs a sequence of movements to test motors
 * Includes final high-speed straight run
 */
#include <Arduino.h>

// Motor pins
const int PIN_MOTOR_LEFT_FWD = 7;
const int PIN_MOTOR_LEFT_REV = 4;
const int PIN_MOTOR_LEFT_PWM = 3;
const int PIN_MOTOR_RIGHT_FWD = 8;
const int PIN_MOTOR_RIGHT_REV = 9;
const int PIN_MOTOR_RIGHT_PWM = 10;
const int PIN_STATUS_LED = 13;

const int MOTOR_SPEED = 150;     // Normal test speed (0-255)
const int MAX_SPEED = 255;       // Maximum speed for final run
const int MOVE_TIME = 1000;      // Time for each movement
const int BLINK_DELAY = 250;     // LED blink interval (ms)
const int PAUSE_TIME = 5000;     // 5 second pause before final run
const int FINAL_RUN_TIME = 3000; // Time for final high-speed run

void setupMotors() {
	pinMode(PIN_MOTOR_LEFT_FWD, OUTPUT);
	pinMode(PIN_MOTOR_LEFT_REV, OUTPUT);
	pinMode(PIN_MOTOR_LEFT_PWM, OUTPUT);
	pinMode(PIN_MOTOR_RIGHT_FWD, OUTPUT);
	pinMode(PIN_MOTOR_RIGHT_REV, OUTPUT);
	pinMode(PIN_MOTOR_RIGHT_PWM, OUTPUT);
	pinMode(PIN_STATUS_LED, OUTPUT);
}

void stopMotors() {
	analogWrite(PIN_MOTOR_LEFT_PWM, 0);
	analogWrite(PIN_MOTOR_RIGHT_PWM, 0);
}

void moveForward(int speed) {
	digitalWrite(PIN_MOTOR_LEFT_FWD, HIGH);
	digitalWrite(PIN_MOTOR_LEFT_REV, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_FWD, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_REV, LOW);
	analogWrite(PIN_MOTOR_LEFT_PWM, speed);
	analogWrite(PIN_MOTOR_RIGHT_PWM, speed);
}

void moveBackward() {
	digitalWrite(PIN_MOTOR_LEFT_FWD, LOW);
	digitalWrite(PIN_MOTOR_LEFT_REV, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_FWD, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_REV, HIGH);
	analogWrite(PIN_MOTOR_LEFT_PWM, MOTOR_SPEED);
	analogWrite(PIN_MOTOR_RIGHT_PWM, MOTOR_SPEED);
}

void turnRight() {
	digitalWrite(PIN_MOTOR_LEFT_FWD, HIGH);
	digitalWrite(PIN_MOTOR_LEFT_REV, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_FWD, LOW);
	digitalWrite(PIN_MOTOR_RIGHT_REV, HIGH);
	analogWrite(PIN_MOTOR_LEFT_PWM, MOTOR_SPEED);
	analogWrite(PIN_MOTOR_RIGHT_PWM, MOTOR_SPEED);
}

void turnLeft() {
	digitalWrite(PIN_MOTOR_LEFT_FWD, LOW);
	digitalWrite(PIN_MOTOR_LEFT_REV, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_FWD, HIGH);
	digitalWrite(PIN_MOTOR_RIGHT_REV, LOW);
	analogWrite(PIN_MOTOR_LEFT_PWM, MOTOR_SPEED);
	analogWrite(PIN_MOTOR_RIGHT_PWM, MOTOR_SPEED);
}

void blinkLED(unsigned long duration) {
	unsigned long startTime = millis();
	bool ledState = false;

	while (millis() - startTime < duration) {
		ledState = !ledState;
		digitalWrite(PIN_STATUS_LED, ledState);
		delay(BLINK_DELAY);
	}
	digitalWrite(PIN_STATUS_LED, LOW);
}

void setup() {
	setupMotors();
	Serial.begin(9600);
	Serial.println("Motors Test");
}

void loop() {
  // Standard test sequence
	Serial.println("Forward");
	moveForward(MOTOR_SPEED);
	delay(MOVE_TIME);

	Serial.println("Stop");
	stopMotors();
	delay(500);

	Serial.println("Turn Right");
	turnRight();
	delay(MOVE_TIME);

	Serial.println("Stop");
	stopMotors();
	delay(500);

	Serial.println("Turn Left");
	turnLeft();
	delay(MOVE_TIME);

	Serial.println("Stop");
	stopMotors();
	delay(500);

	Serial.println("Full Turn (Right)");
	turnRight();
	delay(MOVE_TIME * 2);

	Serial.println("Stop");
	stopMotors();
	delay(500);

	Serial.println("Backward");
	moveBackward();
	delay(MOVE_TIME);

	Serial.println("Stop");
	stopMotors();

	// New addition: 5-second pause with blinking LED
	Serial.println("Pausing for 5 seconds with LED blinking");
	blinkLED(PAUSE_TIME);

	// Final high-speed straight run
	Serial.println("Starting high-speed straight run");
	moveForward(MAX_SPEED);
	delay(FINAL_RUN_TIME);

	Serial.println("Stop");
	stopMotors();

	// Long pause before repeating the entire sequence
	delay(2000);
}