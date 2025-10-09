#include "Headers/Headers.h"
#include "Libs/Libs.h"
#include "Sources/Sources.h"

void setup() {
  MCPDisplayInitialize(0x26, 20, 4);

  IODataSDInitialize();

  sensorsInitialize();

  modbusInitialize();

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
  }

  modbusHandlerListener();

  // modbusSettingsUpdater();

  modbusHandlerResponse();

  IODataSDFileWritePeriodically();

  // deviceDataObject.modbusSlaveAddress = au16data[0];
  // deviceDataObject.modbusBaudrate = au16data[1];
  // deviceDataObject.modbusBaudrate = (deviceDataObject.modbusBaudrate << 16) | au16data[2];
}
