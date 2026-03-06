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
  IODataWatchdogHandler();

  modbusUpdater();

  modbusHandlerListener();
  
  IODataSDFileWritePeriodically();

  UIButtonsHandler();

  UIDisplayHandler();
}