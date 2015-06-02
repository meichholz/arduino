 class Logger {
   public:
   void setup();
   void debug(String message);
   Logger();
   virtual ~Logger() {}; 
 };
 
 void Logger::debug(String  message) {
   Serial.print("DEBUG:");
   Serial.println(message);
 }
 
 void Logger::setup() {
 }

Logger::Logger() {
  this->setup();
}

class Cli {
  private:
    char m_inputbuffer[100];
    bool m_ready;
    int  m_nextkey_i;
    class Logger *m_logger_p;

  public:

  Cli();
  void setLogger(class Logger *logger);
  int getNumber();
  void setup();
  void refresh();
  bool is_available() { return m_ready; }
  virtual ~Cli() {};
  
};

void Cli::setLogger(class Logger *logger)
{
  m_logger_p = logger;
}

Cli::Cli()
{
  m_ready = false;
  m_nextkey_i = 0;
  m_inputbuffer[0] = '\0';
}

void Cli::setup()
{
  m_logger_p->debug("setting up serial");
  m_logger_p->debug("Enter percentage for blink ratio");
}

void Cli::refresh() {
  bool modified = false;
  while (Serial.available()) {
    char ch = Serial.read();
    modified = true;
    if (ch == '\r') {
      m_ready = true;
    } else if (m_nextkey_i < sizeof(m_inputbuffer)-1) {
      m_inputbuffer[m_nextkey_i++] = ch;
      m_inputbuffer[m_nextkey_i] = '\0';
    }
  }
  if (modified) {
    m_logger_p->debug("Got so far:");
    m_logger_p->debug(m_inputbuffer);
  }
}

int Cli::getNumber()
{
  int i = 0;
  int n = 0;
  while (i<sizeof(m_inputbuffer)-1) {
    int ch = m_inputbuffer[i];
    if (ch>='0' && ch<='9') {
       n = 10*n + ch-'0';
       i++;
    }
    else
      break;
  }
  m_nextkey_i = 0;
  m_ready = false;
  if (m_inputbuffer[i]=='\0') {
    char buffer[50];
    sprintf(buffer,"New INT value: %d",n);
    Serial.println(buffer);
    return n;
  }
  return -1;
}

Logger *logger = static_cast<Logger*>(0);
Cli *cli = static_cast<Cli*>(0);

int blink_percent = 100;
const int n_led = 13;

void setup() {
  Serial.begin(9600);
  logger = new Logger;
  cli = new Cli; // this setup is CRAP
  cli->setLogger(logger);
  cli->setup();
  pinMode(n_led, OUTPUT);
}

void loop() {
  cli->refresh();
  if (blink_percent >0 && blink_percent<=100) {
    digitalWrite(n_led, HIGH);
    delay(1+9*blink_percent);
    digitalWrite(n_led, LOW);
    delay(1+1000-9*blink_percent);
  }
  if (cli->is_available()) {
    blink_percent = cli->getNumber();
    if (blink_percent<0) {
      Serial.println("Error in input, resetting...");
      blink_percent = 100;
    }
  }
}

