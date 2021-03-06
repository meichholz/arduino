#include "Arduino.h"
#include "Lcd.h"

// see http://mil.ufl.edu/3744/docs/lcdmanual/commands.html for bit meanings

// ==========================================================================
// base class: Lcd
// ==========================================================================

Lcd::Lcd(int columns, int rows) :
  _face_size(8),
  _cursor_is_visible(false),
  _cursor_is_blinking(false),
  _display_is_visible(true),
  _autoscrolling(false),
  _columns(columns), _rows(rows)
{
}

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

void Lcd::autoscroll()
{
  _autoscrolling = true;
  applyControls();
}

void Lcd::noAutoscroll()
{
  _autoscrolling = false;
  applyControls();
}

void Lcd::setCursor(int x, int y)
{
  command(0x80 | x | (0x40*y)); // todo: virtualize address offset calculation
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
    // TODO: Autoscrolling ?
}

void Lcd::cursor()
{
  _cursor_is_visible=true;
  _cursor_is_blinking=false;
  applyControls();
}

void Lcd::blink()
{
  _cursor_is_visible=true;
  _cursor_is_blinking=true;
  applyControls();
}

void Lcd::noCursor()
{
  _cursor_is_visible=false;
  applyControls();
}

void Lcd::display()
{
  _display_is_visible = true;
  applyControls();
}

void Lcd::noDisplay()
{
  _display_is_visible = false;
  applyControls();
}

void Lcd::scrollDisplayLeft()
{
  command(0b00011000);
}

void Lcd::scrollDisplayRight()
{
  command(0b00011100);
}

void Lcd::rightToLeft()
{
  command(0b00010000);
}

void Lcd::leftToRight()
{
  command(0b00010100);
}


