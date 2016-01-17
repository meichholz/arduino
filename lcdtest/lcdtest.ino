
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

static const byte cg_copyright[] PROGMEM = {
  0b01110,
  0b10001,
  0b10111,
  0b11001,
  0b11001,
  0b10111,
  0b10001,
  0b01110,
};

static const byte cg_face[] PROGMEM = {
  0b00000,
  0b11011,
  0b00000,
  0b00100,
  0b00100,
  0b10001,
  0b01110,
  0b00000,
  
  0b00000,
  0b11011,
  0b00000,
  0b00100,
  0b00100,
  0b10001,
  0b01110,
  0b00000,
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
  display.gotoXY(13,0);
  display.showBlinkingCursor();
  display.setScrolling(true); // reverse
  display.print(F("\1\1\1 yalp \1\1\1"));
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
      ticks = ticksFor(500); // 0,5 sec cycle
      display.defineChar_p(1, cg_face+phase);
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



