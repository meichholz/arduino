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
    char inputbuffer[100];
    bool ready;
    int nextkey_i;

  public:

  Cli();
  int getNumber();
  void refresh();
  bool is_available() { return ready; }
  virtual ~Cli() {};
  
};

Cli::Cli()
{
  ready = false;
  nextkey_i = 0;
  inputbuffer[0] = '\0';
}

void Cli::refresh() {
  bool modified = false;
  while (Serial.available()) {
    char ch = Serial.read();
    modified = true;
    if (ch=='\r') {
      ready = true;
    } else if (nextkey_i < sizeof(inputbuffer)-1) {
      inputbuffer[nextkey_i++] = ch;
      inputbuffer[nextkey_i] = '\0';
    }
  }
  if (modified) {
    Serial.print("Got so far:");
    Serial.println(inputbuffer);
  }
}

int Cli::getNumber()
{
  int i = 0;
  int n = 0;
  while (i<sizeof(inputbuffer)-1) {
    int ch = inputbuffer[i];
    if (ch>='0' && ch<='9') {
       n = 10*n + ch-'0';
       i++;
    }
    else
      break;
  }
  nextkey_i = 0;
  ready = false;
  if (inputbuffer[i]=='\0') {
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
  cli = new Cli;
  logger->debug("setting up serial");
  logger->debug("Enter percentage for blink ratio");
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

