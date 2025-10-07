#include "SD.h"

struct deviceData {
  unsigned long totalizerCommon;
  unsigned long totalizerDirect;
  unsigned long totalizerReverse;
  unsigned long modbusBaudrate;
  byte modbusSlaveAddress;
};
deviceData deviceDataObject;

struct deviceDataChar {
  char totalizerCommon[32];
  char totalizerDirect[32];
  char totalizerReverse[32];
  char modbusBaudrate[32];
  char modbusSlaveAddress[8];
};
deviceDataChar deviceDataCharObject;

char dataFilename[] = "data.dat";

bool writeDataFile(const String& fileName) {
  File dataFile = SD.open(fileName, FILE_WRITE | O_TRUNC | O_APPEND);
  if (dataFile) {
    dataFile.write((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
    dataFile.close();

    Serial.println("Write data - success!");
    return true;
  }
  else{
    Serial.println("Write data - ERROR!");
    return false;
  }
}

bool readDataFile(const String& fileName) {
  File dataFile = SD.open(fileName, FILE_READ | O_APPEND);
  if (dataFile) {
    dataFile.read((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
    dataFile.close();
    Serial.println("Read file - success!");
    return true;
  } else {
    Serial.println("Read file - ERROR!");
    return false;
  }
}

char* totalizerCommonReturn() {
  return ltoa(deviceDataObject.totalizerCommon, deviceDataCharObject.totalizerCommon, 10);
}

char* totalizerDirectReturn() {
  return ltoa(deviceDataObject.totalizerDirect, deviceDataCharObject.totalizerDirect, 10);
}

char* totalizerReverseReturn() {
  return ltoa(deviceDataObject.totalizerReverse, deviceDataCharObject.totalizerReverse, 10);
}

void totalizerCommonValueAdd() {
  deviceDataObject.totalizerCommon++;
}

void totalizerDirectValueAdd() {
  deviceDataObject.totalizerDirect++;
}

void totalizerReverseValueAdd() {
  deviceDataObject.totalizerReverse++;
}

void deviceConfigurationModbusBaudrateSet(byte value) {
  deviceDataObject.modbusBaudrate = value;
}

void deviceConfigurationModbusSlaveAddressSet(byte value) {
  deviceDataObject.modbusSlaveAddress = value;
}

char* deviceConfigurationModbusBaudrateGet() {
  return ltoa(deviceDataObject.modbusBaudrate, deviceDataCharObject.modbusBaudrate, 10);
}

char* deviceConfigurationModbusSlaveAddressGet() {
  return ltoa(deviceDataObject.modbusSlaveAddress, deviceDataCharObject.modbusSlaveAddress, 10);
}