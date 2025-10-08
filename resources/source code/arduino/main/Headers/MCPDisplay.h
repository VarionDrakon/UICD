#ifndef MCPDISPLAY_H
#define MCPDISPLAY_H

#include <Arduino.h>

#define MCP_RS 0  // Register Select.
#define MCP_EN 2  // Signal - Enable.
// Data bits for 4-bit interface (These are the pins/outputs of the microcontroller):
#define MCP_D4 4  // Physical pin.
#define MCP_D5 5
#define MCP_D6 6
#define MCP_D7 7

void MCPDisplayInitialize(byte address, byte column, byte row);

void MCPDisplayWriteBits(byte value);

void MCPDisplayCommandSend(byte cmd);

void MCPDisplayDataSend(byte data);

void MCPDisplayCursorSet(byte column, byte row);

void MCPDisplayPrint(const char* str);

#endif