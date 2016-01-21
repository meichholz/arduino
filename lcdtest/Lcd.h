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
    bool _autoscrolling;
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
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void rightToLeft();
    void leftToRight();
    void cursor();
    void blink();
    void noCursor();
    void display();
    void noDisplay();
    void autoscroll();
    void noAutoscroll();
    int columns() { return _columns; }
    int rows() { return _rows; }
    int faceSize() { return _face_size; }
    
    
    // device specific interface
    void setCursor(int x, int y);
    
  protected:
    // base interface
    virtual void writeByte(unsigned char byte_ch, bool as_data) = 0;
    virtual void wait() = 0; // long command wait
    void applyControls();

 
};


#endif


