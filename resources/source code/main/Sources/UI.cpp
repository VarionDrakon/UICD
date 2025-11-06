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
    int settingsIndexTotalizersSelected = 0;
};
UIDisplayMenuItems UIDisplayMenuItemsObject;

enum UIDisplaySectionList {
    sectionDefault,
    sectionMenu,
    sectionMenuConnections,
    sectionMenuTotalizers,
    sectionMenuTotalizersSelectedRequestReset,
    sectionMenuInformations
};
UIDisplaySectionList UIDisplaySectionListObject = sectionDefault;
bool UIDisplayNeedClear = true;
bool UIDisplayNeedRefresh = true;
static int UIDisplayTimerUpdate = 600;

void UIButtonsInitialize() {
    pinMode(PIN_RIGHT, INPUT_PULLUP);
    pinMode(PIN_UP, INPUT_PULLUP);
    pinMode(PIN_OK, INPUT_PULLUP);
    pinMode(PIN_LEFT, INPUT_PULLUP);
    pinMode(PIN_DOWN, INPUT_PULLUP);
};

void UIDisplayInitialize() {
    UIDisplayHandler();
    UIButtonsInitialize();
};

void UIDisplayClear() {
    MCPDisplayCommandSend(0x01);
    // delay(10);
    // MCPDisplayCursorSet(0, 0);
    // MCPDisplayPrint("                    ");
    // MCPDisplayCursorSet(0, 1);
    // MCPDisplayPrint("                    ");
    // MCPDisplayCursorSet(0, 2);
    // MCPDisplayPrint("                    ");
    // MCPDisplayCursorSet(0, 3);
    // MCPDisplayPrint("                    ");
}

void UIDisplayDefault() {
    if (UIDisplayNeedClear) {
        UIDisplayClear();

        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint("B: ");

        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("C:");

        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("D:");

        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("R:");

        UIDisplayNeedClear = false;
    }
    
    UIDisplayMenuItemsObject.settingsIndexLimit = 0;
    static unsigned int lastUpdateTime = 0;
    unsigned long currentTime = millis();

    if (currentTime - lastUpdateTime >= UIDisplayTimerUpdate) {
        lastUpdateTime = currentTime;
        if (UIDisplayNeedRefresh) {
            MCPDisplayCursorSet(4, 0);
            MCPDisplayPrint("       ");
            MCPDisplayCursorSet(4, 0);
            MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());

            MCPDisplayCursorSet(11, 0);
            MCPDisplayPrint("         ");
            MCPDisplayCursorSet(11, 0);
            MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());

            MCPDisplayCursorSet(4, 1);
            MCPDisplayPrint("                ");
            MCPDisplayCursorSet(4, 1);
            MCPDisplayPrint(totalizerCommonReturn());

            MCPDisplayCursorSet(4, 2);
            MCPDisplayPrint("                ");
            MCPDisplayCursorSet(4, 2);
            MCPDisplayPrint(totalizerDirectReturn());

            MCPDisplayCursorSet(4, 3);
            MCPDisplayPrint("                ");
            MCPDisplayCursorSet(4, 3);
            MCPDisplayPrint(totalizerReverseReturn());

            UIDisplayNeedRefresh = false;
        }
    }
};

void UIDisplayMenuMain() {
    if (UIDisplayNeedClear == true) UIDisplayClear();
    UIDisplayNeedClear = false;

    if (UIDisplayNeedRefresh == false) return;
    UIDisplayNeedRefresh = false;

    UIDisplayMenuItemsObject.settingsIndexLimit = 3;

    MCPDisplayCursorSet(0, 0);
    MCPDisplayPrint("Connections");

    MCPDisplayCursorSet(0, 1);
    MCPDisplayPrint("Totalizers");

    MCPDisplayCursorSet(0, 2);
    MCPDisplayPrint("Informations");

    MCPDisplayCursorSet(0, 3);
    MCPDisplayPrint("Exit");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 0);
        MCPDisplayPrint("> Connections");
        break;
    case 1:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint(">  Totalizers");
        break;
    case 2:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("> Informations");
        break;
    case 3:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("> Exit");
        break;
    }
};

void UIDisplayMenuConnection() {
    if (UIDisplayNeedClear == true) UIDisplayClear();
    UIDisplayNeedClear = false;

    if (UIDisplayNeedRefresh == false) return;
    UIDisplayNeedRefresh = false;

    UIDisplayMenuItemsObject.settingsIndexLimit = 2;

    MCPDisplayCursorSet(1, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(7, 0);
    MCPDisplayPrint("Modbus");

    MCPDisplayCursorSet(0, 1);
    MCPDisplayPrint("Baudrate");

    MCPDisplayCursorSet(0, 2);
    MCPDisplayPrint("Address");

    MCPDisplayCursorSet(0, 3);
    MCPDisplayPrint("Exit");

    MCPDisplayCursorSet(11, 1);
    MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());
    MCPDisplayCursorSet(11, 2);
    MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("> Baudrate");
        break;
    case 1:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("> Address");
        break;
    case 2:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("> Exit");
        break;
    }
}

void UIDisplayMenuTotalizers() {
    if (UIDisplayNeedClear == true) UIDisplayClear();
    UIDisplayNeedClear = false;

    if (UIDisplayNeedRefresh == false) return;
    UIDisplayNeedRefresh = false;
    
    UIDisplayMenuItemsObject.settingsIndexLimit = 2;

    MCPDisplayCursorSet(1, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(7, 0);
    MCPDisplayPrint("Totalizers");

    MCPDisplayCursorSet(0, 1);
    MCPDisplayPrint("Reset totalizer: F");

    MCPDisplayCursorSet(0, 2);
    MCPDisplayPrint("Reset totalizer: R");

    MCPDisplayCursorSet(0, 3);
    MCPDisplayPrint("Exit");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("> Reset totalizer: F");
        break;
    case 1:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("> Reset totalizer: R");
        break;
    case 2:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("> Exit");
        break;
    }
}

void UIDisplayMenuInformation() {
    if (UIDisplayNeedClear == true) UIDisplayClear();
    UIDisplayNeedClear = false;

    if (UIDisplayNeedRefresh == false) return;
    UIDisplayNeedRefresh = false;
    
    UIDisplayMenuItemsObject.settingsIndexSelection = 3;

    MCPDisplayCursorSet(1, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(7, 0);
    MCPDisplayPrint("Info");

    MCPDisplayCursorSet(0, 1);
    MCPDisplayPrint("None");

    MCPDisplayCursorSet(0, 2);
    MCPDisplayPrint("None");

    MCPDisplayCursorSet(0, 3);
    MCPDisplayPrint("Exit");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
    case 0:
        MCPDisplayCursorSet(0, 1);
        MCPDisplayPrint("> None");
        break;
    case 1:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("> None");
        break;
    case 2:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("> Exit");
        break;
    }
}

void UIDisplayMenuTotalizersSelectedRequestReset() {
    if (UIDisplayNeedClear == true) UIDisplayClear();
    UIDisplayNeedClear = false;

    if (UIDisplayNeedRefresh == false) return;
    UIDisplayNeedRefresh = false;
    
    char buffer[4];
    UIDisplayMenuItemsObject.settingsIndexLimit = 1;

    MCPDisplayCursorSet(1, 0);
    MCPDisplayPrint("Menu:");
    MCPDisplayCursorSet(7, 0);
    MCPDisplayPrint("Totalizers");

    MCPDisplayCursorSet(0, 1);
    MCPDisplayPrint("Reset totalizer ");
    MCPDisplayCursorSet(17, 1);
    MCPDisplayPrint(itoa(UIDisplayMenuItemsObject.settingsIndexTotalizersSelected, buffer, 10));
    MCPDisplayCursorSet(18, 1);
    MCPDisplayPrint("?");

    MCPDisplayCursorSet(0, 2);
    MCPDisplayPrint("Yes");
    MCPDisplayCursorSet(0, 3);
    MCPDisplayPrint("No");

    switch (UIDisplayMenuItemsObject.settingsIndexSelection)
    {
    case 0:
        MCPDisplayCursorSet(0, 2);
        MCPDisplayPrint("> Yes");
        break;
    case 1:
        MCPDisplayCursorSet(0, 3);
        MCPDisplayPrint("> No");
        break;
    }
}

void UIDisplayMenuTotalizersSelectedReset() {
    switch (UIDisplayMenuItemsObject.settingsIndexTotalizersSelected)
    {
    case 0:
        totalizerCommonValueReset();
        UIDisplaySectionListObject = sectionMenuTotalizers;
        break;
    case 1:
        totalizerDirectValueReset();
        UIDisplaySectionListObject = sectionMenuTotalizers;
        break;
    case 2:
        totalizerReverseValueReset();
        UIDisplaySectionListObject = sectionMenuTotalizers;
        break;
    }
}

void UIDisplayHandler() {
    switch (UIDisplaySectionListObject)
    {
    case sectionDefault:
        UIDisplayDefault();
        break;
    case sectionMenu:
        UIDisplayMenuMain();
        break;
    case sectionMenuConnections:
        UIDisplayMenuConnection();
        break;
    case sectionMenuTotalizers:
        UIDisplayMenuTotalizers();
        break;
    case sectionMenuTotalizersSelectedRequestReset:
        UIDisplayMenuTotalizersSelectedRequestReset();
        break;
    case sectionMenuInformations:
        UIDisplayMenuInformation();
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
            UIDisplaySectionListObject = sectionMenu;
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_RIGHT] == LOW) {
            delay(50);
            
            if (UIDisplayMenuItemsObject.settingsIndexSelection == 0) {
                modbusBaudrateListIndex++;

                if (modbusBaudrateListIndex > modbusBaudrateListIndexLimit) modbusBaudrateListIndex = 0;
                deviceDataObject.modbusBaudrate = modbusBaudrateList[modbusBaudrateListIndex];

                modbusHandlerReloader();
            }
            else if (UIDisplayMenuItemsObject.settingsIndexSelection == 1) {
                deviceDataObject.modbusSlaveAddress++;
                modbusHandlerReloader();
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_LEFT] == LOW) {
            delay(50);
            if (UIDisplayMenuItemsObject.settingsIndexSelection == 0) {
                modbusBaudrateListIndex--;

                if (modbusBaudrateListIndex < 0) modbusBaudrateListIndex = modbusBaudrateListIndexLimit;
                deviceDataObject.modbusBaudrate = modbusBaudrateList[modbusBaudrateListIndex];
                modbusHandlerReloader();
            }
            else if (UIDisplayMenuItemsObject.settingsIndexSelection == 1) {
                deviceDataObject.modbusSlaveAddress--;
                modbusHandlerReloader();
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);

            switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
            case 0:
                UIDisplayMenuItemsObject.settingsIndexTotalizersSelected = 1;
                UIDisplaySectionListObject = sectionMenuTotalizersSelectedRequestReset;
                break;
            case 1:
                UIDisplayMenuItemsObject.settingsIndexTotalizersSelected = 2;
                UIDisplaySectionListObject = sectionMenuTotalizersSelectedRequestReset;
                break;
            case 2:
                UIDisplaySectionListObject = sectionMenu;
                break;
            }
            UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        break;
    case sectionMenuTotalizersSelectedRequestReset:
        if (buttonStatus[BUTTON_UP] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection > 0) {
                UIDisplayMenuItemsObject.settingsIndexSelection--;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = UIDisplayMenuItemsObject.settingsIndexLimit;
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_OK] == LOW) {
            delay(50);

            switch (UIDisplayMenuItemsObject.settingsIndexSelection) {
            case 0:
                UIDisplayMenuTotalizersSelectedReset();
                break;
            case 1:
                UIDisplaySectionListObject = sectionMenuTotalizers;
                break;
            }
            UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            UIDisplayMenuItemsObject.settingsIndexTotalizersSelected = 0; // Reset selected totalizer!
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        if (buttonStatus[BUTTON_DOWN] == LOW) {
            delay(50);

            if (UIDisplayMenuItemsObject.settingsIndexSelection < UIDisplayMenuItemsObject.settingsIndexLimit) {
                UIDisplayMenuItemsObject.settingsIndexSelection++;
            }
            else {
                UIDisplayMenuItemsObject.settingsIndexSelection = 0;
            }
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
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
            UIDisplayNeedClear = true;
            UIDisplayNeedRefresh = true;
        }
        break;
    }
}