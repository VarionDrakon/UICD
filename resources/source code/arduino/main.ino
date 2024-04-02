//Entire text and serial print - is temporary

#include "ModbusRtu.h"  //Modbus lib
#include <SD.h>
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
volatile bool isIntrTrg_1 = false;                                   //Triggered sensor 1
volatile bool isIntrTrg_2 = false;                                   //Triggered sensor 2
volatile bool back = false;                                          //Triggered sensor 1
volatile bool forv = false;                                          //Triggered sensor 2
volatile uint32_t lstIntrTim_1 = 0;                                  //Timer for sensor 1
volatile uint32_t lstIntrTim_2 = 0;                                  //Timer for sensor 2
volatile uint32_t autSvdTim = 0;                                     //Timer for autosave settings
unsigned long interval = 5 * 60 * 1000;                              // 5 minutes
unsigned long intervalSD = 10000;                                     // Timer for write data SD
const unsigned long debounceDelay = 30;                              // Debounce delay. OLD varian - if (millis() - secDbn >= 100 && digitalRead(3))
//Default settings
Modbus slave(10, 9600, TXEN);
char dataFilename[] = "data.dat";

struct data {
  unsigned long savedGnrBott;
  unsigned long savedFrwBott;
  unsigned long savedRvrBott;
  unsigned long savedBaudrate;
  byte savedSlaveAddr;
};
data commonData;

void printData() {
  Serial.println("------------------------");
  Serial.print("savedGnrBott - ");
  Serial.println(commonData.savedGnrBott);
  Serial.print("savedFrwBott - ");
  Serial.println(commonData.savedFrwBott);
  Serial.print("savedRvrBott - ");
  Serial.println(commonData.savedRvrBott);
  Serial.print("savedBaudrate - ");
  Serial.println(commonData.savedBaudrate);
  Serial.print("savedSlaveAddr - ");
  Serial.println(commonData.savedSlaveAddr);
  Serial.println("------------------------");
}

bool writeDataFile(const String& fileName) {
  File dataFile = SD.open(fileName, FILE_WRITE | O_TRUNC | O_APPEND);
  if (dataFile) {
    dataFile.write((uint8_t*)&commonData, sizeof(commonData));
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
    dataFile.read((uint8_t*)&commonData, sizeof(commonData));
    dataFile.close();
    Serial.println("Read file - success!");
    return true;
  } else {
    Serial.println("Read file - ERROR!");
    return false;
  }
}

void setup() {
  slave.begin(9600);               //ModBus speed
  Serial.begin(9600, SERIAL_8N1);  //Serial settings

  Serial.println("Arduino started---------");

  if (!SD.begin(10)) {
    Serial.println("SD ERROR!");  //Check SD-card
    return;
  }

  attachInterrupt(0, frtIntr, FALLING);
  attachInterrupt(1, scdIntr, FALLING);

  if (SD.exists(dataFilename)) {
    readDataFile(dataFilename);
    printData();
  }
  else {
    Serial.println("Device reset?-----------");    
    commonData.savedBaudrate = 9600;
    commonData.savedSlaveAddr = 10;
    printData();
  }

  au16data[0] = commonData.savedSlaveAddr;
  au16data[1] = (commonData.savedBaudrate >> 16) & 0xFFFF;
  au16data[2] = commonData.savedBaudrate & 0xFFFF;
}


void frtIntr() {
  Serial.println("!TRIGGER!");
  unsigned long currentMillis = millis();
  if (currentMillis - lstIntrTim_1 >= debounceDelay) {
    isIntrTrg_1 = true;
    lstIntrTim_1 = currentMillis;
    Serial.println("Anti-shattering Read! X1");
    if (isIntrTrg_2) {
      noInterrupts();
      forv = true;
      interrupts();
    }
  }
}

void scdIntr() {
  Serial.println("!TRIGGER!");
  unsigned long currentMillis = millis();
  if (currentMillis - lstIntrTim_2 >= debounceDelay) {
    isIntrTrg_2 = true;
    lstIntrTim_2 = currentMillis;
    Serial.println("Anti-shattering Read! X2");
    if (isIntrTrg_1) {
      noInterrupts();
      back = true;
      interrupts();
    }
  }
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
    printData();
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
