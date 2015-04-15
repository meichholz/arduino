#ifndef _H_SPEAKER
#define _H_SPEAKER
class Speaker {
  public:
  Speaker(int pin);
  void iterate();
  
  // no destructor
  private:
  int pin;
  int freq;
  int duration;
  int key;
  
  static const unsigned int tones[];
};
#endif

