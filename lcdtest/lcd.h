#ifndef _H_LCD
#define _H_LCD

class Lcd {
  public:
    Lcd();
    ~Lcd();
    void iterate();
    
    // base services. public face
    void print(const char *pch);
    void print(const __FlashStringHelper *flash_string);
    void home();
    void clear();
    void defineChar(int charnum, const byte *face);
    void defineChar_p(int charnum, const byte *face);
    void setScrolling(bool reverse=false, bool screenshifting=false);
    void shiftScreenLeft();
    void shiftScreenRight();
    void moveCursorLeft();
    void moveCursorRight();
    void showCursor();
    void showBlinkingCursor();
    void hideCursor();
    void showDisplay();
    void hideDisplay();
    
    // device specific interface
    virtual void setup() = 0;
    virtual void gotoXY(int x, int y) = 0;
    virtual int faceSize() = 0;
    
  protected:
    // base interface
    virtual void writeByte(unsigned char byte_ch, bool as_data=false) = 0;
    virtual void wait() = 0; // long command wait
    void applyControls();

    bool m_cursor_is_visible;
    bool m_cursor_is_blinking;
    bool m_display_is_visible;
 
};

class Lcd1602 : public Lcd {

  public:
    
    Lcd1602(int pin_d4, int pin_e, int pin_rs);
    ~Lcd1602();
    virtual int faceSize() { return 8; }
    virtual void setup();
    virtual void gotoXY(int x, int y);
    
  protected:
    int  m_pin_e;
    int  m_pin_rs;
    int  m_pin_d_base;
    
    virtual void writeByte(unsigned char byte_ch, bool as_data=false);
    virtual void wait(); // long command wait
    void setE(bool state);
    void setRS(bool state);

    void writeNibble(unsigned char nibble);
};

#endif


