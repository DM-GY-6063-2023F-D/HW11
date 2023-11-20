#include "Clicky.h";

int STATE[] = { 0, 1, 2, 3, 4, 5 };
int BADSTATE[] = { -1, 6, 7, 8, 9, 10 };

int B0_PIN = 2;
int B1_PIN = 3;
int G_PIN = 7;
int R_PIN = 6;
int POT_PIN = A0;
int LDR_PIN = A1;

int TIMEOUT = 10000;

int cState;

int timerTimeout;

Clicky mButton0;
Clicky mButton1;

void setup() {
  Serial.begin(9600);
  cState = STATE[0];
  timerTimeout = millis() + TIMEOUT;

  mButton0.setup(B0_PIN);
  mButton1.setup(B1_PIN);

  pinMode(G_PIN, OUTPUT);
  pinMode(R_PIN, OUTPUT);
}

void loop() {
  // output logic
  if (cState == STATE[5]) {
    digitalWrite(G_PIN, HIGH);
  } else if (cState == BADSTATE[5]) {
    digitalWrite(R_PIN, HIGH);
  } else {
    digitalWrite(G_PIN, LOW);
    digitalWrite(R_PIN, LOW);
  }

  // timeout logic
  if (millis() > timerTimeout) {
    if (cState == STATE[5] || cState == BADSTATE[5]) {
      cState = STATE[0];
      timerTimeout = millis() + TIMEOUT;
    } else if (cState != STATE[0]) {
      cState = BADSTATE[5];
      timerTimeout = millis() + (TIMEOUT / 3);
    } else {
      timerTimeout = millis() + TIMEOUT;
    }
    return;
  }

  // read inputs
  int b0 = mButton0.wasClicked();
  int b1 = mButton1.wasClicked();
  int potV = analogRead(POT_PIN);
  int ldrV = analogRead(LDR_PIN);
  Serial.println(cState);

  // threshold and pre-process analog values
  bool potHigh = potV > 4000;
  bool potLow = potV < 1000;
  bool potMid = (potV > 1000) && (potV < 4000);
  bool ldrHigh = ldrV > 600;

  // state transition logic
  if (cState == STATE[0]) {
    if (b1 && potHigh) {
      cState = STATE[1];
      timerTimeout = millis() + TIMEOUT;
    } else if (b0 || (b1 && !potHigh)) {
      cState = BADSTATE[1];
      timerTimeout = millis() + TIMEOUT;
    }
  }

  else if (cState == STATE[1]) {
    if (b0 && potLow) {
      cState = STATE[2];
      timerTimeout = millis() + TIMEOUT;
    } else if (b1 || (b0 && !potLow)) {
      cState = BADSTATE[2];
      timerTimeout = millis() + TIMEOUT;
    }
  }

  else if (cState == STATE[2]) {
    if (b1 && potMid) {
      cState = STATE[3];
      timerTimeout = millis() + TIMEOUT;
    } else if (b0 || (b1 && !potMid)) {
      cState = BADSTATE[3];
      timerTimeout = millis() + TIMEOUT;
    }
  }

  else if (cState == STATE[3]) {
    if (b0 && potMid) {
      cState = STATE[4];
      timerTimeout = millis() + TIMEOUT;
    } else if (b1 || (b0 && !potMid)) {
      cState = BADSTATE[4];
      timerTimeout = millis() + TIMEOUT;
    }
  }

  else if (cState == STATE[4]) {
    if (ldrHigh) {
      cState = STATE[5];
      timerTimeout = millis() + TIMEOUT;
    } else if (b0 || b1) {
      cState = BADSTATE[5];
      timerTimeout = millis() + (TIMEOUT / 3);
    }
  }

  else if (cState == STATE[5] || cState == BADSTATE[5]) {
    // transition only on timeout
  }

  else {
    if (b0 || b1) {
      cState = cState + 1;
      if (cState == BADSTATE[5]) {
        timerTimeout = millis() + (TIMEOUT / 3);
      } else {
        timerTimeout = millis() + TIMEOUT;
      }
    }
  }

  delay(10);
}
