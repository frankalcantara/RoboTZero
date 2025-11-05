#include <Arduino.h>
#include "config.h"
#include "debug.h"
#include "Timer.h"

enum ButtonState {
  WAITING_PRESS,
  DEBOUNCING_PRESS,
  WAITING_RELEASE,
  DEBOUNCING_RELEASE,
  BUTTON_HANDLED
};

static ButtonState buttonState = WAITING_PRESS;
static Timer debounceTimer;
static bool lastButtonState = LOW;

void WaitButton() {
  buttonState = WAITING_PRESS;  // Reset do estado inicial

  while (buttonState != BUTTON_HANDLED) {
    bool currentState = digitalRead(PINBUTTON);

    // Se houve mudança no estado do botão
    if (currentState != lastButtonState) {
        // Inicia o timer de debounce
      debounceTimer.start(DEBOUNCE_DELAY);

      // Atualiza estado baseado se está pressionando ou soltando
      if (currentState == HIGH) {
        buttonState = DEBOUNCING_PRESS;
      }
      else {
        buttonState = DEBOUNCING_RELEASE;
      }
    }

    // Verifica se o tempo de debounce passou
    if (debounceTimer.expired()) {
      switch (buttonState) {
      case DEBOUNCING_PRESS:
        if (currentState == HIGH) {
          buttonState = WAITING_RELEASE;
        }
        else {
          buttonState = WAITING_PRESS;
        }
        break;

      case DEBOUNCING_RELEASE:
        if (currentState == LOW) {
          buttonState = BUTTON_HANDLED;
        }
        else {
          buttonState = WAITING_RELEASE;
        }
        break;
      }
    }

    lastButtonState = currentState;
  }
}

void Peripherals_init() {
  pinMode(PINBUTTON, INPUT);
  buttonState = WAITING_PRESS;
}