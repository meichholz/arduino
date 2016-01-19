#include "lcd.h"

// see http://mil.ufl.edu/3744/docs/lcdmanual/commands.html for bit meanings

Lcd::Lcd(int columns, int rows) :
  _face_size(8),
  _cursor_is_visible(false),
  _cursor_is_blinking(false),
  _display_is_visible(true),
  _columns(columns), _rows(rows)
{
}

Lcd::~Lcd() {}

void Lcd::iterate()
{
// nothing, yet, but probably implement some autonomous dynamic here...
}

void Lcd::print(char chData)
{
  writeByte((unsigned char)chData, true);
}

void Lcd::command(byte chCommand)
{
  writeByte(chCommand, false);
}

void Lcd::print(const char *pch)
{
  while (*pch) {
    print(*pch);
    pch++;
  }
}

void Lcd::print(const __FlashStringHelper *flash_string)
{
  const char * pch = reinterpret_cast<const char *>(flash_string);
  char ch;
  do {
    ch = pgm_read_byte(pch);
    pch++;
    if (ch) {
      print(ch);
    }
  } while(ch);
}

void Lcd::defineChar(int charnum, const byte *face)
{
  int cch=faceSize();
  command(0x06); // address counter INCrement
  command(0x40 + cch*charnum);
  for (int i=0; i<cch;  i++) {
    print(face[i]);
  }
}

void Lcd::defineChar_p(int charnum, const byte *flash_bits)
{
  int cch=faceSize();
  command(0x06); // address counter INCrementing needing
  command(0x40 + cch*charnum);
  for (int i=0; i<cch;  i++) {
    print(pgm_read_byte(flash_bits+i));
  }
}

void Lcd::home()
{
  command(0x02);
  wait();
}

void Lcd::clear()
{
  command(0x01); // clear display
  wait();
}

void Lcd::setScrolling(bool reverse, bool screenshifting)
{
  command(0x04
    | (screenshifting ? 0x01 : 0x00)
    | (reverse ? 0x00 : 0x02));
}

void Lcd::applyControls()
{
  command(0x08 |
    (_cursor_is_visible<<1) |
    (_cursor_is_blinking<<0) |
    (_display_is_visible<<2));
}

Lcd1602::Lcd1602(int columns, int rows) :
  Lcd(columns, rows),
  _pin_e(-1), _pin_rs(-1), _pin_rw(-1),
  _pin_d4(-1), _pin_d5(-1), _pin_d6(-1), _pin_d7(-1)
{
}

Lcd1602::~Lcd1602() {}

void Lcd::showCursor()
{
  _cursor_is_visible=true;
  _cursor_is_blinking=false;
  applyControls();
}

void Lcd::showBlinkingCursor()
{
  _cursor_is_visible=true;
  _cursor_is_blinking=true;
  applyControls();
}

void Lcd::hideCursor()
{
  _cursor_is_visible=false;
  applyControls();
}

void Lcd::showDisplay()
{
  _display_is_visible = true;
  applyControls();
}

void Lcd::hideDisplay()
{
  _display_is_visible = false;
  applyControls();
}

void Lcd::shiftScreenLeft()
{
  command(0b00011000);
}

void Lcd::shiftScreenRight()
{
  command(0b00011100);
}

void Lcd::moveCursorLeft()
{
  command(0b00010000);
}

void Lcd::moveCursorRight()
{
  command(0b00010100);
}

// and now for interface and geometry specific implementations

void Lcd1602::setRS(bool state)
{
  digitalWrite(_pin_rs, state ? HIGH : LOW);
}

void Lcd1602::setE(bool state)
{
  digitalWrite(_pin_e, state ? HIGH : LOW);
}

void Lcd1602::gotoXY(int x, int y)
{
  command(0x80 | x | (0x40*y));
}

// log wait for a command
void Lcd1602::wait()
{
  delay(3);
}

void Lcd1602::writeNibble(unsigned char nibble)
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

void Lcd1602::writeByte(unsigned char byte_ch, bool as_data)
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

void Lcd1602::begin(int pin_e, int pin_rw, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7)
{
  _pin_e = pin_e;  _pin_rw = pin_rw; _pin_rs = pin_rs;
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


