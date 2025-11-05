#ifndef UI_H
#define UI_H

#define PIN_RIGHT 13
#define PIN_UP 12
#define PIN_OK 11
#define PIN_LEFT 10
#define PIN_DOWN 9

struct menuItems;

extern bool buttonStatus[5];
extern bool UIDisplayNeedRefresh;
extern bool UIDisplayNeedClear;
void UIButtonsInitialize();

void UIDisplayInitialize();

void UIButtonsHandler();

void UIDisplayDefault();

void UIDisplayMenuMain();

void UIDisplayMenuConnection();

void UIDisplayMenuTotalizers();

void UIDisplayMenuTotalizersSelectedRequestReset();

void UIDisplayMenuTotalizersSelectedReset();

void UIDisplayMenuInformation();

void UIDisplayMenuIndex(int menuIndexLimit, bool action = false);

void UIDisplayHandler();

void UIDisplayClear();

#endif