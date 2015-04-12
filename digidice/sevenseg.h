#ifndef _H_SEVENSEG
#define _H_SEVENSEG

class SevenSeg {
  public:

    static const int segment_pins[];  // map from logical segments to real pins
    static const int step_segments[]; // logical segments per counter
    SevenSeg();
    virtual ~SevenSeg() {};
    void set(int segment);
    void clear(int segment);
    void stepUp();
    void print();

  private:
    int activeseg;
    int current_step;
};

#endif

