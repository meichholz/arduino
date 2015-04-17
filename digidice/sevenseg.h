#ifndef _H_SEVENSEG
#define _H_SEVENSEG

class SevenSeg {
  public:

    enum TCharset { CsDigits=0, CsInverseEight, CsEight, CsSmallWheel, CsDiceFacing };
    
    static const int segment_pins[];  // map from logical segments to real pins
    static const unsigned char *generators[]; // character generator for wheel animation
    SevenSeg();
    virtual ~SevenSeg() {};
    void set(int segment);
    void clear(int segment);
    void stepUp();
    void setAscii(char ch);
    void setChar(unsigned char ch, enum TCharset cs);
    void setNumber(int i);
    void iterate();

  private:
    unsigned char active_bits;
    int current_step;
    enum TCharset animation; // current progress animation
};

#endif

