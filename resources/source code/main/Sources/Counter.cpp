#include "../Headers/Headers.h"

#define SENSOR_FIRST_PINOUT 2
#define SENSOR_SECOND_PINOUT 3

#define STATE_IDLE       0
#define STATE_S1_FIRST   1
#define STATE_S2_FIRST   2
#define STATE_COUNTED    3

volatile uint8_t passageState = STATE_IDLE;

void sensorsInitialize() {
  pinMode(SENSOR_FIRST_PINOUT, INPUT_PULLUP);
  pinMode(SENSOR_SECOND_PINOUT, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirst, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondary, FALLING);
}

void counterSensorFirst() {
  if (passageState == STATE_S1_FIRST) {
    passageState = STATE_IDLE;
    return;
  }
  if (passageState == STATE_IDLE) {
    if (digitalRead(SENSOR_SECOND_PINOUT) == HIGH) {
      passageState = STATE_S1_FIRST;
    }
    return;
  }
  else if (passageState == STATE_S2_FIRST) {
    if (digitalRead(SENSOR_SECOND_PINOUT) == LOW) {
        passageState = STATE_COUNTED;
        counterSensorHandleBackward();
    }
  }
}

void counterSensorSecondary() {
  if (passageState == STATE_S2_FIRST) {
    passageState = STATE_IDLE;
    return;
  }
  if (passageState == STATE_IDLE) {
    if (digitalRead(SENSOR_FIRST_PINOUT) == HIGH) {
      passageState = STATE_S2_FIRST;
    }
    return;
  }
  else if (passageState == STATE_S1_FIRST) {
    if (digitalRead(SENSOR_FIRST_PINOUT) == LOW) {
      passageState = STATE_COUNTED;
      counterSensorHandleForward();
    }
  }
}

void counterSensorHandleForward() {
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
  passageState = STATE_IDLE;
}

void counterSensorHandleBackward() {
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
  passageState = STATE_IDLE;
}
