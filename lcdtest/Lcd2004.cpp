#include "Arduino.h"
#include "Lcd2004.h"
#include "Wire.h"

// the pinout may be considered semi standard
#define LCD_PIN_BL      3
#define LCD_PIN_E       2
#define LCD_PIN_RW      1
#define LCD_PIN_RS      0
#define LCD_PIN_D4      4
#define LCD_PIN_D5      5
#define LCD_PIN_D6      6
#define LCD_PIN_D7      7

Lcd2004::Lcd2004() :
  PortLcd(20,4),
  _addr(0x3F)
{
}

void Lcd2004::begin(int i2c_addr)
{
  _addr = i2c_addr;
  PortLcd::begin(
    LCD_PIN_E, LCD_PIN_RW, LCD_PIN_RS,
    LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7,
    LCD_PIN_BL);
}

void Lcd2004::writePort(byte bits)
{
  Wire.beginTransmission(_addr);
  Wire.write(bits);
  Wire.endTransmission();
}


