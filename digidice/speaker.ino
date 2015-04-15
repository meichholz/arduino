Speaker::Speaker(int pin)
{
  this->pin = pin;
  freq = 110;
  i = 1;
}

void Speaker::iterate()
{
    tone(pin, freq*i);
    i++;
    if (i>=100) i=1;
}


