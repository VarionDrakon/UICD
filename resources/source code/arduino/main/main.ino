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
unsigned long intervalSD = 10000;                                     // Timer for write data SD

//Default settings
Modbus slave(10, 9600, TXEN);
char dataFilename[] = "data.dat";

void setup() {
  slave.begin(9600);               //ModBus speed
  Serial.begin(9600, SERIAL_8N1);  //Serial settings

  MCPDisplayInitialize(0x26, 20, 4);
  
  MCPDisplayCursorSet(3, 1);
  MCPDisplayPrint("UICD loading...");

  if (!SD.begin(10)) {
    MCPDisplayCursorSet(3, 1);
    MCPDisplayPrint("SD ERROR!");
    return;
  }

  attachInterrupt(0, frtIntr, FALLING);
  attachInterrupt(1, scdIntr, FALLING);

  if (SD.exists(dataFilename)) {
    readDataFile(dataFilename);
    // printData();
  }
  else {
    Serial.println("Device reset?-----------");
    commonData.savedBaudrate = 9600;
    commonData.savedSlaveAddr = 10;
    // printData();
  }

  au16data[0] = commonData.savedSlaveAddr;
  au16data[1] = (commonData.savedBaudrate >> 16) & 0xFFFF;
  au16data[2] = commonData.savedBaudrate & 0xFFFF;
}

void loop() {
  //Modbus handler
  slave.poll(au16data, length);  //Max read elements
  //realize struct and write data on SD-card IT`S SLOW DOWN WORK MICROCONTROLLER!
  if (forv) {
    Serial.println("Forv");
    commonData.savedFrwBott++;
    commonData.savedGnrBott++;
    forv = false;
    isIntrTrg_1 = false;
    isIntrTrg_2 = false;
  }
  if (back) {
    Serial.println("Back");
    commonData.savedRvrBott++;
    commonData.savedGnrBott++;
    back = false;
    isIntrTrg_1 = false;
    isIntrTrg_2 = false;
  }

  commonData.savedSlaveAddr = au16data[0];
  commonData.savedBaudrate = au16data[1];
  commonData.savedBaudrate = (commonData.savedBaudrate << 16) | au16data[2];

  unsigned long currentMillisSaved = millis();
  if (currentMillisSaved - autSvdTim >= intervalSD) {
    writeDataFile(dataFilename);
    // printData();
    autSvdTim = currentMillisSaved;
  }

  //Splitting into bytes array from long variable
  if (!commonData.savedGnrBott == 0) {
    au16data[3] = (commonData.savedGnrBott >> 16) & 0xFFFF;  //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[4] = commonData.savedGnrBott & 0xFFFF;          //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
    au16data[5] = (commonData.savedFrwBott >> 16) & 0xFFFF;  //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[6] = commonData.savedFrwBott & 0xFFFF;          //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
    au16data[7] = (commonData.savedRvrBott >> 16) & 0xFFFF;  //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[8] = commonData.savedRvrBott & 0xFFFF;          //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
  }
}
