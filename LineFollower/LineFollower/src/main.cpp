#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include "Timer.h"
#include "globals.h"

// Declare as funções externas
void TB6612FNG_init();
void Peripherals_init();
void calibracion();
void WaitButton();
void Motores(int left, int right);
void detectGeo();
int GetPos();
int controleVelocidade(int erro);

// Inicialização das variáveis globais
int base = 0;
bool isStopped = false;
bool isStoppingSequence = false;
int fin = 0;
bool slow = false;

int posicion_ideal = POSICION_IDEAL_DEFAULT;
float Kprop = KPROP_DEFAULT;
float Kderiv = KDERIV_DEFAULT;
float alpha = ALPHA_DEFAULT;
int d_error_filtered = 0;
int error_pasado = 0;

// Estados do setup
enum SetupState {
    SETUP_INIT,
    SETUP_WAIT1,
    SETUP_WAIT2,
    SETUP_BUTTON1,
    SETUP_CALIBRATION,
    SETUP_BUTTON2,
    SETUP_COMPLETE
};

// Variáveis de controle do setup
SetupState setupState = SETUP_INIT;
Timer setupTimer;

void setup() {
    DEBUG_BEGIN(115200);
    DEBUG_PRINTLN("Iniciando setup");

    // Inicializa hardware
    Peripherals_init();
    TB6612FNG_init();
    pinMode(PINLED, OUTPUT);

    // Loop de setup não bloqueante
    while (setupState != SETUP_COMPLETE) {
        switch (setupState) {
        case SETUP_INIT:
            setupTimer.start(SETUP_DELAY);
            setupState = SETUP_WAIT1;
            break;

        case SETUP_WAIT1:
            if (setupTimer.expired()) {
                digitalWrite(PINLED, LOW);
                Motores(0, 0);
                setupTimer.start(SETUP_DELAY);
                setupState = SETUP_WAIT2;
            }
            break;

        case SETUP_WAIT2:
            if (setupTimer.expired()) {
                setupState = SETUP_BUTTON1;
            }
            break;

        case SETUP_BUTTON1:
            WaitButton();
            digitalWrite(PINLED, HIGH);
            setupState = SETUP_CALIBRATION;
            break;

        case SETUP_CALIBRATION:
            calibracion();
            digitalWrite(PINLED, LOW);
            setupState = SETUP_BUTTON2;
            break;

        case SETUP_BUTTON2:
            WaitButton();
            digitalWrite(PINLED, HIGH);
            setupState = SETUP_COMPLETE;
            break;
        }
    }

    // Inicializa todas as variáveis do PID e controle
    error_pasado = 0;
    d_error_filtered = 0;
    base = BASE_FAST;
    extern int fin;
    fin = 0;

    DEBUG_PRINTLN("Setup concluido");
}

void loop() {
    // Detecta geometria para atualizar situação
    detectGeo();

    // Se o robô estiver parado, não executa o controle
    extern bool isStopped;
    if (isStopped) {
        Motores(0, 0);
        return;
    }

    // Obtém posição atual e calcula erro
    int p = GetPos();
    int error = p - posicion_ideal;

    // Atualiza velocidade base usando novo controle
    base = controleVelocidade(error);

    // Calcula a derivada do erro e filtra
    int d_error = error - error_pasado;
    d_error_filtered = alpha * d_error + (1 - alpha) * d_error_filtered;

    // Calcula correção PID
    float proportional_term = Kprop * error;
    float derivative_term = Kderiv * d_error_filtered;
    float total_correction = proportional_term + derivative_term;

    // Ajusta ganho da correção baseado na velocidade
    float speed_factor = (base > 200) ? 0.85f : 1.0f;
    int correction_power = constrain(int(total_correction * speed_factor), -255, 255);

    // Aplica correção aos motores
    int left_power = constrain(base + correction_power, -255, 255);
    int right_power = constrain(base - correction_power, -255, 255);

    Motores(left_power, right_power);
    error_pasado = error;

    // Debug se necessário
    DEBUG_PRINT("Base: ");
    DEBUG_PRINT_VAL(base);
    DEBUG_PRINT(" Error: ");
    DEBUG_PRINT_VAL(error);
    DEBUG_PRINT(" Correction: ");
    DEBUG_PRINTLN_VAL(correction_power);
}