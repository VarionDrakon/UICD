//Entire text and serial print - is temporary

#include "Display.h"
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

    // Инициализация MCP23008
  if (!mcp.begin(0x26)) {
    Serial.println("MCP23008 not found!");
    while(1);
  }
  
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);
    mcp.digitalWrite(i, LOW);
  }
  
  delay(1000);
  mcp.digitalWrite(MCP_RS, LOW);
  
  write4bits(0x03); delay(15);
  write4bits(0x03); delay(15);
  write4bits(0x03); delay(15);
  write4bits(0x02); delay(15);
  
  sendCommand(0x28); delay(5);
  sendCommand(0x0C); delay(5);
  sendCommand(0x01); delay(10);
  sendCommand(0x06); delay(5);
  
  // Тест для определения реального размера
  Serial.println("Testing display size...");
  
  // Заполняем весь дисплей символами
  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 20; col++) {
      setCursor(col, row);
      sendData('0' + col % 10); // Цифры 0-9
    }
  }
  
  delay(3000);
  
  // Очищаем и выводим адаптивный текст
  sendCommand(0x01); delay(10);
  
  setCursor(0, 0);
  print("Size: 20x4 OK!");
  setCursor(0, 1);
  print("Rows: 4 Cols: 20");
  setCursor(0, 2);
  print("MCP23008 I2C");
  setCursor(0, 3);
  print("Address: 0x26");
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

    static int counter = 0;
  setCursor(15, 3);
  print(String(counter % 1000).c_str());
  counter++;
  delay(100);

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
