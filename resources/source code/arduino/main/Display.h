#include <Wire.h>
#include <Adafruit_MCP23008.h>

Adafruit_MCP23008 mcp;

#define MCP_RS 0
#define MCP_EN 2
#define MCP_D4 4
#define MCP_D5 5
#define MCP_D6 6
#define MCP_D7 7

void write4bits(byte value) {
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

void sendCommand(byte cmd) {
  mcp.digitalWrite(MCP_RS, LOW);
  write4bits(cmd >> 4);
  write4bits(cmd & 0x0F);
  delay(2);
}

void sendData(byte data) {
  mcp.digitalWrite(MCP_RS, HIGH);
  write4bits(data >> 4);
  write4bits(data & 0x0F);
  delayMicroseconds(100);
}

void setCursor(byte col, byte row) {
  // Универсальные адреса для 4x20 дисплея
  byte rowAddr[] = {0x00, 0x40, 0x14, 0x54};
  sendCommand(0x80 | (col + rowAddr[row]));
}

void print(const char* str) {
  while (*str) {
    sendData(*str++);
  }
}