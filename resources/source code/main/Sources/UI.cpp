#include "Arduino.h"
#include "../Headers/Headers.h"

#define PIN_RIGHT 9 // [PB1] | Physical pinout of the microcircuit: 13
#define PIN_UP    6 // [PB0] | Physical pinout of the microcircuit: 12
#define PIN_OK    7 // [PD7] | Physical pinout of the microcircuit: 11
#define PIN_LEFT  5 // [PD6] | Physical pinout of the microcircuit: 10
#define PIN_DOWN  8 // [PD5] | Physical pinout of the microcircuit: 9

bool buttonStatus[5] = { 
    false, // [0] PIN_RIGHT
    false, // [1] PIN_UP
    false, // [2] PIN_OK
    false, // [3] PIN_LEFT
    false  // [4] PIN_DOWN
};



struct menuItems {
    int settingsSelectionIndex = 0;
    char* dictionaryMenuItem[5] = {"> ", "ModBus", "Totalizer`s", "Information",  "Exit"};
};
menuItems menuItemsObject;

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
    UIDisplayMain();
    
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

void UIDisplayMenuMain() {
    MCPDisplayCommandSend(0x01);
    delay(10);

    switch (menuItemsObject.settingsSelectionIndex)
    {
    case 0:
        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[0]);

        MCPDisplayCursorSet(3, 0);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[1]);

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[2]);

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[3]);

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[4]);
        break;
    case 1:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[0]);

        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[1]);

        MCPDisplayCursorSet(3, 1);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[2]);

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[3]);

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[4]);
        break;
    case 2:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[0]);
        
        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[1]);

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[2]);

        MCPDisplayCursorSet(3, 2);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[3]);

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[4]);
        break;
    case 3:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[0]);

        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[1]);
        
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[2]);

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[3]);

        MCPDisplayCursorSet(3, 3);
        MCPDisplayPrint(menuItemsObject.dictionaryMenuItem[4]);
        break;
    }
};

void UIDisplayMenuIndex(bool action = false) {
    if (action == false) {
        if (menuItemsObject.settingsSelectionIndex <= 0) {
            menuItemsObject.settingsSelectionIndex = 0;
        }
        else {
            menuItemsObject.settingsSelectionIndex--;
        }
    }
    else {
        if (menuItemsObject.settingsSelectionIndex >= 3) {
            menuItemsObject.settingsSelectionIndex = 3;
        }
        else {
            menuItemsObject.settingsSelectionIndex++;
        }
    }
}

void UIDisplayMenuHandler() {
    if (menuItemsObject.settingsSelectionIndex == 0) {
        UIDisplayMenuMain();
    }
    if (menuItemsObject.settingsSelectionIndex == 3) {
        menuItemsObject.settingsSelectionIndex = 0;
        UIDisplayMain();
    }
}

void buttonHandle() {
    buttonStatus[0] = digitalRead(PIN_RIGHT);
    buttonStatus[1] = digitalRead(PIN_UP);
    buttonStatus[2] = digitalRead(PIN_OK);
    buttonStatus[3] = digitalRead(PIN_LEFT);
    buttonStatus[4] = digitalRead(PIN_DOWN);


    if (buttonStatus[0] == LOW) {
        delay(50);
        MCPDisplayCursorSet(17, 0);
        MCPDisplayPrint("R");
    }
    if (buttonStatus[1] == LOW) {
        delay(50);
        UIDisplayMenuIndex();
        UIDisplayMenuMain();
    }
    if (buttonStatus[2] == LOW) {
        delay(50);
        UIDisplayMenuHandler();
    }
    if (buttonStatus[3] == LOW) {
        delay(50);
        MCPDisplayCursorSet(17, 0);
        MCPDisplayPrint("L");
    }
    if (buttonStatus[4] == LOW) {
        delay(50);
        UIDisplayMenuIndex(true);
        UIDisplayMenuMain();
    }
};