#ifndef COUNTER_H
#define COUNTER_H

#include <Arduino.h>

extern volatile bool back;             //Triggered sensor 1
extern volatile bool forv;             //Triggered sensor 2
extern volatile bool isIntrTrg_1;      //Triggered sensor 1
extern volatile bool isIntrTrg_2;      //Triggered sensor 2
extern volatile uint32_t lstIntrTim_1;     //Timer for sensor 1
extern volatile uint32_t lstIntrTim_2;     //Timer for sensor 2
extern const uint32_t debounceDelay;  // Debounce delay. OLD varian - if (millis() - secDbn >= 100 && digitalRead(3)) 

void sensorsInitialize();
void frtIntr();
void scdIntr();

#endif