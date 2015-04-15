#include "key.h"

Key::Key(int pin) {
  this->pin = pin;
  debounce_ticks = 0;
  debounced_state = false;
  last_state = false;
  clicked = false;
  pinMode(pin, INPUT);
}

void Key::iterate()
{
  bool new_state = (digitalRead(pin) == LOW);
  if (new_state != last_state) {
    debounce_ticks = 5;
    last_state = new_state;
  } else {
    debounce_ticks--;
    if (debounce_ticks<=0) {
      debounced_state = new_state;
    }
  }
}

bool Key::is_pressed()
{
  return debounced_state;
}

bool Key::is_clicked()
{
  // memorize edge transition.
  if (debounced_state && !clicked) {
    clicked = true;
    return true;
  }
  if (!debounced_state && clicked) {
    clicked = false;
  }
  return false;
}

