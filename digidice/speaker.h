#ifndef _H_SPEAKER
#define _H_SPEAKER
class Speaker {
  public:

  enum opcode {
    end_of_tune = 1, transpose, jump_to
  };
  
  enum melody_id {
    ok_chord=0,
    er_chord=1,
    rolling=2,
    greeter=3,
  };
    
     
  Speaker(int pin);
  void  play(melody_id tune);
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

