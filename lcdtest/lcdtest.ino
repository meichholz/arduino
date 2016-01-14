
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
    App();
    void iterate();
  private:
    Lcd     * m_display;
    Speaker * m_speaker;
};

App::App()
{
  m_display = new Lcd(2, 11, 12);
  m_speaker = new Speaker(13);
  m_display->print("play...");
  m_speaker->play(Speaker::MelodyGreeter);
}

void App::iterate()
{
  delayMicroseconds(10000); // some 100 Hz sampling rate
  m_speaker->iterate();
  m_display->iterate();
}

class App * app_p;

void setup() {
  app_p = new App;
}

void loop() {
  app_p->iterate();
}

