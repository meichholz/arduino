#ifndef _H_WIRED_LCD
#define _H_WIRED_LCD

#include "Lcd.h"

class WiredLcd : public Lcd {

  public:
    
    WiredLcd(int columns, int rows);
    virtual void begin(int i2c_addr, int pin_e, int pin_rw, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7, int pin_bl);
    
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


