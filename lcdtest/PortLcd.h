#ifndef _H_PORT_LCD
#define _H_PORT_LCD

#include "Lcd.h"

// PortLcd contains everything to drive an LCD on **one** abstract port.
// It is an abstract base class.
// You derive Your class and wire it to this one by implenting the `writePort(val)` member function.

class PortLcd : public Lcd {

  public:
    
    PortLcd(int columns, int rows);
    virtual void begin(int pin_e, int pin_rw, int pin_rs, int pin_d4, int pin_d5, int pin_d6, int pin_d7, int pin_bl);
    
  protected:
    char _pin_e, _pin_rs;
    char _pin_rw, _pin_bl;
    char _pin_d4, _pin_d5, _pin_d6, _pin_d7;
    byte _portval;
    bool _state_rs;
    bool _state_rw;
    bool _state_e;
    bool _state_bl;
    
    virtual void writePort(byte bits) = 0; // must be defined by child
    
    virtual void writeByte(unsigned char byte_ch, bool as_data);
    virtual void wait(); // long command wait
    
    void setE(bool state);
    void setE() { setE(true); }
    void clearE() { setE(false); } 
    void setRS(bool state);
    void setRS() { setRS(true); }
    void clearRS() { setRS(false); }
    void setBL(bool state);
    void backlight() { setBL(true); }
    void noBacklight() { setBL(false); }

    void setPortBit(int bitno, bool bitval);
    void sync(); // rebuild state bits and write to the port implicitly
    void writeNibble(unsigned char nibble);
};

#endif


