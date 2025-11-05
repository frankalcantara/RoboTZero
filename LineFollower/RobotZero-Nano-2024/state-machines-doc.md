# State Machines Analysis

## 1. Setup State Machine

Located in `main.cpp`, this state machine handles the robot's initialization process.

### States:
```cpp
enum SetupState {
    SETUP_INIT,
    SETUP_WAIT1,
    SETUP_WAIT2,
    SETUP_BUTTON1,
    SETUP_CALIBRATION,
    SETUP_BUTTON2,
    SETUP_COMPLETE
}
```

### State Flow:
1. **SETUP_INIT**: 
   - Initializes setup timer
   - Transitions to SETUP_WAIT1

2. **SETUP_WAIT1**:
   - Waits for initial delay
   - Turns LED off and stops motors
   - Transitions to SETUP_WAIT2

3. **SETUP_WAIT2**:
   - Additional delay for system stabilization
   - Transitions to SETUP_BUTTON1

4. **SETUP_BUTTON1**:
   - Waits for first button press
   - Turns LED on
   - Transitions to SETUP_CALIBRATION

5. **SETUP_CALIBRATION**:
   - Performs sensor calibration
   - Turns LED off
   - Transitions to SETUP_BUTTON2

6. **SETUP_BUTTON2**:
   - Waits for second button press
   - Turns LED on
   - Transitions to SETUP_COMPLETE

## 2. Button State Machine

Located in `Peripherals.cpp`, this state machine manages button debouncing and press/release detection.

### States:
```cpp
enum ButtonState {
  WAITING_PRESS,
  DEBOUNCING_PRESS,
  WAITING_RELEASE,
  DEBOUNCING_RELEASE,
  BUTTON_HANDLED
}
```

### State Flow:
1. **WAITING_PRESS**:
   - Initial state
   - Monitors for button press
   - On press: Transitions to DEBOUNCING_PRESS

2. **DEBOUNCING_PRESS**:
   - Waits for debounce delay (`DEBOUNCE_DELAY`)
   - If button still pressed: Transitions to WAITING_RELEASE
   - If button released: Returns to WAITING_PRESS

3. **WAITING_RELEASE**:
   - Monitors for button release
   - On release: Transitions to DEBOUNCING_RELEASE

4. **DEBOUNCING_RELEASE**:
   - Waits for debounce delay
   - If button still released: Transitions to BUTTON_HANDLED
   - If button pressed: Returns to WAITING_RELEASE

5. **BUTTON_HANDLED**:
   - Final state indicating successful button press cycle

### Implementation Details:

The button state machine uses a Timer class for debouncing:
```cpp
void WaitButton() {
  buttonState = WAITING_PRESS;
  
  while (buttonState != BUTTON_HANDLED) {
    bool currentState = digitalRead(PINBUTTON);

    if (currentState != lastButtonState) {
      debounceTimer.start(DEBOUNCE_DELAY);
      buttonState = currentState == HIGH ? DEBOUNCING_PRESS : DEBOUNCING_RELEASE;
    }

    if (debounceTimer.expired()) {
      switch (buttonState) {
        case DEBOUNCING_PRESS:
          buttonState = currentState == HIGH ? WAITING_RELEASE : WAITING_PRESS;
          break;
          
        case DEBOUNCING_RELEASE:
          buttonState = currentState == LOW ? BUTTON_HANDLED : WAITING_RELEASE;
          break;
      }
    }

    lastButtonState = currentState;
  }
}
```

## Key Design Patterns

1. **Timer Usage**: Both state machines use the `Timer` class for non-blocking delays:
```cpp
class Timer {
    void start(unsigned long ms) {
        startTime = millis();
        duration = ms;
        running = true;
    }

    bool expired() {
        if (!running) return false;
        return (millis() - startTime >= duration);
    }
}
```

2. **State Protection**: Both machines use enumerated states to prevent invalid transitions.

3. **Non-Blocking Design**: The state machines operate without using blocking delays, allowing the system to remain responsive.