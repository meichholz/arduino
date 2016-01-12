// home made bitbang LCD 1602 interface, 4bit mode
// just for fun, from the spec 2015
// 
// Arduino -> Shield
// D2..D5 -> D4..D7
// D11 -> E
// D12 -> RS
// GND -> RW, K, VSS
// 5V -> A, VDD
// Poti -> V0


class Lcd {
  public:
    Lcd(char pin_d4, char pin_e, char pin_rs);
    void print(const char *pch);
  private:
    void setup();
    void writeNibble(unsigned char nibble);
    void writeByte(unsigned char byte_ch);
    char  m_pin_e;
    char  m_pin_rs;
    char  m_pin_d_base;
    
};

Lcd::Lcd(char pin_d4, char pin_e, char pin_rs) :
  m_pin_e(pin_e),
  m_pin_rs(pin_rs),
  m_pin_d_base(pin_d4)
{
  setup();
}

void Lcd::writeNibble(unsigned char nibble)
{
  for (int i=0; i<4; i++) {
    digitalWrite(m_pin_d_base+i, (nibble & (8>>i)) ? HIGH : LOW);
  }
  digitalWrite(m_pin_e, HIGH);
  delayMicroseconds(2);
  digitalWrite(m_pin_e, LOW);
}

void Lcd::writeByte(unsigned char byte_ch)
{
  writeNibble((byte_ch>>4) & 0x0F);
  writeNibble(byte_ch & 0x0F);
}

void Lcd::print(const char *pch)
{
  digitalWrite(m_pin_rs, HIGH);
  while (*pch) {
    writeByte(*pch);
    pch++;
  }
  digitalWrite(m_pin_rs, LOW);
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
  // reliable initialization procedure for 4 bit
  digitalWrite(m_pin_rs, LOW);  
  delayMicroseconds(15000); // wait for power up
  writeNibble(0x03); // be 8 bit
  delayMicroseconds(4100);
  writeNibble(0x03); // again,be 8 bit (if come from 4 bit mode):-)
  delayMicroseconds(100);
  writeNibble(0x03); // last time, be 8 bit
  delay(1);
  writeNibble(0x02); // now: be 4 bit (and ignore rest, like above)
  writeByte(0x2C); // ok, final fix: 4 bit (0x20), 2 lines (0x08) and font 5x10 (0x04)
  writeByte(0x0C); // display (0x04): on (0x04), cursor (0x02): off, blink (0x01): off
  writeByte(0x01); // clear display
  delay(3);
  writeByte(0x02); // home
  delay(3);
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
  m_display = new Lcd(2, 11, 12);
  m_display->print("Booyiaey!");
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
