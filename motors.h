#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "config.h"


/* =========================================
   DIRECTION INVERSION FLAGS
   Change true/false to match wiring
   =========================================*/
#define INVERT_LEFT  true
#define INVERT_RIGHT true
/* =========================================
   INTERNAL HELPER
   =========================================*/

inline int clampSpeed(int v) {
  if (v > 255) return 255;
  if (v < -255) return -255;
  return v;
}


/* =========================================
   INIT (safe boot)
   =========================================*/

inline void motorsInit() {

  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);

  pinMode(PWMB, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  // Ensure motors OFF
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}


/* =========================================
   LOW LEVEL DRIVE
   =========================================*/

inline void driveA(int speed) {

  speed = clampSpeed(speed);

  if (speed > 0) {
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    analogWrite(PWMA, speed);
  }
  else if (speed < 0) {
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    analogWrite(PWMA, -speed);
  }
  else {
    analogWrite(PWMA, 0);
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, LOW);
  }
}


inline void driveB(int speed) {

  speed = clampSpeed(speed);

  if (speed > 0) {
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    analogWrite(PWMB, speed);
  }
  else if (speed < 0) {
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    analogWrite(PWMB, -speed);
  }
  else {
    analogWrite(PWMB, 0);
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, LOW);
  }
}


/* =========================================
   MAIN CONTROL (used by PID)
   =========================================*/



/* =========================================
   MAIN CONTROL (used by PID)
   =========================================*/
inline void setMotorSpeed(int left, int right) {

  // Apply inversion if needed
  if (INVERT_LEFT)  left  = -left;
  if (INVERT_RIGHT) right = -right;

  driveA(left);
  driveB(right);
}



/* =========================================
   STOP & BRAKE
   =========================================*/

inline void motorsStop() {
  driveA(0);
  driveB(0);
}

/* Fast electronic brake */
inline void motorsBrake() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, HIGH);

  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
}


/* =========================================
   CALIBRATION SPIN HELPER
   =========================================*/

inline void spinInPlace(int speed = 120) {
  setMotorSpeed(-speed, speed);
}

#endif
