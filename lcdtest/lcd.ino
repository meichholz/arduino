#include "lcd.h"

Lcd::Lcd(char pin_d4, char pin_e, char pin_rs) :
  m_pin_e(pin_e),
  m_pin_rs(pin_rs),
  m_pin_d_base(pin_d4)
{
}

void Lcd::iterate()
{
// nothing, yet, but probably implement some autonomous dynamic here...
}

void Lcd::print(const char *pch)
{
  setRS();
  while (*pch) {
    writeByte(*pch);
    pch++;
  }
  digitalWrite(m_pin_rs, LOW);
}

void Lcd::defineChar(int charnum, int byte_c, const byte *bits)
{
  int i;
  clearRS();
  writeByte(0x40+8*charnum);
  setRS();
  for (i=0; i<byte_c;  i++) {
    writeByte(bits[i]);
  }
  clearRS();
}
void Lcd::defineChar_P(int charnum, int byte_c, const byte *bits)
{
  int i;
  clearRS();
  writeByte(0x40+8*charnum);
  setRS();
  for (i=0; i<byte_c;  i++) {
    writeByte(pgm_read_byte(bits+i));
  }
  clearRS();
}

void Lcd::setRS()
{
  digitalWrite(m_pin_rs, HIGH);
}

void Lcd::clearRS()
{
  digitalWrite(m_pin_rs, LOW);
}

void Lcd::setE()
{
  digitalWrite(m_pin_e, HIGH);
}

void Lcd::clearE()
{
  digitalWrite(m_pin_e, LOW);
}

void Lcd::home()
{
  writeByte(0x02);
  wait();
}

// log wait for a command
void Lcd::wait()
{
  delay(3);
}

void Lcd::clear()
{
  writeByte(0x01); // clear display
  wait();
}

// internals


void Lcd::writeNibble(unsigned char nibble)
{
  for (int i=0; i<4; i++) {
    digitalWrite(m_pin_d_base+i, (nibble & (8>>i)) ? HIGH : LOW);
  }
  setE();
  delayMicroseconds(2);
  clearE();
  delayMicroseconds(2);
}

void Lcd::writeByte(unsigned char byte_ch)
{
  writeNibble((byte_ch>>4) & 0x0F);
  writeNibble(byte_ch & 0x0F);
  delayMicroseconds(60);
}

void Lcd::setup()
{
  pinMode(m_pin_e, OUTPUT);
  pinMode(m_pin_rs, OUTPUT);
  for (int i=0; i<4; i++) {
    pinMode(m_pin_d_base+i, OUTPUT);
  }
  clearE();
  setRS();
  // reliable initialization procedure for 4 bit
  clearRS();
  delayMicroseconds(15000); // wait for power up
  writeNibble(0x03); // be 8 bit
  delayMicroseconds(4100);
  writeNibble(0x03); // again,be 8 bit (if come from 4 bit mode):-)
  delayMicroseconds(100);
  writeNibble(0x03); // last time, be 8 bit
  delay(1);
  writeNibble(0x02); // now: be 4 bit (and ignore rest, like above)
  writeByte(0x2C); // ok, final fix: 4 bit (0x20), 2 lines (0x08) and font 5x10 (0x04)
  writeByte(0x0C); // display (0x04): on (0x04), cursor (0x02): off, blink (0x01): off
  clear();
  home();
}


