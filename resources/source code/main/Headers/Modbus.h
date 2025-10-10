#ifndef MODBUS_H
#define MODBUS_H

#define length 9  //Length of sent arrray
#define modbusPin 4

extern uint16_t au16data[length];

void modbusInitialize();

void modbusHandlerListener();

void modbusHandlerResponse();

void modbusSettingsUpdater();

void modbusSettingsUpdater();

#endif