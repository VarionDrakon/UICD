#include "../Headers/Headers.h"

volatile uint8_t sensorState = 0;

#define SENSOR_FIRST_PINOUT 2
#define SENSOR_SECOND_PINOUT 3

void sensorsInitialize() {
  pinMode(SENSOR_FIRST_PINOUT, INPUT_PULLUP);
  pinMode(SENSOR_SECOND_PINOUT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SENSOR_FIRST_PINOUT), counterSensorFirst, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_SECOND_PINOUT), counterSensorSecondary, RISING);
}

void counterSensorFirst() {
  // static uint32_t lastTime = 0;
  // if (millis() - lastTime < 10) return;
  // lastTime = millis();
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
  if (UIDisplaySectionListObject == sectionDefault) UIDisplayNeedRefresh = true;
  // if (sensorState == 0) {
  //   sensorState = 1;
  // } 
  // else if (sensorState == 2) {
  //   counterSensorHandleBackward();
  //   sensorState = 0;
  // }
}

void counterSensorSecondary() {
  // static uint32_t lastTime = 0;
  // if (millis() - lastTime < 10) return;
  // lastTime = millis();
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
  if (UIDisplaySectionListObject == sectionDefault) UIDisplayNeedRefresh = true;
  // if (sensorState == 0) {
  //   sensorState = 2;
  // } 
  // else if (sensorState == 1) {
  //   counterSensorHandleForward();
  //   sensorState = 0;
  // }
}

void counterSensorHandleForward() {
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
  if (UIDisplaySectionListObject == sectionDefault) UIDisplayNeedRefresh = true;
  counterSensorsReset();
}

void counterSensorHandleBackward() {
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
  if (UIDisplaySectionListObject == sectionDefault) UIDisplayNeedRefresh = true;
  counterSensorsReset();
}

void counterSensorsReset() {
  sensorState = 0;
}