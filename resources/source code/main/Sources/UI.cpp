#include "Arduino.h"
#include "../Headers/Headers.h"

#define PIN_RIGHT 9 // [PB1]
#define PIN_DOWN  8 // [PD5]
#define PIN_OK    7 // [PD7]
#define PIN_UP    6 // [PB0]
#define PIN_LEFT  5 // [PD6]

bool buttonStatus[5] = { 
    false, // [0] PIN_RIGHT
    false, // [1] PIN_UP
    false, // [2] PIN_OK
    false, // [3] PIN_LEFT
    false  // [4] PIN_DOWN
};

enum UIState {
    MAIN,
    MENU,
    INFO
};

void buttonInitialize() {
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_UP, INPUT_PULLUP);
    pinMode(PIN_OK, INPUT_PULLUP);
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
};

void UIInitialize() {

    buttonInitialize();
    
};


void UIDisplayMain() {
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
};

void buttonHandle() {
    delay(50);
    buttonStatus[0] = digitalRead(PIN_RIGHT);
    buttonStatus[1] = digitalRead(PIN_UP);
    buttonStatus[2] = digitalRead(PIN_OK);
    buttonStatus[3] = digitalRead(PIN_LEFT);
    buttonStatus[4] = digitalRead(PIN_DOWN);


    if (buttonStatus[0] == LOW) {
        MCPDisplayCommandSend(0x01);
        delay(10);
        MCPDisplayCursorSet(5, 2);
        MCPDisplayPrint("PIN_RIGHT");
    }
    if (buttonStatus[1] == LOW) {
        MCPDisplayCommandSend(0x01);
        delay(10);
        MCPDisplayCursorSet(5, 2);
        MCPDisplayPrint("PIN_UP");
    }
    if (buttonStatus[2] == LOW) {
        MCPDisplayCommandSend(0x01);
        delay(10);
        MCPDisplayCursorSet(5, 2);
        MCPDisplayPrint("PIN_OK");
    }
    if (buttonStatus[3] == LOW) {
        MCPDisplayCommandSend(0x01);
        delay(10);
        MCPDisplayCursorSet(5, 2);
        MCPDisplayPrint("PIN_LEFT");
    }
    if (buttonStatus[4] == LOW) {
        UIDisplayMain();
        MCPDisplayCursorSet(17, 0);
        MCPDisplayPrint("PIN_DOWN");
    }
};