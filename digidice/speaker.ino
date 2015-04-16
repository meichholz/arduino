const unsigned int Speaker::tones[] = {
  // re-generate speaker_tones.h with $PROJECT/generate-tones.rb
  // BUG: the frequencies are off by 5 half tones.
#include "speaker_tones.h"
0 };

#define NULL_MELODY  ((signed const char *)0)

#define OPCODE(a)  (signed char)(-Speaker::a)

static signed const char ops_ok_chord[] = {
  OPCODE(transpose),  24,
  0,5,
  4,5,
  7,5,
  12,15,
  OPCODE(end_of_tune),-1
}; 

static signed const char ops_err_chord[] = {
  OPCODE(transpose),  12,
  12,5,
  7,5,
  3,5,
  0,15,
  OPCODE(end_of_tune),-1
}; 

static signed const char ops_rolling[] = {
  OPCODE(transpose),  24,
  0,5,
  4,5,
  7,5,
  12,5,
  7,5,
  4,5,
  OPCODE(jump_to), 0,
}; 

const signed char * Speaker::melodies[] = {
  ops_ok_chord,
  ops_err_chord,
  ops_rolling,
  ops_err_chord, // greeter
  NULL_MELODY
};
  

Speaker::Speaker(int pin)
{
  this->pin = pin;
  duration = 0;
  melody = NULL_MELODY;
  next_note = 0;
  silenced = true;
}

void Speaker::play(melody_id tune)
{
  silenced = true;
  duration=0;
  melody = Speaker::melodies[tune];
  next_note = 0;
  key_offset = 12;
  silenced = false;
}

void Speaker::iterate()
{
  if (silenced) return;
   duration--;
   if (duration <= 0 && melody!=NULL_MELODY) {
     signed char opcode = melody[next_note++];
     signed char arg = melody[next_note++];
     if (opcode < 0) {
       if (opcode == -end_of_tune) {
           silenced = true;
           melody = NULL_MELODY;
           noTone(pin);
       } else if (opcode == -transpose) {
           key_offset = arg;
       } else if (opcode == -jump_to) {
           next_note = (unsigned char)arg;
       }
     } else {
      tone(pin, Speaker::tones[opcode + key_offset]);
      duration = arg;
     } // note opcode
   }
}


