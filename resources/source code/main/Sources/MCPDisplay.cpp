#include "../Headers/MCPDisplay.h"

extern Adafruit_MCP23008 mcp;

char* dictionaryWords[] = {
"> ",
"Baudrate",
"Address",
"UICD",
"BKЛЮЧAETCЯ",
"CBЯ3b",
"TOTAЛAИ3EPbI",
"ИHФO",
"HA3AД",
"MEHЮ:",
"CБPOCИTb",
"OTKA3",
"ДA",
"HET",
"Modbus",
"CБPOC: B",
"CБPOC: H",
"?",
"B",
"H"
};

/*
The HD44780 stores a character in a format of 8 rows × 5 pixels,
and each row is 1 byte, using the 5 least significant bits. Each row is written in binary format. 
B - binary.
*/
byte customChars[8][8] = {
  // 1 - "Я"
  {
    B01110,
    B10001,
    B10001,
    B01111,
    B00101,
    B01001,
    B10001,
    B00000
  },
  // 2 - "Л"
  {
    B00111,
    B01001,
    B10001,
    B10001,
    B10001,
    B10001,
    B10001,
    B00000
  },
  // 3 - "И"
  {
    B10001,
    B10011,
    B10101,
    B11001,
    B10001,
    B10001,
    B10001,
    B00000
  },
  // 4 - "Ф"
  {
    B00100,
    B01110,
    B10101,
    B10101,
    B10101,
    B01110,
    B00100,
    B00000
  },
  // 5 - "Д"
  {
    B11111,
    B10001,
    B10001,
    B10001,
    B10001,
    B01110,
    B10001,
    B10001
  },
  // 6 - "Ю"
  {
    B10111,
    B10101,
    B10101,
    B11101,
    B10101,
    B10101,
    B10111,
    B00000
  },
  // 7 - "Б"
  {
    B11111,
    B10000,
    B10000,
    B11110,
    B10001,
    B10001,
    B11110,
    B00000
  },
  // 8 - "Ч"
  {
    B10001,
    B10001,
    B10001,
    B11111,
    B00001,
    B00001,
    B00001,
    B00000
  }
};

/*
Here, eight user characters (each 8 bytes in 5x8 rows) are written to the HD44780 controller's CGRAM.
The CGRAM is addressed by HD44780 commands, and the byte writes themselves are sent as "data" (RS=1). 
The MCP23008 acts as a bridge: the code calls MCPDisplayCommandSend/MCPDisplayDataSend, which, through the MCP23008, assert the RS/EN/D4..D7 lines and thus transmit commands/data to the display.

for (byte i = 0; i < 8; i++) {
i is the user character index (0..7). The HD44780 supports exactly 8 CGRAM slots, so the upper limit is 8.
The loop alternately sets the CGRAM address to the beginning of each 8-byte cell and writes 8 rows of pixels.
For each i, we will send the Set CGRAM Address command with the address i*8. This means that the next data written will be directed to the CGRAM, starting from the current address in the iteration.
0x40 is the top part of the "Set CGRAM Address" command code (in HD44780 this is the command code 01xxxxxx).

Here 0x40 | (i << 3):
i << 3 = i * 8 - because each character takes up 8 bytes.
0x40 is the upper part of the "Set CGRAM Address" command code (in the HD44780, this is the command code 01xxxxxx).
Result: 0x40 + i * 8 is the six-bit CGRAM address (a value from 0x40 to 0x7F, where 0x40 - 0 is the start of the CGRAM).
Example: i = 2 -> 0x40 | (2 << 3) = 0x40 | 0x10 = 0x50 - the HD44780 will understand "write to CGRAM, address 0x10 (internally 16)".

for (byte x = 0; x < 8; x++) {
Inner loop over the rows of the current character: 8 rows (0..7). Each row is one byte, using only the 5 least significant bits (D0..D4) for pixels.
Next, 8 bytes are written in a row. The HD44780 increments the internal CGRAM address by 1 with each write, so 8 consecutive DataSend calls fill the entire character.
*/
void MCPDisplayLoadCustomChars() {
    for (byte i = 0; i < 8; i++) {
        MCPDisplayCommandSend(0x40 | (i << 3));
        for (byte x = 0; x < 8; x++) {
            MCPDisplayDataSend(customChars[i][x]);
        }
    }
}

/*
cp/code point (Unicode code point) is simply the number of the character in the Unicode table.

Values ​​(0x042F, 0x041B, etc.) are Unicode code points for specific Cyrillic letters, or more precisely, Unicode codes for Russian letters.
Each character in UTF-8 must first be converted to a single number—a Unicode code point—and then returned as an index in the array for drawing the letter (pixels) on the screen.

If the Cyrillic character is not included in the array, a fallback option is performed:
default:
    if (cp < 128)
        return (uint8_t)cp;
    return '?';

If cp < 128 then the character is a regular ASCII character, othersite (Latin, numbers, space, punctuation marks) return the same character without changes.
Otherwise (if cp >= 128)
it means either:
* an unsupported Russian letter,
* something from Unicode,
* emoji,
* a combining character,
* unreadable UTF-8,
* complete garbage.
*/
uint8_t MCPDisplayTranslateChar(uint16_t cp)
{
    switch (cp)
    {
        case 0x042F: return 0; // Я
        case 0x041B: return 1; // Л
        case 0x0418: return 2; // И
        case 0x0424: return 3; // Ф
        case 0x0414: return 4; // Д
        case 0x042E: return 5; // Ю
        case 0x0411: return 6; // Б
        case 0x0427: return 7; // Ч
        default:
            if (cp < 128)
                return (uint8_t)cp;
            return '?';
    }
}

/*
s - is a pointer to a C string in memory:
*s gives the current byte. Strings in C are null-terminated ('\0'), so while (*s) is a standard loop over the string.

c < 0x80 - a quick test for ASCII:
all single-byte UTF-8 characters are in this range. They can be sent without decoding.

The test (c & 0xE0) == 0xC0 is strictly UTF-8 compliant: the beginning of the two-byte sequence is 110xxxxx (the upper three bits are 110).
Decoding ((c1 & 0x1F) << 6) | (c2 & 0x3F) - converts two UTF-8 bytes to Unicode code points. For Cyrillic, this yields values ​​of the form 0x0400..0x04FF.

MCPDisplayTranslateChar(code) is a bridge between Unicode and what the display can show: 
it returns the CGRAM index (0..7) for loaded custom codes or ASCII/'?'.

MCPDisplayDataSend(...) - essentially writes a byte to the HD44780 via the MCP23008:
sets RS=1, writes nibble by nibble, and strobes EN. Over I2C, each MCP23008 manipulation (set pins, change EN) is a separate transaction; therefore, the operation is not instantaneous, but correct.

Truncated string protection:
If the second byte is missing (if (!c2)), the function sends '?' and aborts execution to avoid reading memory beyond the end of the string.

3- and 4-byte sequences are not processed here:
they are not recognized and are sent to MCPDisplayDataSend('?'). This is a simple and safe behavior for an embedded system with a limited character set.
*/
void MCPDisplayPrintUTF(const char* s)
{
    while (*s)
    {
        uint8_t c = (uint8_t)*s;
        if (c < 0x80)
        {
            MCPDisplayDataSend(c);
            s++;
            continue;
        }
        if ((c & 0xE0) == 0xC0)
        {
            uint8_t c1 = c;
            uint8_t c2 = (uint8_t)*(s + 1);
            if (!c2)
            {
                MCPDisplayDataSend('?');
                return;
            }
            uint16_t code = ((c1 & 0x1F) << 6) | (c2 & 0x3F);
            uint8_t out = MCPDisplayTranslateChar(code);
            MCPDisplayDataSend(out);
            s += 2;
            continue;
        }
        MCPDisplayDataSend('?');
        s++;
    }
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

/*
Loop through all MCP23008 pins from 0 to 7:
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

  MCPDisplayLoadCustomChars();

  MCPDisplayCommandSend(0x01);
  delay(10);
  
  MCPDisplayCursorSet(2, 1);
  MCPDisplayPrintUTF("UICD BKЛЮЧAETCЯ...");
  // MCPDisplayPrint("UICD loading...");
  delay(500);

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
  // delayMicroseconds(10);
  mcp.digitalWrite(MCP_EN, HIGH);
  // delayMicroseconds(10);
  mcp.digitalWrite(MCP_EN, LOW);
  // delayMicroseconds(100);
}

void MCPDisplayCommandSend(byte cmd) {
  mcp.digitalWrite(MCP_RS, LOW);    // We tell the display that we are sending a command.
  MCPDisplayWriteBits(cmd >> 4);    // Send the high-order 4 bits of the command.
  MCPDisplayWriteBits(cmd & 0x0F);  // Send the low-order 4 bits of the command.
  // delay(2);                         // Wait until the display executes the command.
}

void MCPDisplayDataSend(byte data) {
  mcp.digitalWrite(MCP_RS, HIGH);    // We tell the display that we are sending a data.
  MCPDisplayWriteBits(data >> 4);    // Send the high-order 4 bits of the command.
  MCPDisplayWriteBits(data & 0x0F);  // Send the low-order 4 bits of the command.
  // delayMicroseconds(100);            // Wait until the display executes the command.
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