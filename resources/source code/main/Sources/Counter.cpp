#include "../Headers/Headers.h"

volatile uint8_t sensorState = 0;

void sensorsInitialize() {
  attachInterrupt(0, counterSensorFirst, FALLING);
  attachInterrupt(1, counterSensorSecondary, FALLING);
}

void counterSensorFirst() {
  static uint32_t lastTime = 0;
  if (millis() - lastTime < 10) return;
  lastTime = millis();
  
  if (sensorState == 0) {
    sensorState = 1;
  } 
  else if (sensorState == 2) {
    counterSensorHandleBackward();
    sensorState = 0;
  }
}

void counterSensorSecondary() {
  static uint32_t lastTime = 0;
  if (millis() - lastTime < 10) return;
  lastTime = millis();
  
  if (sensorState == 0) {
    sensorState = 2;
  } 
  else if (sensorState == 1) {
    counterSensorHandleForward();
    sensorState = 0;
  }
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