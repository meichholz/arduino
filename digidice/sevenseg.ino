#include "sevenseg.h"

const int SevenSeg::segment_pins[] = {
  2, 3, 4, 6, 7, 8, 9,
  5, // decimal point
  -1
};

const int SevenSeg::step_segments[] = {
  0, 1, 2, 6, 5, 4, 3, 6,
  -1
};

#define SSSP_WHEEL
const unsigned char SevenSeg::step_pattern[] = {
#ifdef SSSP_INVERSE_EIGHT
  B01111110,
  B01111101,
  B01111011,
  B00111111,
  B01011111,
  B01101111,
  B01110111,
  B00111111,
#endif
#ifdef SSSP_EIGHT
  B00000001,
  B00000010,
  B00000100,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B01000000,
 #endif
 #ifdef SSSP_WHEEL
  B01000011,
  B01000101,
  B01000110,
  B00000111,
 #endif
  0xFF,
};

SevenSeg::SevenSeg()
{
  active_bits = 0;
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
  if (SevenSeg::step_pattern[current_step] & 0x80)
    current_step = 0;
  active_bits = SevenSeg::step_pattern[current_step];
}

void SevenSeg::print()
{
  for (int segment = 0; segment < 8; segment++) {
    int pin = SevenSeg::segment_pins[segment];
    unsigned char mask = 1<<segment;
    digitalWrite(pin, (active_bits & mask) ? HIGH : LOW);
    delayMicroseconds(20);
    digitalWrite(pin, LOW);
  }
}
