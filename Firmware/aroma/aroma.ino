#include "state.h"
#include "display.h"

SystemState currentState = OFF;
unsigned long lastPrintTime = 0;
const unsigned long printInterval = 1000;  // 1 second

void setup() {
  delay(200);
  Serial.begin(115200);
  setupStateControl();
  currentState = OFF;
  setupDisplay();

}

void loop() {
  updateState(currentState);
  applyState(currentState);

  unsigned long now = millis();
  if (now - lastPrintTime >= printInterval) {
    lastPrintTime = now;

    Serial.print("Current state: ");
    Serial.println(stateToString(currentState));

    displayState(currentState);  // now uses display.cpp
  }
}