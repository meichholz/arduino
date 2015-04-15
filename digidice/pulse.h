#ifndef _H_PULSE
#define _H_PULSE

class Pulse {
  public:
  Pulse(int pin);
  void iterate();
  // no destructor
  private:
  int pin;
  int brightness;
};

#endif
