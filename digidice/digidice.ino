#include "sevenseg.h"

class DigiDice {

  public:
    DigiDice(int speaker_pin, int pulse_pin, int key_pin);
    void setup();
    void refresh();

    // no destructor

  private:

    void adjustBrightness();
    void adjustFace();
    bool pollKey();

    int speaker_pin, pulse_pin, key_pin;

    SevenSeg *seg_p;

    enum { waiting, rolling, done } state;

    int brightness;
    int next_tick;
    int dice;
    bool key_pressed;
    bool debounced_state, last_state;
    int  debounce_ticks;
} *dice_p;


DigiDice::DigiDice(int speaker_pin, int pulse_pin, int key_pin)
{
  seg_p = new SevenSeg;
  state = waiting;
  this->speaker_pin = speaker_pin;
  this->pulse_pin = pulse_pin;
  this->key_pin = key_pin;
  dice = 1;
  brightness = 0;
  next_tick = 0;
  debounce_ticks = 0;
  debounced_state = false;
  last_state = false;
  key_pressed = false;
  this->setup();
}

void DigiDice::setup()
{
  pinMode(speaker_pin, OUTPUT);
  pinMode(pulse_pin, OUTPUT);
  pinMode(key_pin, INPUT);
  digitalWrite(speaker_pin, LOW);
}

bool DigiDice::pollKey()
{
  bool new_state = (digitalRead(key_pin) == LOW);
  if (new_state != last_state) {
    debounce_ticks = 5;
    last_state = new_state;
  } else {
    debounce_ticks--;
    if (debounce_ticks<=0) {
      debounced_state = new_state;
    }
  }
  return debounced_state;
}

void DigiDice::adjustBrightness()
{
  // prepare next pulse parameter
  brightness--;
  if (brightness <= 0) {
    brightness = 100;
  }
}

void DigiDice::adjustFace()
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
        next_tick = 40;
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
  adjustBrightness();
  bool pressed = pollKey();
  if (pressed) {
    if (!key_pressed) { // act just on edge
      // acklowledge edge
      key_pressed = true;
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
  }
  else
  {
    key_pressed = false; // acklowledge release
  }
  adjustFace();
  seg_p->refresh(); // make the dice value visible
}

// keep the arduino framework plating at a logical minimum

void setup() {
  dice_p = new DigiDice(12, 5, 11);
}

void loop() {
  // refresh comes at some 100 Hz
  delayMicroseconds(10000);
  dice_p->refresh();
}

