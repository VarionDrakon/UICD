#include <Arduino.h>
#include "Headers/Headers.h"

Adafruit_MCP23008 mcp;

void setup() {

  // deviceConfigurationModbusBaudrateSet(9600);
  // deviceConfigurationModbusSlaveAddressSet(10);

  MCPDisplayInitialize(0x26, 20, 4);

  IODataSDInitialize();

  sensorsInitialize();

  modbusInitialize();

  UIDisplayInitialize();

}

void loop() {

  modbusHandlerListener();

  modbusHandlerResponse();

  modbusSettingsUpdater();
  
  IODataSDFileWritePeriodically();

  UIButtonsHandler();

  UIDisplayHandler();

  // deviceDataObject.modbusSlaveAddress = au16data[0];
  // deviceDataObject.modbusBaudrate = au16data[1];
  // deviceDataObject.modbusBaudrate = (deviceDataObject.modbusBaudrate << 16) | au16data[2];
}