#define SPEAKER 12
#define LED  10

class SevenSeg {
  public:
  
  static const int segment_pins[];
  SevenSeg();
  virtual ~SevenSeg() {};
  void set(int segment);
  void clear(int segment);
  void stepUp();
  void print();
  
  private:
  int activeseg;
};

const int SevenSeg::segment_pins[] = {
  2,3,4,6,7,8,9,10,
  0 };

SevenSeg::SevenSeg()  
{
  activeseg = 0;
  for (int i=0; segment_pins[i]>0; i++) {
    pinMode(SevenSeg::segment_pins[i], OUTPUT);
    digitalWrite(SevenSeg::segment_pins[i], LOW);
  }
}

void SevenSeg::set(int segment)
{
  digitalWrite(SevenSeg::segment_pins[segment], HIGH);
}

void SevenSeg::clear(int segment)
{
  digitalWrite(SevenSeg::segment_pins[segment], LOW);
}

void SevenSeg::stepUp()
{
    activeseg++;
    if (activeseg==7) activeseg=0;
}

void SevenSeg::print()
{
  set(activeseg);
  delayMicroseconds(100);
  clear(activeseg);   
}

SevenSeg *pSeg;

void setup() {
  pinMode(SPEAKER, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(SPEAKER, LOW);
  pSeg = new SevenSeg();
}

int brightness = 100;

void loop() {
  delayMicroseconds(10000);
  analogWrite(LED, brightness);
  // tone(SPEAKER, 20*brightness);
  brightness--;
  if (brightness<=0) {
    brightness=100;
    pSeg->stepUp();
  }
  pSeg->print();
}

