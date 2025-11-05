#include "../Headers/Headers.h"

enum SensorState {
  counterSensorWaiting,
  counterSensorFirstTriggered,
  counterSensorSecondaryTriggered
};

volatile SensorState state = counterSensorWaiting;
volatile unsigned long counterSensorFirstLastInterruptTime = 0;
volatile unsigned long counterSensorSecondaryLastInterruptTime = 0;
const uint32_t counterSensorMinTriggerInterval = 3;

void sensorsInitialize() {
  attachInterrupt(0, counterSensorFirst, FALLING);
  attachInterrupt(1, counterSensorSecondary, FALLING);
}

void counterSensorFirst() {
  unsigned long currentTime = millis();
  if (currentTime - counterSensorFirstLastInterruptTime > counterSensorMinTriggerInterval) {
    counterSensorFirstLastInterruptTime = currentTime;
    
    switch(state) {
      case counterSensorWaiting:
        state = counterSensorFirstTriggered;
        break;
      case counterSensorSecondaryTriggered:
        counterSensorHandleBackward();
        state = counterSensorWaiting;
        break;
      default:
        break;
    }
  }
}

void counterSensorSecondary() {
  unsigned long currentTime = millis();
  if (currentTime - counterSensorSecondaryLastInterruptTime > counterSensorMinTriggerInterval) {
    counterSensorSecondaryLastInterruptTime = currentTime;
    
    switch(state) {
      case counterSensorWaiting:
        state = counterSensorSecondaryTriggered;
        break;
      case counterSensorFirstTriggered:
        counterSensorHandleForward();
        state = counterSensorWaiting;
        break;
      default:
        break;
    }
  }
}

void counterSensorHandleForward() {
  UIDisplayNeedRefresh = true;
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
}

void counterSensorHandleBackward() {
  UIDisplayNeedRefresh = true;
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
}