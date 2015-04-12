#include "sevenseg.h"

const int SevenSeg::segment_pins[] = {
  2, 3, 4, 6, 7, 8, 9, 10,
  5, // decimal point
  -1
};

const int SevenSeg::step_segments[] = {
  0, 1, 2, 6, 5, 4, 3, 6,
  -1
};

SevenSeg::SevenSeg()
{
  activeseg = 0;
  current_step = 0;
  for (int i = 0; segment_pins[i] > 0; i++) {
    pinMode(SevenSeg::segment_pins[i], OUTPUT);
    digitalWrite(SevenSeg::segment_pins[i], LOW);
  }
}

void SevenSeg::set(int segment)
{
  digitalWrite(SevenSeg::segment_pins[segment], HIGH);
}

void SevenSeg::clear(int segment)
{
  digitalWrite(SevenSeg::segment_pins[segment], LOW);
}

void SevenSeg::stepUp()
{
  current_step++;
  if (SevenSeg::step_segments[current_step] < 0)
    current_step = 0;
  activeseg = SevenSeg::step_segments[current_step];
}

void SevenSeg::print()
{
  set(activeseg);
  delayMicroseconds(100);
  clear(activeseg);
}
