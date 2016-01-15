
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
  public:
    App(int pollfreq);
    void iterate();
    void setup();
    void pushPhase(int i);
  private:
    Lcd     *m_display;
    Speaker *m_speaker;
    int     ticks;
    int     phase;
    int     pollfreq;
};

static const byte char_face[] PROGMEM = {
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
  pollfreq(pollfreq)
{
}

void App::setup()
{
  m_display = new Lcd(2, 11, 12);
  m_speaker = new Speaker(13, pollfreq);
  m_display->home();
  m_display->print("\1\1\1 play \1\1\1");
  m_speaker->play(Speaker::MelodyGreeter);
}

void App::iterate()
{
  m_speaker->iterate();
  m_display->iterate();
  ticks--;
  if (ticks<=0) {
    ticks = 12;
    pushPhase(phase++);
    if (phase>=8) phase=0;
  }
}

void App::pushPhase(int i)
{
  m_display->defineChar_P(1, 8, char_face+i);
}

App app(20);

void setup() {
  app.setup();
}

void loop() {
  app.iterate();
}

