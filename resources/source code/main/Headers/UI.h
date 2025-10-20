#ifndef UI_H
#define UI_H

#define PIN_RIGHT 13
#define PIN_UP 12
#define PIN_OK 11
#define PIN_LEFT 10
#define PIN_DOWN 9

extern bool buttonStatus[5];

void buttonInitialize();

void UIInitialize();

void buttonHandle();

void UIMainDisplay();

#endif