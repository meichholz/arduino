#include "sevenseg.h"
#include "key.h"

class DigiDice {

  public:
    DigiDice(int speaker_pin, int pulse_pin, int key_pin);
    void setup();
    void refresh();

    // no destructor

  private:

    void refreshBrightness();
    void refreshFace();
    bool pollKey();

    int speaker_pin, pulse_pin, key_pin;

    class SevenSeg *seg_p;
    class Key *key_p;

    enum { waiting, rolling, done } state;

    int brightness;
    int next_tick;
    int dice;
} *dice_p;


DigiDice::DigiDice(int speaker_pin, int pulse_pin, int key_pin)
{
  seg_p = new SevenSeg;
  state = rolling;
  this->speaker_pin = speaker_pin;
  this->pulse_pin = pulse_pin;
  key_p = new Key(key_pin);
  dice = 1;
  brightness = 0;
  next_tick = 0;
  this->setup();
}

void DigiDice::setup()
{
  pinMode(speaker_pin, OUTPUT);
  pinMode(pulse_pin, OUTPUT);
  digitalWrite(speaker_pin, LOW);
}


void DigiDice::refreshBrightness()
{
  // prepare next pulse parameter
  brightness--;
  if (brightness <= 0) {
    brightness = 100;
  }
}

void DigiDice::refreshFace()
{
  switch (state) {
    case done:
      seg_p->setNumber(dice);
      break;
    case waiting:
      seg_p->setChar('?');
      break;
    case rolling:
      // roll the dice, or just wait...
      next_tick--;
      if (next_tick <= 0) {
        seg_p->setNumber(dice);
        dice++;
        if (dice > 6) {
          dice = 1;
        }
        next_tick = 4;
      }
      break;
  }
}

void DigiDice::refresh()
{
  // pulse animation
  analogWrite(pulse_pin, brightness / 10);
  // audio feedback
  tone(speaker_pin, 20 * brightness);
  refreshBrightness();
  key_p->refresh();
  if (key_p->is_clicked()) {
    // state up
    switch (state) {
      case waiting:
        state = rolling;
        break;
      case rolling:
        state = done;
        break;
      case done:
        state = waiting;
        break;
    }
  }
  refreshFace();
  seg_p->refresh(); // make the dice value visible
}

// --- keep the arduino framework plating at a logical minimum

void setup() {
  dice_p = new DigiDice(12, 5, 11);
}

void loop() {
  // refresh comes at some 100 Hz
  delayMicroseconds(10000);
  dice_p->refresh();
}

