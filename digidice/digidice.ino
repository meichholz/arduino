#define SPEAKER 12
#define LED  10

void setup() {
  pinMode(SPEAKER, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(SPEAKER, LOW);
}

int brightness = 100;

void loop() {
  delayMicroseconds(5000);
  analogWrite(LED, brightness);
  tone(SPEAKER, 20*brightness);
  brightness--;
  if (brightness<=0) brightness=100;
}
