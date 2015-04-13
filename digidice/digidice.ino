#define SPEAKER 12
#define LED  10

#include "sevenseg.h"

SevenSeg *pSeg;

void setup() {
  pinMode(SPEAKER, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(SPEAKER, LOW);
  pSeg = new SevenSeg();
}

int brightness = 1;
int next_tick = 1;

void loop() {
  delayMicroseconds(10000);
  analogWrite(LED, brightness/3);
  // tone(SPEAKER, 20*brightness);
  brightness--;
  if (brightness <= 0) {
    brightness = 100;
  }
  next_tick--;
  if (next_tick <= 0) {
    next_tick = 40;
    pSeg->stepUp();
  }
  pSeg->print();
}

