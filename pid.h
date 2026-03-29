#ifndef PID_H
#define PID_H

#include <Arduino.h>
#include "config.h"

/* =========================================
   TUNABLE PARAMETERS (runtime changeable)
   =========================================*/

static float Kp = KP_DEFAULT;
static float Ki = KI_DEFAULT;
static float Kd = KD_DEFAULT;

static int baseSpeed = BASE_SPEED_DEFAULT;


/* =========================================
   INTERNAL STATE
   =========================================*/

static float integral = 0;
static int lastError = 0;

/* Integral clamp to avoid windup */
#define INTEGRAL_LIMIT 1000


/* =========================================
   RESET BEFORE RUN
   =========================================*/

inline void pidReset() {
  integral = 0;
  lastError = 0;
}


/* =========================================
   MAIN PID COMPUTE
   Input: position (0 → 5000)
   Output: correction (+/-)
   =========================================*/

inline int computePID(int position) {

  const int center = 2500;
  int error = position - center;

  /* ---- Proportional ---- */
  float P = Kp * error;

  /* ---- Integral ---- */
  integral += error;

  if (integral > INTEGRAL_LIMIT) integral = INTEGRAL_LIMIT;
  if (integral < -INTEGRAL_LIMIT) integral = -INTEGRAL_LIMIT;

  float I = Ki * integral;

  /* ---- Derivative ---- */
  int derivative = error - lastError;
  float D = Kd * derivative;

  lastError = error;

  /* ---- Output ---- */
  return (int)(P + I + D);
}


/* =========================================
   CORNER BOOST PARAMETERS
   =========================================*/
#define MAX_SPEED   200   // straight speed
#define MIN_SPEED   120   // curve speed
#define BOOST_RANGE 1500  // how fast speed drops in turns


inline void pidToMotors(int position, int &left, int &right) {

  int correction = computePID(position);

  /* ---------- dynamic base speed ---------- */
  int error = abs(position - 2500);

  int dynamicSpeed = MAX_SPEED;

  if (error > BOOST_RANGE)
    dynamicSpeed = MIN_SPEED;
  else {
    dynamicSpeed = MAX_SPEED -
      (error * (MAX_SPEED - MIN_SPEED) / BOOST_RANGE);
  }

  /* ---------- motor mix ---------- */
  left  = dynamicSpeed + correction;
  right = dynamicSpeed - correction;

  /* clamp */
  if (left > 255) left = 255;
  if (left < -255) left = -255;

  if (right > 255) right = 255;
  if (right < -255) right = -255;
}



/* =========================================
   BLUETOOTH TUNING HELPERS
   =========================================*/

inline void setKp(float v) { Kp = v; }
inline void setKi(float v) { Ki = v; }
inline void setKd(float v) { Kd = v; }
inline void setBaseSpeed(int v) { baseSpeed = v; }

inline float getKp() { return Kp; }
inline float getKi() { return Ki; }
inline float getKd() { return Kd; }
inline int   getBaseSpeed() { return baseSpeed; }


/* =========================================
   DEBUG PRINT (for BT)
   =========================================*/

inline void printPID(Stream &out) {
  out.print("KP="); out.print(Kp, 3);
  out.print(" KI="); out.print(Ki, 3);
  out.print(" KD="); out.print(Kd, 3);
  out.print(" SPD="); out.println(baseSpeed);
}

#endif
