#include "state.h"

SystemState currentState = OFF;
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 1000;  // 1 second

void setup() {
  Serial.begin(115200);
  setupStateControl();
}

void loop() {
  updateState(currentState);
  applyState(currentState);

  unsigned long now = millis();
  if (now - lastPrintTime >= printInterval) {
    lastPrintTime = now;
    Serial.print("Current state: ");
    Serial.println(stateToString(currentState));
  }

  delay(10);  // minor debounce
}