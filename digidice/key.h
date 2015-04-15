#ifndef _H_KEY
#define _H_KEY

class Key {
  public:
    Key(int pin);
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

