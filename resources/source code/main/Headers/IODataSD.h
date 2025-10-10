#ifndef IODATASD_H
#define IODATASD_H

#include <SD.h>
#include <SPI.h>

#include "MCPDisplay.h"

struct deviceData {
  unsigned long totalizerCommon;
  unsigned long totalizerDirect;
  unsigned long totalizerReverse;
  unsigned long modbusBaudrate;
  byte modbusSlaveAddress;
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

extern volatile uint32_t autSvdTim;
extern unsigned long intervalSD;
extern char dataFilename[];

void IODataSDFileWrite(const String& fileName);
void IODataSDFileRead(const String& fileName);
char* totalizerCommonReturn();
char* totalizerDirectReturn();
char* totalizerReverseReturn();
void totalizerCommonValueAdd();
void totalizerDirectValueAdd();
void totalizerReverseValueAdd();
void deviceConfigurationModbusBaudrateSet(unsigned long value);
void deviceConfigurationModbusSlaveAddressSet(byte value);
char* deviceConfigurationModbusBaudrateGet();
char* deviceConfigurationModbusSlaveAddressGet();
void IODataSDInitialize();
void IODataSDFileWritePeriodically();


#endif