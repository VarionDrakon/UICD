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

  UIDisplayInitialize();

}

void loop() {

  modbusHandlerListener();

  modbusHandlerResponse();

  modbusSettingsUpdater();
  
  IODataSDFileWritePeriodically();

  UIButtonsHandler();

  UIDisplayHandler();

  // counterDirectionHandler();

  // deviceDataObject.modbusSlaveAddress = au16data[0];
  // deviceDataObject.modbusBaudrate = au16data[1];
  // deviceDataObject.modbusBaudrate = (deviceDataObject.modbusBaudrate << 16) | au16data[2];
}