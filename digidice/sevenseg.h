#ifndef _H_SEVENSEG
#define _H_SEVENSEG

class SevenSeg {
  public:

    static const int segment_pins[];  // map from logical segments to real pins
    static const int step_segments[]; // logical segments per counter
    static const unsigned char step_pattern[]; // character generator for wheel animation
    SevenSeg();
    virtual ~SevenSeg() {};
    void set(int segment);
    void clear(int segment);
    void stepUp();
    void print();

  private:
    unsigned char active_bits;
    int current_step;
};

#endif

