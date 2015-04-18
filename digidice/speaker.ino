const unsigned int Speaker::tones[] = {
  // re-generate speaker_tones.h with $PROJECT/generate-tones.rb
  // BUG: the frequencies are off by 5 half tones.
#include "speaker_tones.h"
0 };

#define NULL_MELODY  ((signed const char *)0)

#define OPCODE(a)  (signed char)(-Speaker::Op##a)

static signed const char ops_ok_chord[] = {
  OPCODE(Transpose),  24,
  0,5,
  4,5,
  7,5,
  12,15,
  OPCODE(EndOfTune),-1
}; 

static signed const char ops_err_chord[] = {
  OPCODE(Transpose),  12,
  12,5,
  7,5,
  3,5,
  0,15,
  OPCODE(EndOfTune),-1
}; 

static signed const char ops_rolling[] = {
  OPCODE(Transpose),  1,
  0,5,
  4,5,
  7,5,
  12,5,
  7,5,
  3,5,
  OPCODE(JumpTo), 0,
}; 

static signed const char ops_greeter[] = {
  OPCODE(Transpose),  48,
  0,15,
  4,15,
  7,15,
  12,15,
  7,15,
  4,15,
  0,15,
  OPCODE(EndOfTune),-1
};

static signed const char ops_cooling[] = {
  OPCODE(Transpose),  48,
  12, 35,
  7, 40,
  4, 45,
  OPCODE(Transpose),  36,
  12, 50,
  7, 60,
  4, 80,
  OPCODE(Transpose),  24,
  12, 120,
  OPCODE(EndOfTune),-1
};

const signed char * Speaker::melodies[] = {
  ops_ok_chord,
  ops_err_chord,
  ops_rolling,
  ops_greeter, // greeter melody
  ops_cooling,
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

void Speaker::play(TMelody tune)
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
       if (opcode == -OpEndOfTune) {
           silenced = true;
           melody = NULL_MELODY;
           noTone(pin);
       } else if (opcode == -OpTranspose) {
           key_offset = arg;
       } else if (opcode == -OpJumpTo) {
           next_note = (unsigned char)arg;
       }
     } else {
      tone(pin, Speaker::tones[opcode + key_offset]);
      duration = arg;
     } // note opcode
   }
}


