#ifndef _H_LCD_2004
#define _H_LCD_2004

#include "PortLcd.h"

// Lcd2004 implements a SainSmart coupled LCD with 4x20 characters and a PCF based I2C interface.
// It uses the Wire library directly.

class Lcd2004 : public PortLcd  {
  public:
  
       Lcd2004();
  void begin(int i2c_addr);
  
  protected:

  int _addr;
  virtual void writePort(byte bits);
  
};

#endif



