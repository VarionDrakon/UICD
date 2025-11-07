#include "../Headers/Headers.h"

// Переменные для отслеживания состояния датчиков
volatile bool sensorFirstTriggered = false;
volatile bool sensorSecondaryTriggered = false;
volatile unsigned long lastSensorTime = 0;
const unsigned long SENSOR_TIMEOUT = 1000; // таймаут между срабатываниями в мс

void sensorsInitialize() {
  attachInterrupt(0, counterSensorFirst, FALLING);
  attachInterrupt(1, counterSensorSecondary, FALLING);
}

void counterSensorFirst() {
  if (millis() - lastSensorTime > 1) { // защита от дребезга
    sensorFirstTriggered = true;
    lastSensorTime = millis();
    counterDirectionHandler();
  }
}

void counterSensorSecondary() {
  if (millis() - lastSensorTime > 1) { // защита от дребезга
    sensorSecondaryTriggered = true;
    lastSensorTime = millis();
    counterDirectionHandler();
  }
}

void counterDirectionHandler() {
  if (sensorFirstTriggered && sensorSecondaryTriggered) {
    // Оба датчика сработали, определяем направление
    if (sensorFirstTriggered && sensorSecondaryTriggered) {
      // Порядок срабатывания определяется по времени
      // В реальной реализации нужно добавить проверку времени срабатывания
      counterSensorHandleForward(); // или counterSensorHandleBackward()
      counterSensorsReset();
    }
  }
  
  // Таймаут для сброса если сработал только один датчик
  counterSensorTimeout();
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

void counterSensorsReset() {
  sensorFirstTriggered = false;
  sensorSecondaryTriggered = false;
}

void counterSensorTimeout() {
  static unsigned long timeoutStart = 0;
  static bool timeoutActive = false;
  
  if ((sensorFirstTriggered || sensorSecondaryTriggered) && !timeoutActive) {
    timeoutStart = millis();
    timeoutActive = true;
  }
  
  if (timeoutActive && (millis() - timeoutStart > SENSOR_TIMEOUT)) {
    counterSensorsReset();
    timeoutActive = false;
  }
}