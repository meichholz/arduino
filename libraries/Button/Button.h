#ifndef _H_BUTTON
#define _H_BUTTON

class Button {
  public:
    Button(int pin);
    // no destructor
    void iterate();
    bool is_pressed();
    bool is_clicked();
    
  private:
    bool debounced_state, last_state, clicked;
    int  debounce_ticks;
    int pin;
};

#endif

