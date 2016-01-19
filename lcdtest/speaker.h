#ifndef _H_SPEAKER
#define _H_SPEAKER

class Speaker {
  public:

  enum Opcode {
    OpFirstNote,
    OpEndOfTune,
    OpTranspose,
    OpJumpTo,
  };
  
  enum TMelody {
    MelodyChordOk,
    MelodyChordError,
    MelodyRolling,
    MelodyGreeter,
    MelodyCooling,
  };
  
  void  play(TMelody tune);
  void begin(int pin, int pollfreq);
  void iterate();

  private:

  void nextNoteEvent();

  int _pin;
  int _poll_freq; // normally 100 (Hz)
  int _duration;
  int _next_note;
  bool _silenced;
  signed const char *_melody;
  signed char _key_offset;
  
  static const unsigned int tones[];
  static const signed char * const melodies[];
};
#endif

