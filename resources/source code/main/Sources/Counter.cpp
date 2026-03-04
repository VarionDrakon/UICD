#include "../Headers/Headers.h"

#define SENSOR_FIRST_PINOUT 2
#define SENSOR_SECOND_PINOUT 3

static volatile uint32_t lastFirstMicros = 0;
static volatile uint32_t lastSecondMicros = 0;
static const uint32_t debounceTime = 100;

void sensorsInitialize() {
  pinMode(SENSOR_FIRST_PINOUT, INPUT_PULLUP);
  pinMode(SENSOR_SECOND_PINOUT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirst, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondary, RISING);
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