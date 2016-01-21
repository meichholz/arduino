// barebone minimal example code for Speaker
// (C) Marian Eichholz 2015

#include "Speaker.h"

#define PIN_SPEAKER 8
#define POLL_FREQUENCY  100

Speaker speaker;

void setup()
{
  speaker.begin(PIN_SPEAKER, POLL_FREQUENCY);
  speaker.play(Speaker::MelodyGreeter); // setup sequencer for a greeting melody
}

void loop() 
{
  speaker.delayLoop(); // pause for a time interval derived from POLL_FREQUENCY
  speaker.iterate();   // play next note or whatever
}


