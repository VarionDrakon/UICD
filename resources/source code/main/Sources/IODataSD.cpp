#include "../Headers/IODataSD.h"

deviceData deviceDataObject;
deviceDataChar deviceDataCharObject;

volatile uint32_t autSvdTim = 0;                        //Timer for autosave settings.
unsigned long intervalSD = 3000;                        // Timer for write data SD.
// const char* dataFilenameSave = "save.dat";              // File name for data saving/write.
// const char* dataFilenameSaveTemp = "save.dat.temp";     // File name for data saving/write.

void IODataSDFileWrite(const unsigned long &value, const unsigned int offset) {
    Wire.beginTransmission(0x50);
    Wire.write((uint32_t)(offset >> 8));
    Wire.write((uint32_t)(offset & 0xFF));
    // For big-endian EEPROM:
    Wire.write((uint32_t)(value >> 24));
    Wire.write((uint32_t)(value >> 16));
    Wire.write((uint32_t)(value >> 8));
    Wire.write((uint32_t)(value & 0xFF));
    // For little-endian EEPROM:
    // Wire.write((uint32_t)(value & 0xFF));
    // Wire.write((uint32_t)(value >> 8));
    // Wire.write((uint32_t)(value >> 16));
    // Wire.write((uint32_t)(value >> 24));
    Wire.endTransmission();
    delay(5);
}

void IODataSDFileRead(unsigned long &value, const unsigned int offset) {
  Wire.beginTransmission(0x50);
  Wire.write((uint32_t)(offset >> 8));
  Wire.write((uint32_t)(offset & 0xFF));
  Wire.endTransmission();
  delay(5);
  Wire.requestFrom(0x50, 4);
  if (Wire.available() >= 4) {
    unsigned long readValue = 0;
    // For big-endian EEPROM:
    readValue = ((uint32_t)Wire.read()) << 24;
    readValue |= ((uint32_t)Wire.read()) << 16;
    readValue |= ((uint32_t)Wire.read()) << 8;
    readValue |= ((uint32_t)Wire.read());
    // For little-endian EEPROM:
    // readValue = ((uint32_t)Wire.read());
    // readValue |= ((uint32_t)Wire.read()) << 8;
    // readValue |= ((uint32_t)Wire.read()) << 16;
    // readValue |= ((uint32_t)Wire.read()) << 24;
    value = readValue;
  }
  else {
    value = 0;
  }
}

char* totalizerCommonReturn() {
  return ultoa(deviceDataObject.totalizerCommon, deviceDataCharObject.totalizerCommon, 10);
}

char* totalizerDirectReturn() {
  return ultoa(deviceDataObject.totalizerDirect, deviceDataCharObject.totalizerDirect, 10);
}

char* totalizerReverseReturn() {
  return ultoa(deviceDataObject.totalizerReverse, deviceDataCharObject.totalizerReverse, 10);
}

void totalizerCommonValueAdd() {
  deviceDataObject.totalizerCommon++;
}

void totalizerCommonValueReset() {
  deviceDataObject.totalizerCommon = 0;
}

void totalizerDirectValueAdd() {
  deviceDataObject.totalizerDirect++;
}

void totalizerDirectValueReset() {
  deviceDataObject.totalizerDirect = 0;
}

void totalizerReverseValueAdd() {
  deviceDataObject.totalizerReverse++;
}

void totalizerReverseValueReset() {
  deviceDataObject.totalizerReverse = 0;
}

void deviceConfigurationModbusBaudrateSet(unsigned long value) {
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

void IODataSDInitialize() {  
  Wire.begin();
  Wire.setClock(1000000L);

  IODataSDFileRead(deviceDataObject.totalizerCommon, 0);
  IODataSDFileRead(deviceDataObject.totalizerDirect, 4);
  IODataSDFileRead(deviceDataObject.totalizerReverse,8);
  IODataSDFileRead(deviceDataObject.modbusBaudrate, 12);
  IODataSDFileRead(deviceDataObject.modbusSlaveAddress, 16);
}

/*
  This is the code for periodic auto-saving of data to the SD card:
    Saves data to an SD card at set intervals.
    Displays a recording indicator on the display.
  
  Variables:
    currentMillisSaved - current time.
    autSvdTim - time of last autosave.
    intervalSD - interval between saves.
*/
void IODataSDFileWritePeriodically() {
  unsigned long currentMillisSaved = millis();
  if (currentMillisSaved - autSvdTim >= intervalSD) {
    MCPDisplayCursorSet(19, 0);
    MCPDisplayPrint("S");

    IODataSDFileWrite(deviceDataObject.totalizerCommon, 0);
    IODataSDFileWrite(deviceDataObject.totalizerDirect, 4);
    IODataSDFileWrite(deviceDataObject.totalizerReverse,8);
    IODataSDFileWrite(deviceDataObject.modbusBaudrate, 12);
    IODataSDFileWrite(deviceDataObject.modbusSlaveAddress, 16);

    MCPDisplayCursorSet(19, 0);
    MCPDisplayPrint(" ");
    autSvdTim = currentMillisSaved;
  }
}
