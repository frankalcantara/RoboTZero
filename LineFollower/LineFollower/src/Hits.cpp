#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include "Timer.h"
#include "globals.h"



// Declaração dos protótipos das funções
void Read_hits();
void detectGeo();
void funcion_HL();
void funcion_HR();
void funcion_Cruce();
int controleVelocidade(int erro);

// Declaração de funções externas
extern void Motores(int left, int right);

// Variáveis globais
int velo = 0;                  // Velocidade atual
int l_geo = 0;                 // Último estado da geometria
int ll_geo = 0;                // Penúltimo estado da geometria
int lll_geo = 0;               // Antepenúltimo estado da geometria
int geo = 0;                   // Estado atual da geometria
int HL = 0, HR = 0;            // Estados dos sensores de hit

// Novas variáveis para controle de velocidade
static bool em_curva = false;
static bool saindo_curva = false;
static uint8_t boost_contador = 0;

// Timer para controle de parada
static Timer stopTimer;
static Timer slowdownTimer;

extern int error_pasado;  // Declaração de variável externa

void Read_hits() {
  HL = analogRead(SENSOR_HIT_L);
  HR = analogRead(SENSOR_HIT_R);

  bool ledState = false;

  if (HL > UMBRAL) {
    HL = 0;
  }
  else {
    HL = 1;
    ledState = true;
  }

  if (HR > UMBRAL) {
    HR = 0;
  }
  else {
    HR = 1;
    ledState = true;
  }

  digitalWrite(PINLED, ledState);

  DEBUG_PRINT("HR: ");
  DEBUG_PRINT_VAL(HR);
  DEBUG_PRINT(" HL: ");
  DEBUG_PRINT_VAL(HL);
  DEBUG_PRINT(" A0: ");
  DEBUG_PRINT_VAL(analogRead(SENSOR_HIT_R));
  DEBUG_PRINT(" A7: ");
  DEBUG_PRINT_VAL(analogRead(SENSOR_HIT_L));
  DEBUG_PRINTLN("");
}

// Nova função de controle de velocidade
int controleVelocidade(int erro) {
    // Detecta situação da pista
  bool curva_detectada = abs(erro) > CURVA_ERRO;
  bool reta_detectada = abs(erro) < RETA_ERRO;

  // Velocidade alvo baseada na situação
  int vel_alvo;

  if (curva_detectada) {
    em_curva = true;
    saindo_curva = false;
    vel_alvo = CURVA_VEL;
  }
  else if (reta_detectada) {
    if (em_curva) {
      saindo_curva = true;
      boost_contador = BOOST_TEMPO;
    }
    em_curva = false;
    vel_alvo = slow ? VELOCIDADE_5 : BASE_FAST;
  }
  else {
    vel_alvo = map(abs(erro),
      RETA_ERRO,
      CURVA_ERRO,
      (slow ? VELOCIDADE_5 : BASE_FAST),
      CURVA_VEL);
  }

  // Aplica boost se necessário e não estiver em modo lento
  if (saindo_curva && boost_contador > 0 && !slow) {
    vel_alvo = min(255, vel_alvo + 30);
    boost_contador--;
  }

  // Determina velocidade de ajuste
  int passo = (vel_alvo > base) ? ACEL_STEP : FREIO_STEP;

  // Ajusta velocidade
  if (abs(vel_alvo - base) <= passo) {
    base = vel_alvo;
  }
  else {
    base += (vel_alvo > base) ? passo : -passo;
  }

  return constrain(base, CURVA_VEL, (slow ? VELOCIDADE_5 : BASE_FAST));
}

void detectGeo() {
  Read_hits();

  if ((HL == 0) && (HR == 0)) geo = 0;
  if ((HL == 1) && (HR == 0)) geo = 1;
  if ((HL == 0) && (HR == 1)) geo = 2;
  if ((HL == 1) && (HR == 1)) geo = 3;

  if (l_geo != geo) {
    if (geo == 0 && l_geo == 2 && ll_geo == 0) { funcion_HL(); }
    if (geo == 0 && l_geo == 1 && ll_geo == 0) { funcion_HR(); }
    if (geo == 0 && ((l_geo == 3) || (ll_geo == 3) || (lll_geo == 3))) { funcion_Cruce(); }
    lll_geo = ll_geo;
    ll_geo = l_geo;
    l_geo = geo;
  }

  // Gerencia a sequência de parada
  if (isStoppingSequence && !isStopped) {
    if (!slowdownTimer.expired() && base > VELOCIDADE_3) {
      base = VELOCIDADE_3;
    }
    else if (stopTimer.expired()) {
      base = 0;
      Motores(0, 0);
      isStopped = true;
    }
  }

  DEBUG_PRINT("Geometria: ");
  DEBUG_PRINTLN_VAL(geo);
}

void funcion_HL() {
  fin++;

  switch (fin) {
  case 2:
    if (!isStoppingSequence) {
      isStoppingSequence = true;
      slowdownTimer.start(50);  // 50ms para desaceleração
      stopTimer.start(STOP_DELAY);
    }
    break;
  }
}

void funcion_HR() {
  if (!slow) {
    slow = true;
    base = VELOCIDADE_5;
    em_curva = false;  // Reset do estado de curva
    saindo_curva = false;
    boost_contador = 0;
    DEBUG_PRINTLN("Modo lento ativado");
  }
  else {
    slow = false;
    base = BASE_FAST;
    em_curva = false;  // Reset do estado de curva
    saindo_curva = false;
    boost_contador = 0;
    DEBUG_PRINTLN("Modo rapido ativado");
  }
}

void funcion_Cruce() {
  DEBUG_PRINTLN("Cruzamento detectado");
}