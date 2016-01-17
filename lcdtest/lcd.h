#ifndef _H_LCD
#define _H_LCD

class Lcd {
  public:
    Lcd(char pin_d4, char pin_e, char pin_rs);
    void setup();
    void iterate();
    
    // the rest of the interface will be a generic interface
    void print(const char *pch);
    void print(const __FlashStringHelper *flash_string);
    void home();
    void clear();
    void defineChar(int charnum, const byte *face);
    void defineChar_p(int charnum, const byte *face);
    void setScrolling(bool reverse=false, bool screenshifting=false);
    void gotoXY(int x, int y);
    void shiftScreenLeft();
    void shiftScreenRight();
    void moveCursorLeft();
    void moveCursorRight();
    void showCursor();
    void showBlinkingCursor();
    void hideCursor();
    void showDisplay();
    void hideDisplay();
    int  faceSize() { return 8; }
    
  protected:
    // base interface
    void writeByte(unsigned char byte_ch);
    void setE();
    void clearE();
    void setRS();
    void clearRS();
    void wait(); // long command wait

    void applyControls();

    bool m_cursor_is_visible;
    bool m_cursor_is_blinking;
    bool m_display_is_visible;

    // interface specific
    void writeNibble(unsigned char nibble);
    char  m_pin_e;
    char  m_pin_rs;
    char  m_pin_d_base;
    
};

#endif


