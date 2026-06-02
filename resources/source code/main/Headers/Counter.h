#ifndef COUNTER_H
#define COUNTER_H

#include <Arduino.h>

#define COUNTER_MODE_AUTO 0
#define COUNTER_MODE_MANUAL 1

#define COUNTER_DIR_FORWARD 0
#define COUNTER_DIR_BACKWARD 1

volatile extern uint8_t counterMode;
volatile extern uint8_t counterManualDirection;

void sensorsInitialize();
void sensorsAttachAuto();
void sensorsDetachAll();
void counterModeApply();

void counterSensorFirst();
void counterSensorSecondary();
void counterSensorFirstManual();
void counterSensorSecondaryManual();
void counterSensorHandleForward();
void counterSensorHandleBackward();
void counterSensorHandleManual();
#endif