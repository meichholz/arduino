#include "Arduino.h"
#include "Button.h"

Button::Button(int pin) :
  pin(pin),
  debounce_ticks(0),
  clicked(false),
  last_state(false)
{
  pinMode(pin, INPUT);
}

void Button::iterate()
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

bool Button::is_pressed()
{
  return debounced_state;
}

bool Button::is_clicked()
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

