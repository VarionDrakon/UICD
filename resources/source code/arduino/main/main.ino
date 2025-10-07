#include "Headers/Counter.h"
#include "Headers/MCPDisplay.h"
#include "Headers/Debug.h"
#include "Headers/IODataSD.h"
#include "Headers/ModBus.h"
#include "Libs/ModbusRtu.h"  //Modbus lib
#include <SPI.h>

#define TXEN 4
#define length 9  //Length of sent arrray
/*
Element (9):
[0] - slave addres;
[1], [2] - baudrate;
[3], [4] - totalize 0 - GENERAL;
[5], [6] - totalize 1 - FORWARD;
[7], [8] - totalize 2 - REVERSE;
*/
volatile uint16_t au16data[length] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //Element [X] - Reserve for future
volatile uint32_t autSvdTim = 0;                                     //Timer for autosave settings
unsigned long interval = 5 * 60 * 1000;                              // 5 minutes
unsigned long intervalSD = 10000;                                    // Timer for write data SD

//Default settings
Modbus slave(10, 9600, TXEN);
// Modbus slave(10, 9600, TXEN);
// char dataFilename[] = "data.dat";

void setup() {

  if (!SD.begin(10)) {
    MCPDisplayCursorSet(3, 1);
    MCPDisplayPrint("SD ERROR!");
    return;
  }

  MCPDisplayInitialize(0x26, 20, 4);

  MCPDisplayCursorSet(3, 1);
  MCPDisplayPrint("UICD loading...");
  delay(500);

  attachInterrupt(0, frtIntr, FALLING);
  attachInterrupt(1, scdIntr, FALLING);

  if (SD.exists(dataFilename)) {
    readDataFile(dataFilename);

    MCPDisplayCommandSend(0x01);
    delay(10);
    MCPDisplayCursorSet(2, 1);
    MCPDisplayPrint("Load saved file...");
    delay(500);
  } else {
    MCPDisplayCommandSend(0x01);
    delay(10);

    MCPDisplayCursorSet(3, 0);
    MCPDisplayPrint("Device reset?");

    deviceConfigurationModbusBaudrateSet(9600);
    MCPDisplayCursorSet(3, 1);
    MCPDisplayPrint("Baudrate = 9600");

    MCPDisplayCursorSet(3, 2);
    MCPDisplayPrint("Slave Address = 10");
    deviceConfigurationModbusSlaveAddressSet(10);
    delay(1000);
  }

  slave = Modbus(deviceDataObject.modbusSlaveAddress, 
                deviceDataObject.modbusBaudrate, 
                TXEN);

  slave.begin();  //ModBus speed
  Serial.begin(deviceDataObject.modbusBaudrate, SERIAL_8N1);  //Serial settings

  au16data[0] = deviceDataObject.modbusSlaveAddress;
  au16data[1] = (deviceDataObject.modbusBaudrate >> 16) & 0xFFFF;
  au16data[2] = deviceDataObject.modbusBaudrate & 0xFFFF;

  MCPDisplayCommandSend(0x01);
  delay(10);

  MCPDisplayCursorSet(0, 0);
  MCPDisplayPrint("B: ");
  MCPDisplayCursorSet(4, 0);
  MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());
  MCPDisplayCursorSet(11, 0);
  MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());

  MCPDisplayCursorSet(0, 1);
  MCPDisplayPrint("C:");
  MCPDisplayCursorSet(4, 1);
  MCPDisplayPrint(totalizerCommonReturn());

  MCPDisplayCursorSet(0, 2);
  MCPDisplayPrint("D:");
  MCPDisplayCursorSet(4, 2);
  MCPDisplayPrint(totalizerDirectReturn());

  MCPDisplayCursorSet(0, 3);
  MCPDisplayPrint("R:");
  MCPDisplayCursorSet(4, 3);
  MCPDisplayPrint(totalizerReverseReturn());

}

void loop() {
  //Modbus handler
  slave.poll(au16data, length);  //Max read elements
  //realize struct and write data on SD-card IT`S SLOW DOWN WORK MICROCONTROLLER!
  if (forv) {
    totalizerDirectValueAdd();
    totalizerCommonValueAdd();
    forv = false;
    isIntrTrg_1 = false;
    isIntrTrg_2 = false;

    MCPDisplayCursorSet(4, 1);
    MCPDisplayPrint(totalizerCommonReturn());
    MCPDisplayCursorSet(4, 2);
    MCPDisplayPrint(totalizerDirectReturn());
  }
  if (back) {
    totalizerReverseValueAdd();
    totalizerCommonValueAdd();
    back = false;
    isIntrTrg_1 = false;
    isIntrTrg_2 = false;

    MCPDisplayCursorSet(4, 1);
    MCPDisplayPrint(totalizerCommonReturn());
    MCPDisplayCursorSet(4, 3);
    MCPDisplayPrint(totalizerReverseReturn());
  }

  deviceDataObject.modbusSlaveAddress = au16data[0];
  deviceDataObject.modbusBaudrate = au16data[1];
  deviceDataObject.modbusBaudrate = (deviceDataObject.modbusBaudrate << 16) | au16data[2];

  unsigned long currentMillisSaved = millis();
  if (currentMillisSaved - autSvdTim >= intervalSD) {
    writeDataFile(dataFilename);
    // printData();
    autSvdTim = currentMillisSaved;
  }

  //Splitting into bytes array from long variable
  if (!deviceDataObject.totalizerCommon == 0) {
    au16data[3] = (deviceDataObject.totalizerCommon >> 16) & 0xFFFF;   //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[4] = deviceDataObject.totalizerCommon & 0xFFFF;           //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
    au16data[5] = (deviceDataObject.totalizerDirect >> 16) & 0xFFFF;   //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[6] = deviceDataObject.totalizerDirect & 0xFFFF;           //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
    au16data[7] = (deviceDataObject.totalizerReverse >> 16) & 0xFFFF;  //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[8] = deviceDataObject.totalizerReverse & 0xFFFF;          //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
  }
}
