#include "../Headers/Headers.h"

volatile bool back = false;             // Triggered sensor 1
volatile bool forv = false;             // Triggered sensor 2
volatile bool isIntrTrg_1 = false;      // Triggered sensor 1
volatile bool isIntrTrg_2 = false;      // Triggered sensor 2
volatile uint32_t lstIntrTim_1 = 0;     // Timer for sensor 1
volatile uint32_t lstIntrTim_2 = 0;     // Timer for sensor 2
const uint32_t debounceDelay = 1;       // Debounce delay. OLD varian - if (millis() - secDbn >= 100 && digitalRead(3)) 

void sensorsInitialize() {
  attachInterrupt(0, frtIntr, FALLING);
  attachInterrupt(1, scdIntr, FALLING);
}

void frtIntr() {
  // unsigned long currentMillis = millis();
  // if (currentMillis - lstIntrTim_1 >= debounceDelay) {
  //   lstIntrTim_1 = currentMillis;

    bool tempIsIntrTrg_2 = isIntrTrg_2;
    forv = tempIsIntrTrg_2;
    isIntrTrg_1 = true;
  // }
    if (forv) {
      forv = false;
      isIntrTrg_1 = false;
      isIntrTrg_2 = false;
      totalizerDirectValueAdd();
      totalizerCommonValueAdd();
    }
    if (back) {
      back = false;
      isIntrTrg_1 = false;
      isIntrTrg_2 = false;
      totalizerReverseValueAdd();
      totalizerCommonValueAdd();
    }
}

void scdIntr() {
  // unsigned long currentMillis = millis();
  // if (currentMillis - lstIntrTim_2 >= debounceDelay) {
  //   lstIntrTim_2 = currentMillis;

    bool tempIsIntrTrg_1 = isIntrTrg_1;
    back = tempIsIntrTrg_1;
    isIntrTrg_2 = true;
    if (forv) {
      forv = false;
      isIntrTrg_1 = false;
      isIntrTrg_2 = false;
      totalizerDirectValueAdd();
      totalizerCommonValueAdd();
    }
    if (back) {
      back = false;
      isIntrTrg_1 = false;
      isIntrTrg_2 = false;
      totalizerReverseValueAdd();
      totalizerCommonValueAdd();
    }
  // }
}