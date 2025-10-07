#include "Headers/MCPDisplay.h"
#include <Wire.h>
#include <Adafruit_MCP23008.h>

Adafruit_MCP23008 mcp;

/*
Loop through all MCP23008 pins from 0 to 7.

 - mcp.pinMode(i, OUTPUT); - Configures each MCP23008 pin as an output (to drive a signal rather than read it).

 - mcp.digitalWrite(i, LOW); - Sets all pins to a low level (0 volts) at startup.

 - delay(1000); - Waits 1000 milliseconds (1 second). This is important to allow the display to warm up its power supply and be ready for operation.

 - mcp.digitalWrite(MCP_RS, LOW); - Sets the RS pin to LOW, indicating that the next command will be a command, not data.
*/
void MCPDisplayInitialize(byte address, byte column, byte row) {

  mcp.begin(address);

  MCPDisplayCommandSend(0x01);  // Clear the display (the longest command).

  delay(10);  // Strict adherence to timing diagrams from the datasheet.

  // Initialize every pins:
  for (int i = 0; i < 8; i++) {
    mcp.pinMode(i, OUTPUT);    // Setting up a pin as an output (for display control).
    mcp.digitalWrite(i, LOW);  // Set low (safe initial state).
  }

  delay(1000);
  mcp.digitalWrite(MCP_RS, LOW);

  MCPDisplayWriteBits(0x03);  // Send the most significant 4 bits of the command 0x30. This is a blinking display to wake it up and understand that we want to work in 8-bit mode.
  delay(15);                  // Strict adherence to timing diagrams from the datasheet.
  MCPDisplayWriteBits(0x03);
  delay(15);
  MCPDisplayWriteBits(0x03);
  delay(15);                  // Strict adherence to timing diagrams from the datasheet.
  MCPDisplayWriteBits(0x02);  // Here switch the screen to 4-bit mode and send the data in two portions of 4 bits.
  delay(15);                  // Strict adherence to timing diagrams from the datasheet.

  MCPDisplayCommandSend(0x28);  // 4-bit mode, 2 lines, 5x8 font.
  delay(5);                     // Strict adherence to timing diagrams from the datasheet.
  MCPDisplayCommandSend(0x0C);  // Turn on the display, turn off the cursor.
  delay(5);
  MCPDisplayCommandSend(0x01);
  delay(10);
  MCPDisplayCommandSend(0x06);  // Input mode: cursor moves to the right.
  delay(5);

  // Filling the display by rows and columns to initialize the display.
  // for (int r = 0; r < row; r++) {
  //   for (int c = 0; c < column; c++) {
  //     MCPDisplayCursorSet(c, r);
  //     MCPDisplayDataSend('8');
  //   }
  // }

  delay(1000);  // Waiting for the screen LEDs to warm up.

  MCPDisplayCommandSend(0x01);
  delay(10);
}

/*
The function takes 4 bits (value) and assigns them to the physical data pins (D4-D7).

  (value >> 0) & 0x01: value >> 0 - Shifts the bits to the right by 0 positions (does nothing).

  & 0x01 - Applies a bit mask. Keeps only the least significant (rightmost) bit.

  (value >> 1) & 0x01 takes the second bit, and so on.

Next comes a strobe pulse (Synchronization signal) on the EN pin (HIGH -> LOW), which causes the display to flash and read the data set on D4-D7.
*/
void MCPDisplayWriteBits(byte value) {
  mcp.digitalWrite(MCP_D4, (value >> 0) & 0x01);
  mcp.digitalWrite(MCP_D5, (value >> 1) & 0x01);
  mcp.digitalWrite(MCP_D6, (value >> 2) & 0x01);
  mcp.digitalWrite(MCP_D7, (value >> 3) & 0x01);

  mcp.digitalWrite(MCP_EN, LOW);
  delayMicroseconds(10);
  mcp.digitalWrite(MCP_EN, HIGH);
  delayMicroseconds(10);
  mcp.digitalWrite(MCP_EN, LOW);
  delayMicroseconds(100);
}

void MCPDisplayCommandSend(byte cmd) {
  mcp.digitalWrite(MCP_RS, LOW);    // We tell the display that we are sending a command.
  MCPDisplayWriteBits(cmd >> 4);    // Send the high-order 4 bits of the command.
  MCPDisplayWriteBits(cmd & 0x0F);  // Send the low-order 4 bits of the command.
  delay(2);                         // Wait until the display executes the command.
}

void MCPDisplayDataSend(byte data) {
  mcp.digitalWrite(MCP_RS, HIGH);    // We tell the display that we are sending a data.
  MCPDisplayWriteBits(data >> 4);    // Send the high-order 4 bits of the command.
  MCPDisplayWriteBits(data & 0x0F);  // Send the low-order 4 bits of the command.
  delayMicroseconds(100);            // Wait until the display executes the command.
}

/*
Each display line has its own starting address in the display controller memory (DDRAM - display memory).

  Line 0: 0x00

  Line 1: 0x40

  Line 2: 0x14

  Line 3: 0x54

The cursor position command always starts with 0x80:
  0x80 | (column + rowAddr[row]) - We take the base address of the row, add the offset (column), and combine it with the 0x80 command using the bitwise OR (|).
*/
void MCPDisplayCursorSet(byte column, byte row) {
  byte rowAddr[] = { 0x00, 0x40, 0x14, 0x54 };
  MCPDisplayCommandSend(0x80 | (column + rowAddr[row]));
}

/*
Original string in memory: "Hi!" + null terminator:
  Addresses: [100] [101] [102] [103]
  Values: 'H' 'i' '!' '\0'

Iterations:
  1. str = 100, *str = 'H' → send 'H', str++ → 101
  2. str = 101, *str = 'i' → send 'i', str++ → 102
  3. str = 102, *str = '!' → send '!', str++ → 103
  4. str = 103, *str = '\0' → exit loop
*/
void MCPDisplayPrint(const char* str) {  // Pointer to one row.
  while (*str) {                         // Loop until the null terminator is reached.
    MCPDisplayDataSend(*str++);          // Send the current character and move the pointer.
  }
}
