#ifndef BUTTONS_H
#define BUTTONS_H

#include <Arduino.h>
#include "config.h"

/* =========================================
   INTERNAL STRUCTURE
   =========================================*/

struct Button {
  uint8_t pin;
  bool lastState;
  uint32_t lastChangeTime;
};

/* Create button objects */
static Button btn1 = {BTN1, HIGH, 0};  // Cal / Run
static Button btn2 = {BTN2, HIGH, 0};  // Stop
static Button btn3 = {BTN3, HIGH, 0};  // Battery check

/* Debounce time (ms) */
#define DEBOUNCE_MS 25

/* Hold time for battery check (ms) */
#define HOLD_MS 600


/* =========================================
   INIT
   =========================================*/

inline void buttonsInit() {
  pinMode(btn1.pin, INPUT);
  pinMode(btn2.pin, INPUT);
  pinMode(btn3.pin, INPUT);
}


/* =========================================
   RAW READ (ACTIVE-LOW → pressed = true)
   =========================================*/

inline bool readPressed(uint8_t pin) {
  return digitalRead(pin) == LOW;
}


/* =========================================
   EDGE-TRIGGERED PRESS DETECTION
   Returns true ONLY once per press
   =========================================*/

inline bool buttonPressed(Button &b) {

  bool current = readPressed(b.pin);

  if (current != b.lastState) {
    b.lastChangeTime = millis();
    b.lastState = current;
  }

  if (current && (millis() - b.lastChangeTime > DEBOUNCE_MS)) {
    // wait until release to avoid repeat
    while (readPressed(b.pin));
    delay(10);
    return true;
  }

  return false;
}


/* =========================================
   HOLD DETECTION (for battery display)
   =========================================*/

inline bool buttonHeld(Button &b) {

  if (!readPressed(b.pin)) return false;

  uint32_t start = millis();

  while (readPressed(b.pin)) {
    if (millis() - start > HOLD_MS)
      return true;
  }

  return false;
}


/* =========================================
   PUBLIC HELPERS
   =========================================*/

inline bool btn1Pressed() { return buttonPressed(btn1); }
inline bool btn2Pressed() { return buttonPressed(btn2); }
inline bool btn3Pressed() { return buttonPressed(btn3); }

inline bool btn1Held() { return buttonHeld(btn1); }
inline bool btn2Held() { return buttonHeld(btn2); }
inline bool btn3Held() { return buttonHeld(btn3); }

#endif
