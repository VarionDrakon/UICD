#include "../Headers/IODataSD.h"

deviceData deviceDataObject;
deviceDataChar deviceDataCharObject;

volatile uint32_t autSvdTim = 0;                        //Timer for autosave settings.
unsigned long intervalSD = 3000;                        // Timer for write data SD.
// const char* dataFilenameSave = "save.dat";              // File name for data saving/write.
// const char* dataFilenameSaveTemp = "save.dat.temp";     // File name for data saving/write.

void IODataSDFileWrite(const unsigned long &value) {
  // File dataFile = SD.open(mainFilename, FILE_WRITE | O_TRUNC);

  // if (dataFile) {
  //   dataFile.write((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
  //   dataFile.close();
  // } else {
  //     File dataFile = SD.open(mainFilename, FILE_WRITE | O_TRUNC);

  //     if (dataFile) {
  //       dataFile.write((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
  //       dataFile.close();
  //     } else {
  //       while (1) {
  //         MCPDisplayCommandSend(0x01);
  //         MCPDisplayCursorSet(2, 1);
  //         MCPDisplayPrint("SAVE FILE ERROR!");
  //       }
  //     }
  // }

  // for (int i = 0; i < 5; i++) {
    Wire.beginTransmission(0x50);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.write((uint8_t)(value >> 24));
    Wire.write((uint8_t)(value >> 16));
    Wire.write((uint8_t)(value >> 8));
    Wire.write((uint8_t)(value & 0xFF));
    Wire.endTransmission();
    delay(5);
  // }

}

void IODataSDFileRead(unsigned long &value) {
  // File dataFile = SD.open(mainFilename, FILE_READ);

  // if (dataFile) {
  //   dataFile.read((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
  //   dataFile.close();
  // } else {
  //     File dataFile = SD.open(mainFilename, FILE_READ);

  //     if (dataFile) {
  //       dataFile.read((uint8_t*)&deviceDataObject, sizeof(deviceDataObject));
  //       dataFile.close();
  //     } else {
  //       while (1) {
  //         MCPDisplayCommandSend(0x01);
  //         MCPDisplayCursorSet(2, 1);
  //         MCPDisplayPrint("READ FILE ERROR!");
  //       }
  //     }
  // }

  Wire.beginTransmission(0x50);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(5);

  Wire.requestFrom(0x50, 4);

  if (Wire.available() >= 4) {

    unsigned long readValue = 0;

    readValue = ((uint8_t)Wire.read()) << 24;
    readValue |= ((uint8_t)Wire.read()) << 16;
    readValue |= ((uint8_t)Wire.read()) << 8;
    readValue |= ((uint8_t)Wire.read());

    // char strData[11] = {0};

    // ultoa(readValue, strData, 10);
    
    // MCPDisplayCursorSet(0, 0);
    // MCPDisplayPrint(strData);

    value = readValue;
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
  Wire.begin();
  Wire.setClock(1000000L);

  MCPDisplayCommandSend(0x01);
  delay(10);
  
  MCPDisplayCursorSet(3, 1);
  MCPDisplayPrint("UICD loading...");
  delay(500);

  IODataSDFileRead(deviceDataObject.totalizerCommon);

  // if (!SD.begin(10)) {
  //   MCPDisplayCommandSend(0x01);
  //   MCPDisplayCursorSet(5, 1);
  //   MCPDisplayPrint("SD ERROR!");
  //   while (1);
  // }
  // SPI.setClockDivider(SPI_CLOCK_DIV4);

  // if (SD.exists(dataFilenameSave) || SD.exists(dataFilenameSaveTemp)) {
  //   IODataSDFileRead();

  //   MCPDisplayCommandSend(0x01);
  //   delay(10);

  //   MCPDisplayCursorSet(2, 1);
  //   MCPDisplayPrint("Load saved file...");
  //   delay(500);
  // } else {
  //   MCPDisplayCommandSend(0x01);
  //   delay(10);

  //   MCPDisplayCursorSet(2, 0);
  //   MCPDisplayPrint("Device reset?");
  
  //   deviceDataObject.totalizerCommon = 0;
  //   deviceDataObject.totalizerDirect = 0;
  //   deviceDataObject.totalizerReverse = 0;

  //   deviceConfigurationModbusBaudrateSet(9600);
  //   MCPDisplayCursorSet(2, 1);
  //   MCPDisplayPrint("Baudrate:");
  //   MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());

  //   deviceConfigurationModbusSlaveAddressSet(10);
  //   MCPDisplayCursorSet(2, 2);
  //   MCPDisplayPrint("Slave Address = 10");
  //   MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());
  //   delay(1000);
  // }
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

    IODataSDFileWrite(deviceDataObject.totalizerCommon);

    MCPDisplayCursorSet(19, 0);
    MCPDisplayPrint(" ");
    autSvdTim = currentMillisSaved;
  }
}
