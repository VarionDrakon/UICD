#ifndef MODBUS_H
#define MODBUS_H

#define length 9  //Length of sent arrray
#define modbusPin 4

extern uint16_t au16data[length];
extern int modbusBaudrateListIndex;
extern const int modbusBaudrateListIndexLimit;
extern uint32_t modbusBaudrateList[];
extern bool modbusBaudrateValueFound;

extern byte modbusSlaveAddressValueCurrent;
extern uint32_t modbusBaudrateValueCurrent;

void modbusInitialize();

void modbusHandlerReloader();

void modbusHandlerListener();

void modbusHandlerResponse();

void modbusSettingsUpdater();

void modbusSettingsUpdater();

#endif