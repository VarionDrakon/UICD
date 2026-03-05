#ifndef IODATASD_H
#define IODATASD_H

#include <SD.h>
#include <SPI.h>

#include <Wire.h>

#include "MCPDisplay.h"

struct deviceData {
  unsigned long totalizerCommon;  // Max: 4294967295
  unsigned long totalizerDirect;  // Max: 4294967295 
  unsigned long totalizerReverse; // Max: 4294967295 
  unsigned long modbusBaudrate;
  unsigned long modbusSlaveAddress;
};
extern deviceData deviceDataObject;

struct deviceDataChar {
  char totalizerCommon[32];
  char totalizerDirect[32];
  char totalizerReverse[32];
  char modbusBaudrate[32];
  char modbusSlaveAddress[8];
};
extern deviceDataChar deviceDataCharObject;

struct deviceDataShadow {
  // unsigned long totalizerCommon;
  unsigned long totalizerDirect;
  unsigned long totalizerReverse;
  bool resetRequestExternal;
};
extern deviceDataShadow deviceDataShadowObject;

extern volatile bool deviceDataObjectIsBusy;
extern const unsigned long watchdogTimeout;
extern volatile unsigned long watchdogTimeSignalRefresh;

extern volatile uint32_t autSvdTim;
extern unsigned long intervalSD;

void IODataSDFileWrite(const unsigned long &value, const unsigned int offset);
void IODataSDFileRead(unsigned long &value, const unsigned int offset);
char* totalizerCommonReturn();
char* totalizerDirectReturn();
char* totalizerReverseReturn();
void totalizerCommonValueAdd();
void totalizerCommonValueReset();
void totalizerDirectValueAdd();
void totalizerDirectValueReset();
void totalizerReverseValueAdd();
void totalizerReverseValueReset();
void modbusSlaveAddressAdd();
void modbusSlaveAddressReduce();
void deviceConfigurationModbusBaudrateSet(unsigned long value);
void deviceConfigurationModbusSlaveAddressSet(byte value);
char* deviceConfigurationModbusBaudrateGet();
char* deviceConfigurationModbusSlaveAddressGet();
void IODataSDInitialize();
void IODataSDFileWritePeriodically();
void IODataWatchdogHandler();

#endif