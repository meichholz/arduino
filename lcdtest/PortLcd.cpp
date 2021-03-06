#include "Arduino.h"
#include "PortLcd.h"

// Alternative Library: http://playground.arduino.cc/Main/SoftwareI2CLibrary

// ==========================================================================
// specific class: PortLcd
// ==========================================================================

PortLcd::PortLcd(int columns, int rows) :
  Lcd(columns, rows),
  _portval(0x00),
  _state_rs(false),
  _state_rw(false),
  _state_e(false),
  _state_bl(true),
  _pin_rw(1), _pin_e(2), _pin_rs(0), _pin_bl(3),
  _pin_d4(4), _pin_d5(5), _pin_d6(6), _pin_d7(7)
{
}

void PortLcd::setRS(bool state)
{
  _state_rs = state;
  sync();
}

void PortLcd::setE(bool state)
{
  _state_e = state;
  sync();
}

void PortLcd::setBL(bool state)
{
  _state_bl = state;
  sync();
}

// log wait for a command
void PortLcd::wait()
{
  delay(3);
}

void PortLcd::setPortBit(int bitno, bool bitval)
{
  byte mask = 0xFF & ( ~ (1 << bitno) );
  byte val = (!!bitval) << bitno;
  _portval = (_portval & mask) | val;
}

void PortLcd::writeNibble(unsigned char nibble)
{
  setPortBit(_pin_d4, nibble&0x01);
  setPortBit(_pin_d5, nibble&0x02);
  setPortBit(_pin_d6, nibble&0x04);
  setPortBit(_pin_d7, nibble&0x08);
  sync();
  setE();
  delayMicroseconds(2);
  clearE();
  delayMicroseconds(2);
}

void PortLcd::sync()
{
  setPortBit(_pin_e, _state_e);
  setPortBit(_pin_rs, _state_rs);
  setPortBit(_pin_rw, _state_rw);
  setPortBit(_pin_bl, _state_bl);
  writePort(_portval);
}

void PortLcd::writeByte(unsigned char byte_ch, bool as_data)
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

void PortLcd::begin(int pin_e, int pin_rw, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7, int pin_bl)
{
  _pin_e = pin_e;  _pin_rs = pin_rs; _pin_rw = pin_rw;
  _pin_d4 = pin_d4; _pin_d5 = pin_d5; _pin_d6 = pin_d6; _pin_d7 = pin_d7;
  _pin_bl = pin_bl;
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

