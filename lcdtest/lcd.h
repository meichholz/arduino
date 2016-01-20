#ifndef _H_LCD
#define _H_LCD

class Lcd {
  public:
    Lcd(int columns, int rows);
    void iterate();

  protected:
    int  _rows;
    int  _columns;
    bool _cursor_is_visible;
    bool _cursor_is_blinking;
    bool _display_is_visible;
    int  _face_size;

  public:
    // base services. public face
    void print(const char *pch);
    void print(const __FlashStringHelper *flash_string);
    void print(char chData);
    void home();
    void clear();
    void command(byte chCommand);
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
    int columns() { return _columns; }
    int rows() { return _rows; }
    int faceSize() { return _face_size; }
    
    
    // device specific interface
    virtual void gotoXY(int x, int y) = 0;
    
  protected:
    // base interface
    virtual void writeByte(unsigned char byte_ch, bool as_data) = 0;
    virtual void wait() = 0; // long command wait
    void applyControls();

 
};

class LcdOnPorts : public Lcd {

  public:
    
    LcdOnPorts(int columns, int rows);
    virtual void begin(int pin_e, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7);
    virtual void gotoXY(int x, int y);
    
  protected:
    int  _pin_e, _pin_rs;
    int  _pin_d4, _pin_d5, _pin_d6, _pin_d7;
    
    virtual void writeByte(unsigned char byte_ch, bool as_data);
    virtual void wait(); // long command wait
    void setE(bool state);
    void setE() { setE(true); }
    void clearE() { setE(false); }
    
    void setRS(bool state);
    void setRS() { setRS(true); }
    void clearRS() { setRS(false); }

    void writeNibble(unsigned char nibble);
};

class LcdOnI2c : public Lcd {

  public:
    
    LcdOnI2c(int columns, int rows);
    virtual void begin(int i2c_addr, int pin_e, int pin_rw, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7, int pin_bl);
    virtual void gotoXY(int x, int y);
    
  protected:
    char _pin_e, _pin_rs;
    char _pin_rw, _pin_bl;
    char _pin_d4, _pin_d5, _pin_d6, _pin_d7;
    byte _portval;
    byte _addr;
    bool _state_rs;
    bool _state_rw;
    bool _state_e;
    bool _state_bl;
    
    virtual void writeByte(unsigned char byte_ch, bool as_data);
    virtual void wait(); // long command wait
    void setE(bool state);
    void setE() { setE(true); }
    void clearE() { setE(false); }
    
    void setRS(bool state);
    void setRS() { setRS(true); }
    void clearRS() { setRS(false); }

    void setPortBit(int bitno, bool bitval);
    void writePort();
    void writeNibble(unsigned char nibble);
};

#endif


