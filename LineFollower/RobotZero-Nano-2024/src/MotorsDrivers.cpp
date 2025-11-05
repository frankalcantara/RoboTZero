#include <Arduino.h>
#include "MotorsDrivers.h"
#include "config.h"

void MotorDriver::initializeMotorDriver() {
  pinMode(PIN_MOTOR_LEFT_FWD, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_REV, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_PWM, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_FWD, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_REV, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_PWM, OUTPUT);
}

void MotorDriver::setLeftMotorPower(int value) {
  // Protect PWM value range
  value = constrain(value, -255, 255);

  if (value >= 0) {
    digitalWrite(PIN_MOTOR_LEFT_FWD, HIGH);
    digitalWrite(PIN_MOTOR_LEFT_REV, LOW);
  }
  else {
    digitalWrite(PIN_MOTOR_LEFT_FWD, LOW);
    digitalWrite(PIN_MOTOR_LEFT_REV, HIGH);
    value *= -1;
  }

  analogWrite(PIN_MOTOR_LEFT_PWM, value);
}

void MotorDriver::setRightMotorPower(int value) {
  // Protect PWM value range
  value = constrain(value, -255, 255);

  if (value >= 0) {
    digitalWrite(PIN_MOTOR_RIGHT_FWD, HIGH);
    digitalWrite(PIN_MOTOR_RIGHT_REV, LOW);
  }
  else {
    digitalWrite(PIN_MOTOR_RIGHT_FWD, LOW);
    digitalWrite(PIN_MOTOR_RIGHT_REV, HIGH);
    value *= -1;
  }

  analogWrite(PIN_MOTOR_RIGHT_PWM, value);
}

void MotorDriver::setMotorsPower(int left, int right) {
  setLeftMotorPower(left);
  setRightMotorPower(right);
}