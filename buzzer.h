#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "config.h"

/* =========================================
   BASIC CONTROL
   =========================================*/

inline void buzzerOn()  { digitalWrite(BUZZER_PIN, HIGH); }
inline void buzzerOff() { digitalWrite(BUZZER_PIN, LOW);  }

/* Short blocking beep (safe: used only in transitions) */
inline void beep(uint16_t ms = 80) {
  buzzerOn();
  delay(ms);
  buzzerOff();
  delay(40);
}


/* =========================================
   PATTERNS
   =========================================*/

/* ---- Boot pattern ---- */
inline void beepBoot() {
  beep(60);
  beep(60);
}

/* ---- Calibration start (2 beeps) ---- */
inline void beepCalStart() {
  beep(70);
  beep(70);
}

/* ---- Calibration done (3 quick beeps) ---- */
inline void beepCalDone() {
  beep(60);
  beep(60);
  beep(60);
}

/* ---- Run start (one long beep) ---- */
inline void beepRunStart() {
  buzzerOn();
  delay(250);
  buzzerOff();
  delay(60);
}

/* ---- Stop / finish (double medium beep) ---- */
inline void beepStop() {
  beep(120);
  beep(120);
}

/* ---- Error alert ---- */
inline void beepError() {
  for (uint8_t i = 0; i < 4; i++) {
    beep(40);
  }
}


/* =========================================
   INIT
   =========================================*/

inline void buzzerInit() {
  pinMode(BUZZER_PIN, OUTPUT);
  buzzerOff();
}

#endif
