void setup() {
  pinMode(13, OUTPUT);
}

bool b;

int del=1;
void loop() {
  digitalWrite(13, b ? HIGH : LOW);
  // delay(del);
  b=!b;
}
