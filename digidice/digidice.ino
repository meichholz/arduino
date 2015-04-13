#define SPEAKER 12
#define HEART 5
#define KEY 12

#include "sevenseg.h"

class DigiDice {
  
  public:
    DigiDice();
    void print();
    
    // no destructor
    
  private:
  
  SevenSeg *seg_p;

  enum { waiting, rolling, valid } state;

  int brightness;
  int next_tick;
  int dice;
} *dice_p;

DigiDice::DigiDice()
{
  seg_p = new SevenSeg;
  state = waiting;
  dice = 1;
  brightness = 0;
  next_tick = 0;
}

void DigiDice::print()
{
  analogWrite(HEART, brightness/3);
  // tone(SPEAKER, 20*brightness);
  brightness--;
  if (brightness <= 0) {
    brightness = 100;
  }
  next_tick--;
  if (next_tick <= 0) {
    seg_p->setNumber(dice);
    dice++;
    if (dice>6) {
      dice=1;
    }
    next_tick = 40;
  }
  seg_p->print();
}

void setup() {
  pinMode(SPEAKER, OUTPUT);
  pinMode(HEART, OUTPUT);
  pinMode(KEY, INPUT);
  digitalWrite(SPEAKER, LOW);
  dice_p = new DigiDice;
}


void loop() {
  // refresh comes at some 100 Hz
  delayMicroseconds(10000);
  dice_p->print();
}

