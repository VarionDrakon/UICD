#include "Arduino.h"
#include "../Headers/Headers.h"

#define PIN_RIGHT 9 // [PB1] | Physical pinout of the microcircuit: 13
#define PIN_UP    6 // [PB0] | Physical pinout of the microcircuit: 12
#define PIN_OK    7 // [PD7] | Physical pinout of the microcircuit: 11
#define PIN_LEFT  5 // [PD6] | Physical pinout of the microcircuit: 10
#define PIN_DOWN  8 // [PD5] | Physical pinout of the microcircuit: 9

#define BUTTON_RIGHT 0
#define BUTTON_UP    1
#define BUTTON_OK    2
#define BUTTON_LEFT  3
#define BUTTON_DOWN  4

bool buttonStatus[5] = { 
    false, // [0] PIN_RIGHT
    false, // [1] PIN_UP
    false, // [2] PIN_OK
    false, // [3] PIN_LEFT
    false  // [4] PIN_DOWN
};

struct UIDisplayMenuItems {
    int settingsIndexSelection = 0;
    int settingsIndexLimit = 0;
};
UIDisplayMenuItems UIDisplayMenuItemsObject;

enum UIDisplaySectionList {
    sectionDefault,
    sectionMenu,
    sectionMenuConnections,
    sectionMenuTotalizers,
    sectionMenuInformations
};
UIDisplaySectionList UIDisplaySectionListObject = sectionDefault;

void buttonInitialize() {
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_UP, INPUT_PULLUP);
    pinMode(PIN_OK, INPUT_PULLUP);
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
};

void UIDisplayInitialize() {
    UIDisplayHandler();
};


void UIDisplayDefault() {
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
    UIDisplayMenuItemsObject.settingsIndexLimit = 3;
    MCPDisplayCommandSend(0x01);
    delay(10);

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(2, 0);
        MCPDisplayPrint("Connections");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("Totalizers");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("Informations");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 1:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint("Connections");

        MCPDisplayCursorSet(2, 1);
        MCPDisplayPrint("Totalizers");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("Informations");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 2:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(">");
        
        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint("Connections");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("Totalizers");

        MCPDisplayCursorSet(2, 2);
        MCPDisplayPrint("Informations");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 3:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint("Connections");
        
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("Totalizers");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("Informations");

        MCPDisplayCursorSet(2, 3);
        MCPDisplayPrint("Exit");
        break;
    }
};

void UIDisplayMenuConnection() {
    MCPDisplayCommandSend(0x01);
    delay(10);

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(11, 0);
    MCPDisplayPrint("Modbus");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(2, 1);
        MCPDisplayPrint("Baudrate");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("Address");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 1:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("Baudrate");

        MCPDisplayCursorSet(2, 2);
        MCPDisplayPrint("Address");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 2:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("Baudrate");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("Address");

        MCPDisplayCursorSet(2, 3);
        MCPDisplayPrint("Exit");
        break;
    }
}

void UIDisplayMenuTotalizers() {
    MCPDisplayCommandSend(0x01);
    delay(10);

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(11, 0);
    MCPDisplayPrint("TTLZRS");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(2, 1);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 1:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(2, 2);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 2:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(2, 3);
        MCPDisplayPrint("Exit");
        break;
    }
}

void UIDisplayMenuInformation() {
    MCPDisplayCommandSend(0x01);
    delay(10);

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(11, 0);
    MCPDisplayPrint("Info");

    MCPDisplayCursorSet(11, 1);
    MCPDisplayPrint("19200");
    MCPDisplayCursorSet(11, 2);
    MCPDisplayPrint("256");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(2, 1);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 1:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(2, 2);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("Exit");
        break;
    case 2:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint(">");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("None");

        MCPDisplayCursorSet(2, 3);
        MCPDisplayPrint("Exit");
        break;
    }
}

void UIDisplayHandler() {
    switch (UIDisplaySectionListObject)
    {
    case sectionDefault:
        UIDisplaySectionListObject = sectionDefault;
        UIDisplayMenuItemsObject.settingsIndexLimit = 0;
        UIDisplayDefault();
        break;
    case sectionMenu:
        UIDisplayMenuMain();
        UIDisplayMenuItemsObject.settingsIndexLimit = 3;
        break;
    case sectionMenuConnections:
        UIDisplayMenuConnection();
        UIDisplayMenuItemsObject.settingsIndexLimit = 2;
        break;
    case sectionMenuTotalizers:
        UIDisplayMenuTotalizers();
        UIDisplayMenuItemsObject.settingsIndexLimit = 2;
        break;
    case sectionMenuInformations:
        UIDisplayMenuInformation();
        UIDisplayMenuItemsObject.settingsIndexLimit = 2;
        break;
    }
}

void UIButtonsHandler() {
    buttonStatus[BUTTON_RIGHT] = digitalRead(PIN_RIGHT);
    buttonStatus[BUTTON_UP] = digitalRead(PIN_UP);
    buttonStatus[BUTTON_OK] = digitalRead(PIN_OK);
    buttonStatus[BUTTON_LEFT] = digitalRead(PIN_LEFT);
    buttonStatus[BUTTON_DOWN] = digitalRead(PIN_DOWN);

    switch (UIDisplaySectionListObject)
    {
    case sectionDefault:
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("O");
            UIDisplaySectionListObject = sectionMenu;
            UIDisplayHandler();
        }
        break;
    case sectionMenu:
        if (buttonStatus[BUTTON_UP] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection > 0) {
                UIDisplayMenuItemsObject.settingsIndexSelection--;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = UIDisplayMenuItemsObject.settingsIndexLimit;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("U");
        }

        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("D");
        }
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);

            switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
            case 0:
                UIDisplaySectionListObject = sectionMenuConnections;
                break;
            case 1:
                UIDisplaySectionListObject = sectionMenuTotalizers;
                break;
            case 2:
                UIDisplaySectionListObject = sectionMenuInformations;
                break;
            case 3:
                UIDisplaySectionListObject = sectionDefault;
                break;
            }
            UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("O");
            UIDisplayHandler();
        }
        break;
    case sectionMenuConnections:
        if (buttonStatus[BUTTON_UP] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection > 0) {
                UIDisplayMenuItemsObject.settingsIndexSelection--;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = UIDisplayMenuItemsObject.settingsIndexLimit;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("U");
        }

        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("D");
        }
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);

            switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                UIDisplaySectionListObject = sectionMenu;
                break;
            }
            UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("O");
            UIDisplayHandler();
        }
        break;
    case sectionMenuTotalizers:
        if (buttonStatus[BUTTON_UP] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection > 0) {
                UIDisplayMenuItemsObject.settingsIndexSelection--;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = UIDisplayMenuItemsObject.settingsIndexLimit;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("U");
        }

        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("D");
        }
        if (buttonStatus[BUTTON_RIGHT] == LOW) {
            delay(50);

            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("R");
        }
        if (buttonStatus[BUTTON_LEFT] == LOW) {
            delay(50);

            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("L");
        }
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);

            switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                UIDisplaySectionListObject = sectionMenu;
                break;
            }
            UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("O");
            UIDisplayHandler();
        }
        break;
    case sectionMenuInformations:
        if (buttonStatus[BUTTON_UP] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection > 0) {
                UIDisplayMenuItemsObject.settingsIndexSelection--;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = UIDisplayMenuItemsObject.settingsIndexLimit;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("U");
        }

        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayHandler();
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("D");
        }
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);

            switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
            case 0:
                break;
            case 1:
                break;
            case 2:
                UIDisplaySectionListObject = sectionMenu;
                break;
            }
            UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            MCPDisplayCursorSet(17, 0);
            MCPDisplayPrint("O");
            UIDisplayHandler();
        }
        break;
    }
};