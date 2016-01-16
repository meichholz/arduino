#include "lcd.h"

// see http://mil.ufl.edu/3744/docs/lcdmanual/commands.html for bit meanings

Lcd::Lcd(char pin_d4, char pin_e, char pin_rs) :
  m_pin_e(pin_e),
  m_pin_rs(pin_rs),
  m_pin_d_base(pin_d4),
  m_cursor_is_visible(false),
  m_cursor_is_blinking(false),
  m_display_is_visible(true)
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
  clearRS();
}

void Lcd::defineChar(int charnum, int byte_c, const byte *bits)
{
  int i;
  clearRS();
  writeByte(0x06); // address counter INCrement
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
  writeByte(0x06); // address counter INCrementing needing
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

void Lcd::gotoXY(int x, int y)
{
  writeByte(0x80 | x | (0x40*y));
}

void Lcd::clear()
{
  writeByte(0x01); // clear display
  wait();
}

void Lcd::setScrolling(bool reverse, bool screenshifting)
{
  writeByte(0x04
    | (screenshifting ? 0x01 : 0x00)
    | (reverse ? 0x00 : 0x02));
}

void Lcd::applyControls()
{
  writeByte(0x08 |
    (m_cursor_is_visible<<1) |
    (m_cursor_is_blinking<<0) |
    (m_display_is_visible<<2));
}

void Lcd::showCursor()
{
  m_cursor_is_visible=true;
  m_cursor_is_blinking=false;
  applyControls();
}

void Lcd::showBlinkingCursor()
{
  m_cursor_is_visible=true;
  m_cursor_is_blinking=true;
  applyControls();
}

void Lcd::hideCursor()
{
  m_cursor_is_visible=false;
  applyControls();
}

void Lcd::showDisplay()
{
  m_display_is_visible = true;
  applyControls();
}

void Lcd::hideDisplay()
{
  m_display_is_visible = false;
  applyControls();
}

void Lcd::shiftScreenLeft()
{
  writeByte(0b00011000);
}

void Lcd::shiftScreenRight()
{
  writeByte(0b00011100);
}

void Lcd::moveCursorLeft()
{
  writeByte(0b00010000);
}

void Lcd::moveCursorRight()
{
  writeByte(0b00010100);
}

// internals


// log wait for a command
void Lcd::wait()
{
  delay(3);
}

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
  applyControls();
  clear();
  home();
}


