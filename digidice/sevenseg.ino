#include "sevenseg.h"

const int SevenSeg::segment_pins[] = {
  2, 3, 4, 6, 7, 8, 9,
  5, // decimal point
  -1
};

static unsigned char cgen_digits[] = {
  B00000001,
  B00000011,
  B00000111,
  B00001111,
  B00011111,
  B00111111,
  B01111111,
  0xFF,
};

static unsigned char cgen_anim_inverse_eight[] = {
  B01111110,
  B01111101,
  B01111011,
  B00111111,
  B01011111,
  B01101111,
  B01110111,
  B00111111,
  0xFF
};

static unsigned char cgen_anim_eight[] = {
  B00000001,
  B00000010,
  B00000100,
  B01000000,
  B00100000,
  B00010000,
  B00001000,
  B01000000,
  0xFF
};

static unsigned char cgen_anim_small_wheel[] = {
  B01000011,
  B01000101,
  B01000110,
  B00000111,
  0xFF
};

const unsigned char *SevenSeg::generators[] = {
    cgen_digits,
    cgen_anim_inverse_eight,
    cgen_anim_eight,
    cgen_anim_small_wheel,
    (unsigned char *)0
};

SevenSeg::SevenSeg()
{
  active_bits = 0;
  current_step = 0;
  animation = small_wheel;
  animation = digits;
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
  const unsigned char *generator = SevenSeg::generators[animation];
  if (generator[current_step] & 0x80)
    current_step = 0;
  active_bits = generator[current_step];
}

void SevenSeg::print()
{
  for (int segment = 0; segment < 8; segment++) {
    unsigned char mask = 1<<segment;
    if (active_bits & mask) {
      set(segment);
    }
  }
  delayMicroseconds(500);
  for (int segment = 0; segment < 8; segment++) {
    clear(segment);
  }
}
