
// initially testbed for home made bitbang LCD 1602 interface
// (4bit mode, just for fun, from the spec 2015)
// Also for UI concepts, I2C testing and so on.
// 
// GND -> RW, K, VSS
// 5V -> A, VDD
// Poti -> V0

#include "Speaker.h"
#include "Lcd2004.h"

#include "Wire.h"

#define LCD_I2C_ADDR  0x27

#define PIN_SPEAKER 8

class App {
  enum TAppState {
    AppStateInit,
    AppStateGreeting,
    AppStateDemo,
  };
  public:
    App(int pollfreq);
    void iterate();
    void setup();
  private:
    Lcd2004     _display;
    TAppState   _state;
    Speaker     _speaker;
    int         _ticks;
    int         _phase;
    int         _pollfreq; // in Hertz
    
    void    startUI();
    void    startDemo();
    int     ticksFor(int millisecs);
    void    refreshState();
};

App app(100);

void setup() {
  app.setup();
}

void loop() {
  app.iterate();
}

// and now for the details
// cg helper: http://www.8051projects.net/lcd-interfacing/lcd-custom-character.php

static const byte cg_copyright[] PROGMEM = { 0xe, 0x11, 0x17, 0x19, 0x19, 0x17, 0x11, 0xe };
static const byte cg_face[] PROGMEM = {
  0x00,0x1b,0x00,0x04,0x04,0x11,0x0e,0x00,
  0x00,0x1b,0x00,0x04,0x04,0x11,0x0e,0x00,
  };
static const byte cg_clock[] PROGMEM = {
  0x0, 0x0, 0x1f, 0x11, 0x17, 0x11, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x11, 0x17, 0x15, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x11, 0x15, 0x15, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x11, 0x1d, 0x15, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x11, 0x1d, 0x11, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x15, 0x1d, 0x11, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x15, 0x15, 0x11, 0x1f, 0x0,
  0x0, 0x0, 0x1f, 0x15, 0x17, 0x11, 0x1f, 0x0,
};

App::App(int pollfreq) :
  _ticks(0),
  _phase(0),
  _pollfreq(pollfreq),
  _speaker(),
  _display(),
  _state(AppStateInit)
{
}

void App::startUI()
{
  // display.gotoXY(13,0);
  _display.home();
  //_display.showBlinkingCursor();
  // display.setScrolling(true); // reverse
  _display.print(F("\3\3 losa geht\1 \3\3"));
}

void App::startDemo()
{
  _display.clear();
  startUI();
}

void App::setup()
{
  Wire.begin();
  // TODO: Abstraktion von Wire
  // TODO: Zeilenstartberechnung
  _display.begin(LCD_I2C_ADDR);
  _speaker.begin(PIN_SPEAKER, _pollfreq);
  _speaker.play(Speaker::MelodyGreeter);
  _display.defineChar_p(2, cg_copyright);
  _display.defineChar_p(1, cg_face);
  _display.home();
  _display.print(F("***  LCD-UI  ***"));
  _display.setCursor(0,1);
  _display.print(F("\2M.Eichholz 2015"));
  _state = AppStateGreeting;
  _ticks = ticksFor(2000); // 2 seconds
}

void App::iterate()
{
  _speaker.delayLoop();
  _speaker.iterate();
  _display.iterate();
  _ticks--;
  if (_ticks<=0) {
    _ticks = 1; // default: just minimal re-check
    refreshState();
  }
}

void App::refreshState()
{
  switch (_state) {
    case AppStateGreeting:
      _state = AppStateDemo;
      _phase = 1;
      startDemo();
      break;
    case AppStateDemo:
      _ticks = ticksFor(150);
      _display.defineChar_p(1, cg_face + _phase);
      _display.defineChar_p(3, cg_clock + 8*_phase);
      if (++_phase >= 8) _phase = 0;
      break;
  }
}

int App::ticksFor(int millisecs)
{
  return (int)(((long)_pollfreq*(long)millisecs+500L)/1000L);
}



