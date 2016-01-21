#include "Arduino.h"
#include "Speaker.h"

// borrowed speaker driver from digidice.
//
// This implementation uses PROGMEM data to minimise the sketch footprint, effectively saving some 200 bytes.

const uint16_t Speaker::tones[] PROGMEM = {
  // re-generate speaker_tones.h with $PROJECT/generate-tones.rb
  // BUG: the frequencies are off by 5 half tones.
#include "speaker_tones.hx"
0 };

#define NULL_MELODY  ((signed const char *)0)

#define OPCODE(a)  (signed char)(-Speaker::Op##a)

// all durations are based on 100 Hz control rate.

static signed const char ops_ok_chord[] PROGMEM = {
  OPCODE(Transpose),  24,
  0,5,
  4,5,
  7,5,
  12,15,
  OPCODE(EndOfTune),-1
}; 

static signed const char ops_err_chord[] PROGMEM = {
  OPCODE(Transpose),  12,
  12,5,
  7,5,
  3,5,
  0,15,
  OPCODE(EndOfTune),-1
}; 

static signed const char ops_rolling[] PROGMEM = {
  OPCODE(Transpose),  1,
  0,5,
  4,5,
  7,5,
  12,5,
  7,5,
  3,5,
  OPCODE(JumpTo), 0,
}; 

static signed const char ops_greeter[] PROGMEM = {
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

static PROGMEM signed const char ops_cooling[] = {
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

const signed char *  const Speaker::melodies[] PROGMEM = {
  ops_ok_chord,
  ops_err_chord,
  ops_rolling,
  ops_greeter, // greeter melody
  ops_cooling,
  NULL_MELODY
};
  
void Speaker::begin(int pin, int pollfreq)
{
  _pin = pin;
  _poll_freq = pollfreq;
  _silenced = true;
  _duration = 0;
  _melody = NULL_MELODY;
  _next_note = 0;
}

void Speaker::play(TMelody tune)
{
  _duration=0;
  _melody = static_cast<const signed char *>(pgm_read_ptr_near(Speaker::melodies + tune));
  _next_note = 0;
  _key_offset = 12;
  _silenced = false;
}

void Speaker::nextNoteEvent()
{
  signed char opcode = pgm_read_byte_near( _melody + (_next_note++));
  signed char arg = pgm_read_byte_near( _melody + (_next_note++));
  if (opcode < 0) {
    if (opcode == -OpEndOfTune) {
        _silenced = true;
        _melody = NULL_MELODY;
        noTone(_pin);
    } else if (opcode == -OpTranspose) {
        _key_offset = arg;
    } else if (opcode == -OpJumpTo) {
        _next_note = (unsigned char)arg;
    }
  } else {
   uint16_t toneval = pgm_read_word_near(Speaker::tones + opcode + _key_offset);
   tone(_pin, toneval);
   _duration = ((long)arg * _poll_freq + 50L) / 100L;
  } // note opcode
}

void Speaker::iterate()
{
  if (_silenced) return;
  _duration--;
  if (_duration <= 0 && _melody != NULL_MELODY) {
    nextNoteEvent();
  }
}

void Speaker::delayLoop()
{
    if (_poll_freq >= 1000) {
        delayMicroseconds((unsigned int)(1000000L/(long)_poll_freq));
    } else {
        delay((unsigned int)(1000L/(long)_poll_freq));
    }
}
