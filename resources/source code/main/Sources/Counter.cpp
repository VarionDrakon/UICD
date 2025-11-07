#include "../Headers/Headers.h"

volatile unsigned long sensorFirstTime = 0;
volatile unsigned long sensorSecondaryTime = 0;
volatile bool sensorFirstTriggered = false;
volatile bool sensorSecondaryTriggered = false;
volatile int direction = 0; 


const unsigned long DEBOUNCE_TIME = 50;
const unsigned long TIMEOUT = 1000;

void sensorsInitialize() {
  attachInterrupt(0, counterSensorFirst, FALLING);
  attachInterrupt(1, counterSensorSecondary, FALLING);
}

void counterSensorFirst() {
  noInterrupts();
  unsigned long currentTime = millis();
  static unsigned long lastFirstTime = 0;
  
  if (currentTime - lastFirstTime < DEBOUNCE_TIME) return;
  lastFirstTime = currentTime;
  
  sensorFirstTime = currentTime;
  sensorFirstTriggered = true;
  counterDirectionHandler();
  interrupts();
}

void counterSensorSecondary() {
  noInterrupts();
  unsigned long currentTime = millis();
  static unsigned long lastSecondaryTime = 0;
  
  if (currentTime - lastSecondaryTime < DEBOUNCE_TIME) return;
  lastSecondaryTime = currentTime;

  sensorSecondaryTime = currentTime;
  sensorSecondaryTriggered = true;
  counterDirectionHandler();
  interrupts();
}

void counterDirectionHandler() {
  unsigned long currentTime = millis();
  
  if (currentTime - sensorFirstTime > TIMEOUT || currentTime - sensorSecondaryTime > TIMEOUT) {
    counterSensorsReset();
    return;
  }
  
  if (sensorFirstTriggered && sensorSecondaryTriggered) {
    if (sensorFirstTime < sensorSecondaryTime) {
      direction = 1;
      counterSensorHandleForward();
    } else {
      direction = -1;
      counterSensorHandleBackward();
    }
    counterSensorsReset();
  }
}

void counterSensorsReset() {
  sensorFirstTriggered = false;
  sensorSecondaryTriggered = false;
  direction = 0;
}

void counterSensorHandleForward() {
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
  if (UIDisplaySectionListObject == sectionDefault) UIDisplayNeedRefresh = true;
}

void counterSensorHandleBackward() {
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
  if (UIDisplaySectionListObject == sectionDefault) UIDisplayNeedRefresh = true;
}