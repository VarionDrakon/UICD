#include <SD.h>

struct data {
  unsigned long savedGnrBott;
  unsigned long savedFrwBott;
  unsigned long savedRvrBott;
  unsigned long savedBaudrate;
  byte savedSlaveAddr;
};
data commonData;

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