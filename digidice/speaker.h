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
    
     
  Speaker(int pin);
  void  play(TMelody tune);
  void iterate();

  private:

  int pin;
  signed char duration;
  signed char key_offset;
  bool  silenced;
  signed const char *melody;
  int next_note;
  
  static const unsigned int tones[];
  static const signed char* melodies[];
};
#endif

