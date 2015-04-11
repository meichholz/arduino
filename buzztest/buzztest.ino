const int buzzer = 12;

void setup() {
  pinMode(buzzer, OUTPUT);
}

int del=1;
int prewait=1;

void loop() {
  digitalWrite(buzzer, HIGH);
  delay(del);
  digitalWrite(buzzer, LOW);
  delay(del);
  prewait--;
  if (prewait==0) {
    prewait=100;
    del+=1;
    if (del==10) del=1;
  }
}
