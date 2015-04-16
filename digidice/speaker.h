#ifndef _H_SPEAKER
#define _H_SPEAKER
class Speaker {
  public:

  Speaker(int pin);
  void  play(int tune);
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

