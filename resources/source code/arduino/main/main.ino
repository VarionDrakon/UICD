#include "Headers/Headers.h"
#include "Libs/Libs.h"
#include "Sources/Sources.cpp"

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
unsigned long intervalSD = 3000;                                     // Timer for write data SD

//Default settings
Modbus slave(10, 9600, TXEN);
// Modbus slave(10, 9600, TXEN);
// char dataFilename[] = "data.dat";

byte currentSlaveAddress = 10;
unsigned long currentBaudrate = 9600;

void setup() {

  MCPDisplayInitialize(0x26, 20, 4);

  IODataSDInitialize();

  currentSlaveAddress = deviceDataObject.modbusSlaveAddress;
  currentBaudrate = deviceDataObject.modbusBaudrate;

  attachInterrupt(0, frtIntr, FALLING);
  attachInterrupt(1, scdIntr, FALLING);

  slave = Modbus(currentSlaveAddress, currentBaudrate, TXEN);
  // slave = Modbus(deviceDataObject.modbusSlaveAddress, deviceDataObject.modbusBaudrate, TXEN); //ModBus initialize.
  slave.begin();
  Serial.begin(currentBaudrate, SERIAL_8N1);
  // Serial.begin(deviceDataObject.modbusBaudrate, SERIAL_8N1);  //Serial settings

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

volatile bool back = false;             //Triggered sensor 1
volatile bool forv = false;             //Triggered sensor 2
volatile bool isIntrTrg_1 = false;      //Triggered sensor 1
volatile bool isIntrTrg_2 = false;      //Triggered sensor 2
volatile uint32_t lstIntrTim_1 = 0;     //Timer for sensor 1
volatile uint32_t lstIntrTim_2 = 0;     //Timer for sensor 2
const unsigned long debounceDelay = 50; // Debounce delay. OLD varian - if (millis() - secDbn >= 100 && digitalRead(3))

void frtIntr() {
  unsigned long currentMillis = millis();

  if (currentMillis - lstIntrTim_1 >= debounceDelay) {

    // isIntrTrg_1 = true;
    lstIntrTim_1 = currentMillis;

    // if (isIntrTrg_2) {
    //   noInterrupts();
    //   forv = true;
    //   interrupts();
    // }
    bool tempIsIntrTrg_2 = isIntrTrg_2;
    forv = tempIsIntrTrg_2;
    isIntrTrg_1 = true;
  }
}

void scdIntr() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lstIntrTim_2 >= debounceDelay) {

    // isIntrTrg_2 = true;
    lstIntrTim_2 = currentMillis;

    // if (isIntrTrg_1) {
    //   noInterrupts();
    //   back = true;
    //   interrupts();
    // }
    bool tempIsIntrTrg_1 = isIntrTrg_1;
    back = tempIsIntrTrg_1;
    isIntrTrg_2 = true;
  }
}

void loop() {
  // Modbus handler.
  slave.poll(au16data, length); // Max read elements.

  if (au16data[0] != currentSlaveAddress || ((uint32_t)au16data[1] << 16 | au16data[2]) != currentBaudrate) {

    currentSlaveAddress = au16data[0];
    currentBaudrate = (uint32_t)au16data[1] << 16 | au16data[2];

    Serial.end();
    delay(100);
    Serial.begin(currentBaudrate, SERIAL_8N1);
    slave = Modbus(currentSlaveAddress, currentBaudrate, TXEN);
    slave.start();

    deviceDataObject.modbusSlaveAddress = currentSlaveAddress;
    deviceDataObject.modbusBaudrate = currentBaudrate;

    IODataSDFileWrite(dataFilename);

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint("                ");

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());
    MCPDisplayCursorSet(11, 0);
    MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());
  }

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

  // deviceDataObject.modbusSlaveAddress = au16data[0];
  // deviceDataObject.modbusBaudrate = au16data[1];
  // deviceDataObject.modbusBaudrate = (deviceDataObject.modbusBaudrate << 16) | au16data[2];

  unsigned long currentMillisSaved = millis();
  if (currentMillisSaved - autSvdTim >= intervalSD) {
    MCPDisplayCursorSet(19, 0);
    MCPDisplayPrint("S");

    IODataSDFileWrite(dataFilename);

    MCPDisplayCursorSet(19, 0);
    MCPDisplayPrint(" ");

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
