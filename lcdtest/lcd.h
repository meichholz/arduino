#ifndef _H_LCD
#define _H_LCD

class Lcd {
  public:
    Lcd(char pin_d4, char pin_e, char pin_rs);
    
    // the rest of the interface will be a generic interface
    void iterate();
    void print(const char *pch);
    
  protected:
    // base interface
    void setup();
    void writeByte(unsigned char byte_ch);

    // interface specific
    void writeNibble(unsigned char nibble);
    char  m_pin_e;
    char  m_pin_rs;
    char  m_pin_d_base;
    
};

#endif


