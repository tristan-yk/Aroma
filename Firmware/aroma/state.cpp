#include "state.h"

// Adjust these to your actual pins
const int SWITCH_PINS[5] = {39, 34, 33, 32, 35};
const int OUTPUT_PINS[5] = {25, 27, 14, 12, 13};
const int FAN_PIN = 2;

static bool lastPressed[5] = {false, false, false, false, false};  // edge detection

const char* stateToString(SystemState state) {
  switch (state) {
    case OFF:      return "OFF";
    case ACTIVE_1: return "ACTIVE_1";
    case ACTIVE_2: return "ACTIVE_2";
    case ACTIVE_3: return "ACTIVE_3";
    case ACTIVE_4: return "ACTIVE_4";
    case ACTIVE_5: return "ACTIVE_5";
    default:       return "UNKNOWN";
  }
}

void setupStateControl() {
  for (int i = 0; i < 5; i++) {
    pinMode(SWITCH_PINS[i], INPUT_PULLUP);
    pinMode(OUTPUT_PINS[i], OUTPUT);
    digitalWrite(OUTPUT_PINS[i], LOW); // start OFF
  }
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
}

void updateState(SystemState& state) {
  for (int i = 0; i < 5; i++) {
    bool pressed = (digitalRead(SWITCH_PINS[i]) == LOW);

    if (pressed && !lastPressed[i]) {
      if (state == (SystemState)(ACTIVE_1 + i)) {
        state = OFF;
      } else {
        state = (SystemState)(ACTIVE_1 + i);
      }
    }

    lastPressed[i] = pressed;
  }
}

void applyState(SystemState state) {
  for (int i = 0; i < 5; i++) {
    digitalWrite(OUTPUT_PINS[i], LOW);
  }
  digitalWrite(FAN_PIN, LOW);

  

  switch (state) {
    case ACTIVE_1: 
      digitalWrite(OUTPUT_PINS[0], HIGH); 
      digitalWrite(FAN_PIN, HIGH);
      break;
    case ACTIVE_2: 
      digitalWrite(OUTPUT_PINS[1], HIGH); 
      digitalWrite(FAN_PIN, HIGH);
      break;
    case ACTIVE_3: 
      digitalWrite(OUTPUT_PINS[2], HIGH); 
      digitalWrite(FAN_PIN, HIGH);
      break;
    case ACTIVE_4: 
      digitalWrite(OUTPUT_PINS[3], HIGH);
      digitalWrite(FAN_PIN, HIGH);
      break;
    case ACTIVE_5: 
      digitalWrite(OUTPUT_PINS[4], HIGH); 
      digitalWrite(FAN_PIN, HIGH);
      break;
    case OFF:
      break;
    default: 
      break;
  }

}
