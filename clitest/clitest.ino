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

// end of class Logger =============================================================

/** Thoughts about a CLI:

It consists of a Commandline, a Parser for the specific language and uses Tokens.
The Commandline should not know about the language per se.
*/
class Cli {
  public:

  Cli(class Logger *logger);
  int getNumber();
  void setup();
  void refresh();
  bool is_available();
  virtual ~Cli() {};

  private:
    char m_inputbuffer[100];
    bool m_ready;
    int  m_nextkey_i;
    class Logger *m_logger;
};

Cli::Cli(class Logger *logger) :
  m_logger(logger),
  m_ready(false)
{
  m_inputbuffer[0] = '\0';
  this->setup();
}

bool Cli::is_available() { return m_ready; }

void Cli::setup()
{
  Serial.begin(9600);
  Serial.println("Enter percentage for blink ratio");
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
    m_logger->debug("Got so far:");
    m_logger->debug(m_inputbuffer);
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

// end of class Cli =============================================================

Logger *logger = static_cast<Logger*>(0);
Cli *cli = static_cast<Cli*>(0);

int blink_percent = 100;
const int n_led = 13;

void setup() {
  logger = new Logger;
  cli = new Cli(logger); // this setup is CRAP
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

