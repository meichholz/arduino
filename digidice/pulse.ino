Pulse::Pulse(int pin) :
  pin(pin),
  brightness(0)
{
  pinMode(pin, OUTPUT);
}

void Pulse::iterate()
{
  brightness--;
  if (brightness <= 0) {
    brightness = 100;
  }
  analogWrite(pin, brightness / 10);
}

