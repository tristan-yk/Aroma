#include "sensor.h"
#include <Arduino.h>

#define WATER_SENSOR_PIN 36        // GPIO36 = ADC1_CH0
#define FULL_THRESHOLD   2000      // tweak based on actual readings

void setupWaterSensor() {
  pinMode(WATER_SENSOR_PIN, INPUT);
}

bool waterSensorFull() {
  int reading = analogRead(WATER_SENSOR_PIN);
  return reading > FULL_THRESHOLD;  // High analog = full
}
