#include "../Headers/Headers.h"

#define SENSOR_FIRST_PINOUT 2
#define SENSOR_SECOND_PINOUT 3

void sensorsInitialize() {
  pinMode(SENSOR_FIRST_PINOUT, INPUT_PULLUP);
  pinMode(SENSOR_SECOND_PINOUT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirst, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondary, FALLING);
}

void counterSensorFirst() {
  if (digitalRead(SENSOR_SECOND_PINOUT) == LOW) {
    counterSensorHandleForward();
  }
}

void counterSensorSecondary() {
  if (digitalRead(SENSOR_FIRST_PINOUT) == LOW) {
    counterSensorHandleBackward();
  }
}

void counterSensorHandleForward() {
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
}

void counterSensorHandleBackward() {
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
}