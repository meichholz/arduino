#ifndef _H_BITBANG_LCD
#define _H_BITBANG_LCD

#include "Lcd.h"

class BitBangLcd : public Lcd {

  public:
    
    BitBangLcd(int columns, int rows);
    virtual void begin(int pin_e, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7);
    
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

#endif




