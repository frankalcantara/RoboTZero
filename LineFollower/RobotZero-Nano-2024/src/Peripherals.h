#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "Timer.h"

class Peripherals {
private:
    // Button state machine states
    enum ButtonState {
        WAITING_PRESS,
        DEBOUNCING_PRESS,
        WAITING_RELEASE,
        DEBOUNCING_RELEASE,
        BUTTON_HANDLED
    };

    // Button control variables
    static ButtonState buttonState;
    static Timer debounceTimer;
    static bool lastButtonState;

public:
    // Initialize peripherals
    static void initialize();

    // Wait for button press with debounce
    static void waitForButtonPress();
};

#endif // PERIPHERALS_H