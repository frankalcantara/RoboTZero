#include <Arduino.h>
#include "Peripherals.h"
#include "config.h"
#include "debug.h"
#include "LedPattern.h"

#if DEBUG_LEVEL > 0
#include "FlashManager.h"
#include "FlashReader.h"
#endif

// Static member initialization
Peripherals::ButtonState Peripherals::buttonState = WAITING_PRESS;
Timer Peripherals::debounceTimer;
bool Peripherals::lastButtonState = LOW;

void Peripherals::initialize() {
  pinMode(PIN_START_BUTTON, INPUT);
  buttonState = WAITING_PRESS;
#if DEBUG_LEVEL > 0
  LedPattern::initialize();
#endif
}

void Peripherals::waitForButtonPress() {
  buttonState = WAITING_PRESS;

#if DEBUG_LEVEL > 0
// In debug mode, check if there's a log ready to be transmitted
  if (FlashManager::isLogReady()) {
    bool currentState = digitalRead(PIN_START_BUTTON);

    if (currentState != lastButtonState) {
      debounceTimer.Start(DEBOUNCE_DELAY);
      if (currentState == HIGH) {
        buttonState = DEBOUNCING_PRESS;
      }
    }

    if (debounceTimer.Expired() && buttonState == DEBOUNCING_PRESS) {
        // Start log transmission
      LedPattern::startTransmissionPattern();
      FlashReader::processCommands();
      LedPattern::stopPattern();
      return;
    }
    lastButtonState = currentState;
    return;
  }
#else
// In normal mode, clear the log ready flag
  FlashManager::clearLogReady();
#endif

// Normal button operation for calibration/start
  while (buttonState != BUTTON_HANDLED) {
    bool currentState = digitalRead(PIN_START_BUTTON);

    if (currentState != lastButtonState) {
      debounceTimer.Start(DEBOUNCE_DELAY);

      if (currentState == HIGH) {
        buttonState = DEBOUNCING_PRESS;
      }
      else {
        buttonState = DEBOUNCING_RELEASE;
      }
    }

    if (debounceTimer.Expired()) {
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

      case WAITING_PRESS:
      case WAITING_RELEASE:
      case BUTTON_HANDLED:
      default:
        break;
      }
    }

    lastButtonState = currentState;
  }
}