#include "lcd.h"

// ==========================================================================
// specific class: LcdOnPorts
// ==========================================================================

LcdOnPorts::LcdOnPorts(int columns, int rows) :
  Lcd(columns, rows),
  _pin_e(-1), _pin_rs(-1),
  _pin_d4(-1), _pin_d5(-1), _pin_d6(-1), _pin_d7(-1)
{
}

void LcdOnPorts::setRS(bool state)
{
  digitalWrite(_pin_rs, state ? HIGH : LOW);
}

void LcdOnPorts::setE(bool state)
{
  digitalWrite(_pin_e, state ? HIGH : LOW);
}

void LcdOnPorts::gotoXY(int x, int y)
{
  command(0x80 | x | (0x40*y));
}

// log wait for a command
void LcdOnPorts::wait()
{
  delay(3);
}

void LcdOnPorts::writeNibble(unsigned char nibble)
{
  digitalWrite(_pin_d4, (nibble&0x01) ? HIGH : LOW);
  digitalWrite(_pin_d5, (nibble&0x02) ? HIGH : LOW);
  digitalWrite(_pin_d6, (nibble&0x04) ? HIGH : LOW);
  digitalWrite(_pin_d7, (nibble&0x08) ? HIGH : LOW);
  setE();
  delayMicroseconds(2);
  clearE();
  delayMicroseconds(2);
}

void LcdOnPorts::writeByte(unsigned char byte_ch, bool as_data)
{
  if (as_data) {
    setRS();
  }
  writeNibble((byte_ch>>4) & 0x0F);
  writeNibble(byte_ch & 0x0F);
  if (as_data) {
    clearRS();
  }
  delayMicroseconds(60);
}

void LcdOnPorts::begin(int pin_e, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7)
{
  _pin_e = pin_e;  _pin_rs = pin_rs;
  _pin_d4 = pin_d4; _pin_d5 = pin_d5; _pin_d6 = pin_d6; _pin_d7 = pin_d7;
  pinMode(_pin_e, OUTPUT);
  pinMode(_pin_rs, OUTPUT);
  pinMode(_pin_d4, OUTPUT);
  pinMode(_pin_d5, OUTPUT);
  pinMode(_pin_d6, OUTPUT);
  pinMode(_pin_d7, OUTPUT);
  // reliable initialization procedure for 4 bit
  clearE();
  setRS();
  delayMicroseconds(10);
  clearRS();
  delayMicroseconds(15000); // wait for power up
  writeNibble(0x03); // be 8 bit
  delayMicroseconds(4100);
  writeNibble(0x03); // again,be 8 bit (if come from 4 bit mode):-)
  delayMicroseconds(100);
  writeNibble(0x03); // last time, be 8 bit
  delay(1);
  writeNibble(0x02); // now: be 4 bit (and ignore rest, like above)
  command(0x2C); // ok, final fix: 4 bit (0x20), 2 lines (0x08) and font 5x10 (0x04)
  applyControls();
  clear();
  home();
}


