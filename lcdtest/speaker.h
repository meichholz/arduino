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
    
     
  Speaker(int pin, int loopfreq);
  void  play(TMelody tune);
  void setup();
  void iterate();

  private:

  void nextNoteEvent();

  int pin;
  int duration;
  int next_note;
  int poll_freq; // normally 100 (Hz)
  bool  silenced;
  signed const char *melody;
  signed char key_offset;
  
  static const unsigned int tones[];
  static const signed char * const melodies[];
};
#endif

