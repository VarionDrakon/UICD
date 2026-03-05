#include <Arduino.h>
#include "Headers/Headers.h"

Adafruit_MCP23008 mcp;

void setup() {

  MCPDisplayInitialize(0x26, 20, 4);

  IODataSDInitialize();

  sensorsInitialize();

  modbusInitialize();

  UIDisplayInitialize();

}

void loop() {
  // if (deviceDataObjectIsBusy) {
  //   MCPDisplayCursorSet(18, 0);
  //   MCPDisplayPrint("B");
  // }
  // else {
  //   MCPDisplayCursorSet(18, 0);
  //   MCPDisplayPrint("F");
  // }
  
  IODataWatchdogHandler();

  modbusUpdater();

  modbusHandlerListener();
  
  IODataSDFileWritePeriodically();

  UIButtonsHandler();

  UIDisplayHandler();
}