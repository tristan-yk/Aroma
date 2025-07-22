#pragma once

#include <Arduino.h>

enum SystemState {
  OFF,
  ACTIVE_1,
  ACTIVE_2,
  ACTIVE_3,
  ACTIVE_4,
  ACTIVE_5
};

void setupStateControl();
void updateState(SystemState& state);
void applyState(SystemState state);
const char* stateToString(SystemState state);
