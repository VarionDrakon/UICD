#include "../Headers/Headers.h"

volatile bool back = false;             // Triggered sensor 1
volatile bool forv = false;             // Triggered sensor 2
volatile bool isIntrTrg_1 = false;      // Triggered sensor 1
volatile bool isIntrTrg_2 = false;      // Triggered sensor 2
volatile bool isIntrTrg_1Prev = false;
volatile bool isIntrTrg_2Prev = false;
const uint32_t debounceDelay = 1;       // Debounce delay. OLD varian - if (millis() - secDbn >= 100 && digitalRead(3)) 

void sensorsInitialize() {
  attachInterrupt(0, frtIntr, FALLING);
  attachInterrupt(1, scdIntr, FALLING);
}

void frtIntr() {
  isIntrTrg_1 = true;

  if (isIntrTrg_1Prev == false) {
    isIntrTrg_1Prev = true;
    if (isIntrTrg_2 == true && isIntrTrg_1 == true) {
      forv = true;
      isIntrTrg_1 = false;
      isIntrTrg_2 = false;
      sensorHandler();
    }
  }
}

void scdIntr() {
  isIntrTrg_2 = true;
  if (isIntrTrg_2Prev == false) {
    isIntrTrg_2Prev = true;
    if (isIntrTrg_2 == true && isIntrTrg_1 == true) {
      back = true;
      isIntrTrg_1 = false;
      isIntrTrg_2 = false;
      sensorHandler();
    }
  }
}

void sensorHandler() {
    if (forv) {
      forv = false;
      isIntrTrg_1Prev = false;
      isIntrTrg_2Prev = false;
      UIDisplayNeedRefresh = true;
      totalizerDirectValueAdd();
      totalizerCommonValueAdd();
    }
    if (back) {
      back = false;
      isIntrTrg_1Prev = false;
      isIntrTrg_2Prev = false;
      UIDisplayNeedRefresh = true;
      totalizerReverseValueAdd();
      totalizerCommonValueAdd();
    }
}