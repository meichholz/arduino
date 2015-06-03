#include "logger.h"
#include "cli.h"

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

