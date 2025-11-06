#include "../Headers/Headers.h"

enum SensorState {
  counterSensorWaiting,
  counterSensorFirstTriggered,
  counterSensorSecondaryTriggered
};

volatile SensorState state = counterSensorWaiting;
volatile unsigned long counterSensorFirstLastInterruptTime = 0;
volatile unsigned long counterSensorSecondaryLastInterruptTime = 0;
const uint32_t counterSensorMinTriggerInterval = 20;

void sensorsInitialize() {
  attachInterrupt(0, counterSensorFirst, FALLING);
  attachInterrupt(1, counterSensorSecondary, FALLING);
}

void sensorsStateTimeout() {
  unsigned long currentTime = millis();
  if (state != counterSensorWaiting && (currentTime - 0 > 1000)) {
    state = counterSensorWaiting;
  }
}

void counterSensorFirst() {
  static unsigned int counterSensorMinDebounceTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - counterSensorFirstLastInterruptTime > counterSensorMinTriggerInterval) {
    counterSensorFirstLastInterruptTime = currentTime;

      if (currentTime - counterSensorMinDebounceTime < counterSensorMinTriggerInterval) {
        return;
      }
    
    switch(state) {
      case counterSensorWaiting:
        state = counterSensorFirstTriggered;
        break;
      case counterSensorSecondaryTriggered:
        counterSensorHandleBackward();
        state = counterSensorWaiting;
        break;
    }
  }
}

void counterSensorSecondary() {
  static unsigned int counterSensorMinDebounceTime = 0;
  unsigned long currentTime = millis();
  if (currentTime - counterSensorSecondaryLastInterruptTime > counterSensorMinTriggerInterval) {
    counterSensorSecondaryLastInterruptTime = currentTime;
    
    if (currentTime - counterSensorMinDebounceTime < counterSensorMinTriggerInterval) {
      return;
    }

    switch(state) {
      case counterSensorWaiting:
        state = counterSensorSecondaryTriggered;
        break;
      case counterSensorFirstTriggered:
        counterSensorHandleForward();
        state = counterSensorWaiting;
        break;
    }
  }
}

void counterSensorHandleForward() {
  totalizerDirectValueAdd();
  totalizerCommonValueAdd();
  UIDisplayNeedRefresh = true;
}

void counterSensorHandleBackward() {
  totalizerReverseValueAdd();
  totalizerCommonValueAdd();
  UIDisplayNeedRefresh = true;
}