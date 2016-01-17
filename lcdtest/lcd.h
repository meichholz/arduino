#ifndef _H_LCD
#define _H_LCD

class Lcd {
  public:
    Lcd();
    virtual ~Lcd();
    virtual void setup() = 0;
    virtual void iterate();
    
    // the rest of the interface will be a generic interface
    virtual void print(const char *pch);
    virtual void print(const __FlashStringHelper *flash_string);
    virtual void home();
    virtual void clear();
    virtual void defineChar(int charnum, const byte *face);
    virtual void defineChar_p(int charnum, const byte *face);
    virtual void setScrolling(bool reverse=false, bool screenshifting=false);
    virtual void shiftScreenLeft();
    virtual void shiftScreenRight();
    virtual void moveCursorLeft();
    virtual void moveCursorRight();
    virtual void showCursor();
    virtual void showBlinkingCursor();
    virtual void hideCursor();
    virtual void showDisplay();
    virtual void hideDisplay();
    virtual void gotoXY(int x, int y) = 0;
    virtual int faceSize() = 0;
    
  protected:
    // base interface
    virtual void writeByte(unsigned char byte_ch) = 0;
    virtual void wait() = 0; // long command wait
    virtual void setE() = 0;
    virtual void clearE() = 0;
    virtual void setRS() = 0;
    virtual void clearRS() = 0;
    virtual void writeNibble(unsigned char nibble) = 0;
    virtual void applyControls();

    bool m_cursor_is_visible;
    bool m_cursor_is_blinking;
    bool m_display_is_visible;
 
};

class Lcd1602 : public Lcd {

  public:
    
    Lcd1602(int pin_d4, int pin_e, int pin_rs);
    virtual ~Lcd1602();
    virtual int faceSize() { return 8; }
    virtual void setup();
    virtual void gotoXY(int x, int y);
    
  protected:
    int  m_pin_e;
    int  m_pin_rs;
    int  m_pin_d_base;
    
    virtual void writeByte(unsigned char byte_ch);
    virtual void wait(); // long command wait
    virtual void setE();
    virtual void clearE();
    virtual void setRS();
    virtual void clearRS();

    void writeNibble(unsigned char nibble);
};

#endif


