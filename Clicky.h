class Clicky {
private:
  int STATE_0 = 0;
  int STATE_1 = 1;
  int cState = STATE_0;
  int pin;

public:
  Clicky() {}

  void setup(int _pin) {
    pin = _pin;
    pinMode(pin, INPUT);
  }

  bool wasClicked() {
    int v = digitalRead(pin);

    bool clicked = false;

    if (cState == STATE_0 && v) {
      cState = STATE_1;
      clicked = true;
    } else if (cState == STATE_1 && !v) {
      cState = STATE_0;
    }

    return clicked;
  }

  bool isPressed() {
    return cState == STATE_1;
  }
};
