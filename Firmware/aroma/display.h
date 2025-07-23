#pragma once

#include <Arduino.h>
#include "state.h"  // for SystemState and stateToString()

void setupDisplay();
void displayState(SystemState state);
