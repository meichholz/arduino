#include <Button.h>

Button button(8);

bool lit=false;

void setup() {
  pinMode(13, OUTPUT);
}

void loop() {
  delay(100);
  button.iterate();
  if (button.is_clicked()) lit = !lit;
  digitalWrite(13, lit ? HIGH : LOW);
}
