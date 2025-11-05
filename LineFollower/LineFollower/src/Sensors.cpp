#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include "Timer.h"

// Arrays de calibração
int v_s_min[NUM_SENSORES] = { SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE, SENSOR_MAX_VALUE };
int v_s_max[NUM_SENSORES] = { SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE, SENSOR_MIN_VALUE };

// Variáveis globais
volatile int s_p[NUM_SENSORES];
boolean online;
int pos;
int l_pos;

// Variáveis para controle da calibração
static Timer calibrationTimer;
static int calibrationCount = 0;

void calibracion() {
  calibrationCount = 0;
  calibrationTimer.start(CALIBRATION_DELAY);

  while (calibrationCount < CALIBRATION_SAMPLES) {
    if (calibrationTimer.expired()) {
      int v_s[NUM_SENSORES];

      // Lê todos os sensores
      v_s[0] = analogRead(SENSOR_1);
      v_s[1] = analogRead(SENSOR_2);
      v_s[2] = analogRead(SENSOR_3);
      v_s[3] = analogRead(SENSOR_4);
      v_s[4] = analogRead(SENSOR_5);
      v_s[5] = analogRead(SENSOR_6);

      // Atualiza valores mínimos e máximos
      for (int i = 0; i < NUM_SENSORES; i++) {
        DEBUG_PRINT_VAL(v_s[i]);
        DEBUG_PRINT("\t");

        if (v_s[i] < v_s_min[i]) v_s_min[i] = v_s[i];
        if (v_s[i] > v_s_max[i]) v_s_max[i] = v_s[i];
      }
      DEBUG_PRINTLN("");

      // Debug dos valores mínimos
      DEBUG_PRINT("Minimos\t");
      for (int i = 0; i < NUM_SENSORES; i++) {
        DEBUG_PRINT_VAL(v_s_min[i]);
        DEBUG_PRINT("\t");
      }
      DEBUG_PRINTLN("");

      // Debug dos valores máximos
      DEBUG_PRINT("Maximos\t");
      for (int i = 0; i < NUM_SENSORES; i++) {
        DEBUG_PRINT_VAL(v_s_max[i]);
        DEBUG_PRINT("\t");
      }
      DEBUG_PRINTLN("");

      calibrationCount++;
      calibrationTimer.start(CALIBRATION_DELAY);  // Reinicia o timer para próxima amostra
    }
  }
}

void readSensors() {
  int s[NUM_SENSORES];
  bool isOnline;
  int localSum = 0;

  // Leitura dos sensores
  s[0] = analogRead(SENSOR_1);
  s[1] = analogRead(SENSOR_2);
  s[2] = analogRead(SENSOR_3);
  s[3] = analogRead(SENSOR_4);
  s[4] = analogRead(SENSOR_5);
  s[5] = analogRead(SENSOR_6);

  // Processamento dos valores
  for (int i = 0; i < NUM_SENSORES; i++) {
    if (s[i] < v_s_min[i]) s[i] = v_s_min[i];
    if (s[i] > v_s_max[i]) s[i] = v_s_max[i];
    s[i] = map(s[i], v_s_min[i], v_s_max[i], 100, 0);
    localSum += s[i];
  }

  isOnline = (localSum > SENSOR_THRESHOLD);
  if (!isOnline) localSum = SENSOR_THRESHOLD;

  // Atualiza variáveis globais de forma atômica
  noInterrupts();
  for (int i = 0; i < NUM_SENSORES; i++) {
    s_p[i] = s[i];
  }
  online = isOnline;
  interrupts();
}

int GetPos() {
  readSensors();

  int s_p_local[NUM_SENSORES];
  bool isOnline;

  // Copia valores de forma atômica
  noInterrupts();
  for (int i = 0; i < NUM_SENSORES; i++) {
    s_p_local[i] = s_p[i];
  }
  isOnline = online;
  interrupts();

  float prom = PESO_S1 * s_p_local[0] + PESO_S2 * s_p_local[1] +
    PESO_S3 * s_p_local[2] + PESO_S4 * s_p_local[3] +
    PESO_S5 * s_p_local[4] + PESO_S6 * s_p_local[5];

  int sum = s_p_local[0] + s_p_local[1] + s_p_local[2] +
    s_p_local[3] + s_p_local[4] + s_p_local[5];

  if (isOnline && sum > SENSOR_THRESHOLD) {
    if (sum != 0) {
      float position = constrain(100.0f * prom / sum, -100.0f, 100.0f);
      pos = int(position);
    }
    else {
      pos = l_pos;
    }
  }
  else {
    pos = (l_pos < 0) ? -100 : 100;
  }

  l_pos = pos;
  return pos;
}

