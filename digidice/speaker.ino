const unsigned int Speaker::tones[] = {
#include "speaker_tones.h"
0 };

Speaker::Speaker(int pin)
{
  this->pin = pin;
  key = 0;
  duration = 0;
}

void Speaker::iterate()
{
   duration--;
   if (duration <= 0) {
     duration = 5;
      tone(pin, Speaker::tones[key]);
      key++;
      if (Speaker::tones[key]==0) key=0;
   }
}


