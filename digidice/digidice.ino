#include "sevenseg.h"
#include "key.h"
#include "speaker.h"
#include "pulse.h"

/***
= Summary

Digital dice with some bells and whistles.
- roll out animation
- pulsing power LED
- chords on speaker for signaling
- roll start button

=Pinout

- D12 : speaker
- D11 : input button (LOW-active, pullup required)
- D2 : dice a(1)
- D3, D4 : dice b,c (2)
- D5 : Power LED
- D6, D7 : dice d,e (4)
- D8, D9 : dice f,g (6)

**/
#define TICKS_FOR_ROLLING  1

class DigiDice {

  public:
    DigiDice(int speaker_pin, int pulse_pin, int key_pin);
    void iterate();

    // no destructor

  private:

    void refreshFace();
    bool pollKey();
    void advanceState();
    void rollDice();

    SevenSeg *seg_p;
    Key *key_p;
    Speaker *speaker_p;
    Pulse *pulse_p;

    enum State { StateRolling, StateCooling, StateDone } state;

    int next_tick;
    int face;
    int cooling_ticks;
} *dice_p;


DigiDice::DigiDice(int speaker_pin, int pulse_pin, int key_pin)
{
  seg_p = new SevenSeg;
  key_p = new Key(key_pin);
  speaker_p = new Speaker(speaker_pin);
  pulse_p = new Pulse(pulse_pin);
  state = StateCooling;
  cooling_ticks = 0;
  face = 1;
  next_tick = 0;
  speaker_p->play(Speaker::MelodyGreeter);
}

void DigiDice::rollDice()
{
  if (++face > 6) {
     face = 1;
  }
  seg_p->setChar(face, SevenSeg::CsDiceFacing);
}

void DigiDice::refreshFace()
{
  switch (state) {
    case StateDone:
        seg_p->setChar(face, SevenSeg::CsDiceFacing);
      break;
    case StateRolling:
      if (--next_tick <= 0) {
        rollDice();
        next_tick = TICKS_FOR_ROLLING;
      }
      break;
    case StateCooling: // same, but with slow down
      if (--next_tick <= 0) {
        rollDice();
        next_tick = cooling_ticks;
        cooling_ticks += 15;
        if (cooling_ticks >= 100) {
          advanceState();
        }
      }
      break;
  }
}

void DigiDice::advanceState()
{
    switch (state) {
      case StateRolling:
        state = StateCooling;
        cooling_ticks = 0;
        speaker_p->play(Speaker::MelodyCooling);
        break;
      case StateCooling:
        state = StateDone;
        break;
      case StateDone:
        state = StateRolling;
        next_tick = 0;
        speaker_p->play(Speaker::MelodyRolling);
        break;
    }
}

void DigiDice::iterate()
{
  speaker_p->iterate();
  pulse_p->iterate();
  key_p->iterate();
  if (key_p->is_clicked()) {
    speaker_p->play(Speaker::MelodyChordOk); // may be overruled by the state advancer
    advanceState();
  }
  refreshFace();
  seg_p->iterate(); // make the dice value visible
}

// --- keep the arduino framework plating at a logical minimum

void setup() {
  dice_p = new DigiDice(12, 5, 11);
}

void loop() {
  // refresh comes at some 100 Hz
  delayMicroseconds(10000);
  dice_p->iterate();
}

