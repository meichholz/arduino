const unsigned int Speaker::tones[] = {
  // re-generate speaker_tones.h with $PROJECT/generate-tones.rb
  // BUG: the frequencies are off by 5 half tones.
#include "speaker_tones.h"
0 };

#define NULL_MELODY  ((signed const char *)0)

#define SPK_OP_END  ((signed char)-1)
#define SPK_OP_TRANSPOSE  ((signed char)-2)
#define SPK_OP_GOTO  ((signed char)-3)

static signed const char ok_chord[] = {
  SPK_OP_TRANSPOSE,  24,
  0,5,
  4,5,
  7,5,
  12,15,
  SPK_OP_END,-1
}; 

static signed const char err_chord[] = {
  SPK_OP_TRANSPOSE,  12,
  12,5,
  7,5,
  3,5,
  0,15,
  SPK_OP_END,-1
}; 

static signed const char rolling_tune[] = {
  SPK_OP_TRANSPOSE,  24,
  0,5,
  4,5,
  7,5,
  12,5,
  7,5,
  4,5,
  SPK_OP_GOTO, 0,
}; 

const signed char * Speaker::melodies[] = {
  ok_chord,
  err_chord,
  rolling_tune,
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

void Speaker::play(int melody_id)
{
  silenced = true;
  duration=0;
  melody = Speaker::melodies[melody_id];
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
       if (opcode == SPK_OP_END) {
           silenced = true;
           melody = NULL_MELODY;
           noTone(pin);
       } else if (opcode == SPK_OP_TRANSPOSE) {
           key_offset = arg;
       } else if (opcode == SPK_OP_GOTO) {
           next_note = (unsigned char)arg;
       }
     } else {
      tone(pin, Speaker::tones[opcode + key_offset]);
      duration = arg;
     } // note opcode
   }
}


