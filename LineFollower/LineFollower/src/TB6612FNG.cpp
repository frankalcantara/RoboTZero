#include <Arduino.h>
#include "config.h" 

// Resto do código permanece igual
void TB6612FNG_init() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
}

void MotorIz(int value) {
  // Proteção do valor PWM
  value = constrain(value, -255, 255);

  if (value >= 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
  }
  else {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    value *= -1;
  }

  analogWrite(PWMA, value);
}

void MotorDe(int value) {
  // Proteção do valor PWM
  value = constrain(value, -255, 255);

  if (value >= 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
  }
  else {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    value *= -1;
  }

  analogWrite(PWMB, value);
}

void Motores(int left, int right) {
  MotorIz(left);
  MotorDe(right);
}