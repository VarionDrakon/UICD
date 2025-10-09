#include <SD.h>
#include "MCPDisplay.h"

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

volatile uint32_t autSvdTim = 0;         //Timer for autosave settings.
unsigned long interval = 5 * 60 * 1000;  // 5 minutes.
unsigned long intervalSD = 3000;         // Timer for write data SD.
char dataFilename[] = "data.dat";        // File name for data saving/write.

bool IODataSDFileWrite(const String& fileName) {
  File dataFile = SD.open(fileName, FILE_WRITE | O_TRUNC | O_APPEND);
  if (dataFile) {
    dataFile.write((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
    dataFile.close();

    return true;
  } else {
    return false;
  }
}

bool IODataSDFileRead(const String& fileName) {
  File dataFile = SD.open(fileName, FILE_READ | O_APPEND);
  if (dataFile) {
    dataFile.read((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
    dataFile.close();

    return true;
  } else {
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
  MCPDisplayCursorSet(3, 1);
  MCPDisplayPrint("UICD loading...");
  delay(500);

  if (!SD.begin(10)) {
    MCPDisplayCommandSend(0x01);
    MCPDisplayCursorSet(5, 1);
    MCPDisplayPrint("SD ERROR!");
    while (1)
      ;
  }

  if (SD.exists(dataFilename)) {
    IODataSDFileRead(dataFilename);

    MCPDisplayCommandSend(0x01);
    delay(10);

    MCPDisplayCursorSet(2, 1);
    MCPDisplayPrint("Load saved file...");
    delay(500);
  } else {
    MCPDisplayCommandSend(0x01);
    delay(10);

    MCPDisplayCursorSet(2, 0);
    MCPDisplayPrint("Device reset?");

    deviceConfigurationModbusBaudrateSet(9600);
    MCPDisplayCursorSet(2, 1);
    MCPDisplayPrint("Baudrate:");
    MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());

    deviceConfigurationModbusSlaveAddressSet(10);
    MCPDisplayCursorSet(2, 2);
    MCPDisplayPrint("Slave Address = 10");
    MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());
    delay(1000);
  }
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

    IODataSDFileWrite(dataFilename);

    MCPDisplayCursorSet(19, 0);
    MCPDisplayPrint(" ");

    autSvdTim = currentMillisSaved;
  }
}