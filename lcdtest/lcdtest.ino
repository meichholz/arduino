
// home made bitbang LCD 1602 interface, 4bit mode
// just for fun, from the spec 2015
// 
// Arduino -> Shield
// D2..D5 -> D4..D7
// D11 -> E
// D12 -> RS
// GND -> RW, K, VSS
// 5V -> A, VDD
// Poti -> V0
// D13 -> Buzzer

#include "speaker.h"
#include "lcd.h"

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
    Lcd1602 display;
    Speaker speaker;
    int     ticks;
    int     phase;
    TAppState state;
    int     pollfreq; // in Hertz
    
    void    startUI();
    void    startDemo();
    int     ticksFor(int millisecs);
    void    delayLoop();
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
  ticks(0),
  phase(0),
  pollfreq(pollfreq),
  speaker(13, pollfreq),
  display(2, 11, 12),
  state(AppStateInit)
{
}

void App::startUI()
{
  // display.gotoXY(13,0);
  display.home();
  display.showBlinkingCursor();
  // display.setScrolling(true); // reverse
  display.print(F("\3\3 losa geht\1 \3\3"));
}

void App::startDemo()
{
  display.clear();
  startUI();
}

void App::setup()
{
  display.setup(); // caveat: the arduino is not ready before global setup()
  speaker.setup();
  speaker.play(Speaker::MelodyGreeter);
  display.defineChar_p(2, cg_copyright);
  display.defineChar_p(1, cg_face);
  display.home();
  display.print(F("***  LCD-UI  ***"));
  display.gotoXY(0,1);
  display.print(F("\2M.Eichholz 2015"));
  state = AppStateGreeting;
  ticks = ticksFor(2000); // 2 seconds
}

void App::iterate()
{
  delayLoop();
  speaker.iterate();
  display.iterate();
  ticks--;
  if (ticks<=0) {
    ticks = 1; // default: just minimal re-check
    refreshState();
  }
}

void App::refreshState()
{
  switch (state) {
    case AppStateGreeting:
      state = AppStateDemo;
      phase = 1;
      startDemo();
      break;
    case AppStateDemo:
      ticks = ticksFor(150);
      display.defineChar_p(1, cg_face+phase);
      display.defineChar_p(3, cg_clock+8*phase);
      if (++phase >= 8) phase=0;
      break;
  }
}

void App::delayLoop()
{
  if (pollfreq >= 1000) {
    delayMicroseconds((unsigned int)(1000000L/(long)pollfreq));
  } else {
    delay((unsigned int)(1000L/(long)pollfreq));
  }
}

int App::ticksFor(int millisecs)
{
  return (int)(((long)pollfreq*(long)millisecs+500L)/1000L);
}



