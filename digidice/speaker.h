#ifndef _H_SPEAKER
#define _H_SPEAKER
class Speaker {
  public:

  enum Opcode {
    OpEndOfTune = 1,
    OpTranspose,
    OpJumpTo
  };
  
  enum TMelody {
    MelodyChordOk=0,
    MelodyChordError=1,
    MelodyRolling=2,
    MelodyGreeter=3,
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

