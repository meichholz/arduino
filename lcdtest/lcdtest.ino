class Lcd {
  public:
    Lcd();
  private:
    void setup();
    void writeHighNibble(unsigned char nibble);
    char  m_pin_e;
    char  m_pin_rs;
    char  m_pin_d_base;
};

Lcd::Lcd() :
  m_pin_e(12),
  m_pin_rs(11),
  m_pin_d_base(2)
{
  setup();
}

void Lcd::writeHighNibble(unsigned char nibble)
{
  unsigned char mask = 1;
  digitalWrite(m_pin_rs, LOW);
  for (int i=0; i<4; i++) {
    digitalWrite(m_pin_d_base+i, (nibble & mask) ? HIGH : LOW);
    mask = mask<<1;
  }
  digitalWrite(m_pin_e, HIGH);
  delay(1);
  digitalWrite(m_pin_e, LOW);
}

void Lcd::setup()
{
  pinMode(m_pin_e, OUTPUT);
  digitalWrite(m_pin_e, LOW);
  pinMode(m_pin_rs, OUTPUT);
  digitalWrite(m_pin_rs, HIGH);
  for (int i=0; i<4; i++) {
    pinMode(m_pin_d_base+i, OUTPUT);
  }
  writeHighNibble(0x03); // master reset, 4 bit
  delay(5);
  writeHighNibble(0x03); // master reset, 4 bit
  delay(1);
  writeHighNibble(0x03); // master reset, 4 bit
  delay(1);
  writeHighNibble(0x02); // set interface to 4 bit, simplified
   
}

class App {
  public:
    App();
    void iterate();
  private:
    Lcd * m_display;
};

App::App()
{
  m_display = new Lcd;
}

void App::iterate()
{
}

class App * app_p;

void setup() {
  app_p = new App;
}

void loop() {
  app_p->iterate();
}
