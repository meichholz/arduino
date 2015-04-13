#ifndef _H_SEVENSEG
#define _H_SEVENSEG

class SevenSeg {
  public:

    static const int segment_pins[];  // map from logical segments to real pins
    static const unsigned char *generators[]; // character generator for wheel animation
    SevenSeg();
    virtual ~SevenSeg() {};
    void set(int segment);
    void clear(int segment);
    void stepUp();
    void print();

  private:
    unsigned char active_bits;
    int current_step;
    enum { digits=0, inverse_eight, eight, small_wheel } animation; // current progress animation
};

#endif

