#include "../Libs/ModbusRtu.h"
#include "../Headers/Headers.h"

/*
Element (9):
[0] - slave addres;
[1], [2] - baudrate;
[3], [4] - totalize 0 - GENERAL;
[5], [6] - totalize 1 - FORWARD;
[7], [8] - totalize 2 - REVERSE;
*/
uint16_t au16data[length] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };  //Element [X] - Reserve for future

int modbusBaudrateListIndex = 3;
const int modbusBaudrateListIndexLimit = 8;
uint32_t modbusBaudrateList[modbusBaudrateListIndexLimit] = { 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200 };
bool modbusBaudrateValueFound = false;

byte modbusSlaveAddressValueCurrent = 0;
unsigned long modbusBaudrateValueCurrent = 0;

// Class instance ModBus.
Modbus slave(modbusSlaveAddressValueCurrent, modbusBaudrateValueCurrent, modbusPin);

void modbusInitialize() {

  for (int i = 0; i < modbusBaudrateListIndexLimit; i++ ) {
    if (modbusBaudrateValueCurrent == modbusBaudrateList[i]) {
      modbusBaudrateValueFound = true;
      break;
    }
  }

  if (modbusBaudrateValueFound == false) {
    deviceDataObject.modbusBaudrate = modbusBaudrateList[modbusBaudrateListIndex];
    deviceDataObject.modbusSlaveAddress = 10;
  }

  modbusHandlerReloader();
}

void modbusHandlerReloader() {
  modbusSlaveAddressValueCurrent = deviceDataObject.modbusSlaveAddress;
  modbusBaudrateValueCurrent = deviceDataObject.modbusBaudrate;
  
  Serial.end();
  delay(100);
  Serial.begin(modbusBaudrateValueCurrent, SERIAL_8N1);
  slave = Modbus(modbusSlaveAddressValueCurrent, modbusBaudrateValueCurrent, modbusPin);
  slave.start();

  au16data[0] = deviceDataObject.modbusSlaveAddress;
  au16data[1] = (deviceDataObject.modbusBaudrate >> 16) & 0xFFFF;
  au16data[2] = deviceDataObject.modbusBaudrate & 0xFFFF;
}

/*
  This method listens to the serial port for incoming Modbus requests from the master device.
  Upon receiving a valid request, it processes it according to the Modbus function (reading/writing registers, coils, etc.).
  Updates the au16data data array according to the received commands.
  Sends a response to the master device.
  Returns the execution status (0 - no request, >4 - successful processing, 1-4 - errors).
*/
void modbusHandlerListener() {
  slave.poll(au16data, length); // Max read elements.
}

/*
  This code splits 32-bit totalizer (counter) values ​​into two 16-bit Modbus registers.
  Modbus uses 16-bit registers, while totalizers are often 32-bit. This code allows reading 32-bit values ​​through two consecutive Modbus registers.
*/
void modbusHandlerResponse() {
  //Splitting into bytes array from long variable
  if (!deviceDataObject.totalizerCommon == 0) {
    au16data[3] = (deviceDataObject.totalizerCommon >> 16) & 0xFFFF;   //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[4] = deviceDataObject.totalizerCommon & 0xFFFF;           //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
    au16data[5] = (deviceDataObject.totalizerDirect >> 16) & 0xFFFF;   //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[6] = deviceDataObject.totalizerDirect & 0xFFFF;           //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
    au16data[7] = (deviceDataObject.totalizerReverse >> 16) & 0xFFFF;  //getting (1 part) most significant and shift right 16 bits (0xFFFF - 4 bytes)
    au16data[8] = deviceDataObject.totalizerReverse & 0xFFFF;          //filtering (2 part) results on 16 bits (0xFFFF - 4 bytes)
  }
}

/*
  This code handles dynamic, on-the-fly changes to the Modbus master (slave) settings.
  Short summary:

    Checks whether the Modbus address or baud rate in au16data has changed.

    If so, reconfigures Modbus:

      Stops Serial

      Restarts at the new baud rate

      Creates a new Modbus slave with a new address

      Saves settings to EEPROM/SD card

      Updates display data
*/
void modbusSettingsUpdater() {
  if (au16data[0] != deviceDataObject.modbusSlaveAddress || ((uint32_t)au16data[1] << 16 | au16data[2]) != deviceDataObject.modbusBaudrate) {

    deviceDataObject.modbusSlaveAddress = au16data[0];
    deviceDataObject.modbusBaudrate = (uint32_t)au16data[1] << 16 | au16data[2];

    modbusHandlerReloader();

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint("                ");

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());
    MCPDisplayCursorSet(11, 0);
    MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());
  }
}