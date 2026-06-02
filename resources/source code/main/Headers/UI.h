#ifndef UI_H
#define UI_H

#define PIN_RIGHT 13
#define PIN_UP 12
#define PIN_OK 11
#define PIN_LEFT 10
#define PIN_DOWN 9

enum UIDisplaySectionList {
    sectionDefault,
    sectionMenu,
    sectionMenuConnections,
    sectionMenuTotalizers,
    sectionMenuTotalizersSelectedRequestReset,
    sectionMenuInformations,
    sectionMenuCountMode,
    sectionVoid
};
extern UIDisplaySectionList UIDisplaySectionListObject;

struct menuItems;
extern bool buttonStatus[5];
extern bool UIDisplayNeedClear;
extern bool UIDisplayNeedRefresh;

void UIButtonsInitialize();

void UIDisplayInitialize();

void UIDisplayMenuScrollable(const char** items, int count, const char* title = nullptr);

void UIButtonsHandler();

void UIDisplayDefault();

void UIDisplayMenuMain();

void UIDisplayMenuConnection();

void UIDisplayMenuTotalizers();

void UIDisplayMenuTotalizersSelectedRequestReset();

void UIDisplayMenuTotalizersSelectedReset();

void UIDisplayMenuInformation();

void UIDisplayMenuCountMode();

void UIDisplayMenuIndex(int menuIndexLimit, bool action = false);

void UIDisplayHandler();

void UIDisplayClear();

void UIDisplayDelay();

void UIDisplayToVoidAndBack();

#endif