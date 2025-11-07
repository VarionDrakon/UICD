#ifndef COUNTER_H
#define COUNTER_H

#include <Arduino.h>

void sensorsInitialize();
void counterSensorFirst();
void counterSensorSecondary();
void counterSensorHandleForward();
void counterSensorHandleBackward();
void counterSensorsReset();
void counterSensorTimeout();
void counterDirectionHandler();

#endif