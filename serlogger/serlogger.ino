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
   Serial.begin(9600);
 }

Logger::Logger() {
  this->setup();
}

Logger *logger = static_cast<Logger*>(0);
const int n_led = 13;

int blink_percent = 100;

void setup() {
  logger = new Logger;
  logger->debug("setting up serial");
  logger->debug("Enter percentage for blink ratio");
  pinMode(n_led, OUTPUT);
}

int n_new_percent=0;

void loop() {
  digitalWrite(n_led, HIGH);
  delay(100+9*blink_percent);
  digitalWrite(n_led, LOW);
  delay(1000-9*blink_percent);
  if (Serial.available()) {
    char ch = Serial.read();
    if (ch>='0' && ch<='9')
      n_new_percent = 10*n_new_percent+ch-'0';
    else if (ch=='\r') {
      blink_percent = n_new_percent;
      n_new_percent = 0;
    }
    else
    {
      Serial.println("Error in input, resetting...");
      blink_percent = 1;
      n_new_percent = 0;
    }
  }
}
