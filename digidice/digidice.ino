#include "sevenseg.h"
#include "key.h"
#include "speaker.h"
#include "pulse.h"

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

    class SevenSeg *seg_p;
    class Key *key_p;
    class Speaker *speaker_p;
    class Pulse *pulse_p;

    enum State { StateRolling, StateDone };
    enum State state;

    int next_tick;
    int face;
} *dice_p;


DigiDice::DigiDice(int speaker_pin, int pulse_pin, int key_pin)
{
  seg_p = new SevenSeg;
  key_p = new Key(key_pin);
  speaker_p = new Speaker(speaker_pin);
  pulse_p = new Pulse(pulse_pin);
  state = StateRolling;  
  face = 1;
  next_tick = 0;
  speaker_p->play(Speaker::MelodyGreeter);
}

void DigiDice::refreshFace()
{
  switch (state) {
    case StateDone:
        seg_p->setChar(face, SevenSeg::CsDiceFacing);
      break;
    case StateRolling:
      // roll the dice, or just wait...
      next_tick--;
      if (next_tick <= 0) {
        seg_p->setChar(face, SevenSeg::CsDiceFacing);
        if (++face > 6) {
          face = 1;
        }
        next_tick = TICKS_FOR_ROLLING;
      }
      break;
  }
}

void DigiDice::advanceState()
{
    switch (state) {
      case StateRolling:
        state = StateDone;
        speaker_p->play(Speaker::MelodyChordOk);
        break;
      case StateDone:
        state = StateRolling;
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

