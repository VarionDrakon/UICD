#ifndef MODBUS_H
#define MODBUS_H

#define length 9  //Length of sent arrray
#define modbusPin 4

extern uint16_t au16data[length];
extern int modbusBaudrateListIndex;
extern uint32_t modbusBaudrateList[];
extern int modbusBaudrateListIndexLimit;
extern bool modbusBaudrateListNotTrue;

void modbusInitialize();

void modbusHandlerListener();

void modbusHandlerResponse();

void modbusSettingsUpdater();

void modbusSettingsUpdater();

#endif