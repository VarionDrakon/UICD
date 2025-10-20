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

//Default settings
Modbus slave(10, 9600, modbusPin);
byte currentSlaveAddress = 10;
unsigned long currentBaudrate = 9600;

void modbusInitialize() {
  currentSlaveAddress = deviceDataObject.modbusSlaveAddress;
  currentBaudrate = deviceDataObject.modbusBaudrate;

  slave = Modbus(currentSlaveAddress, currentBaudrate, modbusPin);
  slave.begin();
  Serial.begin(currentBaudrate, SERIAL_8N1);

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
  if (au16data[0] != currentSlaveAddress || ((uint32_t)au16data[1] << 16 | au16data[2]) != currentBaudrate) {

    currentSlaveAddress = au16data[0];
    currentBaudrate = (uint32_t)au16data[1] << 16 | au16data[2];

    Serial.end();
    delay(100);
    Serial.begin(currentBaudrate, SERIAL_8N1);
    slave = Modbus(currentSlaveAddress, currentBaudrate, modbusPin);
    slave.start();

    deviceDataObject.modbusSlaveAddress = currentSlaveAddress;
    deviceDataObject.modbusBaudrate = currentBaudrate;

    // IODataSDFileWrite();

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint("                ");

    MCPDisplayCursorSet(4, 0);
    MCPDisplayPrint(deviceConfigurationModbusBaudrateGet());
    MCPDisplayCursorSet(11, 0);
    MCPDisplayPrint(deviceConfigurationModbusSlaveAddressGet());
  }
}