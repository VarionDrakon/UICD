#include <Arduino.h>
#include "Headers/Headers.h"

Adafruit_MCP23008 mcp;

void setup() {

  MCPDisplayInitialize(0x26, 20, 4);

  IODataSDInitialize();

  sensorsInitialize();

  deviceConfigurationModbusBaudrateSet(9600);
  deviceConfigurationModbusSlaveAddressSet(10);

  modbusInitialize();

  UIInitialize();
  
}

void loop() {
  //realize struct and write data on SD-card IT`S SLOW DOWN WORK MICROCONTROLLER!
  if (forv) {
    totalizerDirectValueAdd();
    totalizerCommonValueAdd();
    forv = false;
    isIntrTrg_1 = false;
    isIntrTrg_2 = false;

    MCPDisplayCursorSet(4, 1);
    MCPDisplayPrint(totalizerCommonReturn());
    MCPDisplayCursorSet(4, 2);
    MCPDisplayPrint(totalizerDirectReturn());
    IODataSDFileWrite(deviceDataObject.totalizerCommon);
  }
  if (back) {
    totalizerReverseValueAdd();
    totalizerCommonValueAdd();
    back = false;
    isIntrTrg_1 = false;
    isIntrTrg_2 = false;

    MCPDisplayCursorSet(4, 1);
    MCPDisplayPrint(totalizerCommonReturn());
    MCPDisplayCursorSet(4, 3);
    MCPDisplayPrint(totalizerReverseReturn());
    IODataSDFileWrite(deviceDataObject.totalizerCommon);
  }

  modbusHandlerListener();

  modbusHandlerResponse();

  modbusSettingsUpdater();

  IODataSDFileWritePeriodically();

  buttonHandle();

  // deviceDataObject.modbusSlaveAddress = au16data[0];
  // deviceDataObject.modbusBaudrate = au16data[1];
  // deviceDataObject.modbusBaudrate = (deviceDataObject.modbusBaudrate << 16) | au16data[2];
}
