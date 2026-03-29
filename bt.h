#include "HardwareSerial.h"
#ifndef BT_H
#define BT_H

#include <Arduino.h>
#include "pid.h"
#include "rgb.h"
#include "state.h"
#include "qtr.h"
#include "motors.h"
#include "buzzer.h"
#include "battery.h"

#define BT Serial

#define BT_BUF 32
static char btBuf[BT_BUF];
static uint8_t btIndex = 0;


/* =========================================
   FLOAT MAP
   =========================================*/
inline float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


/* =========================================
   HELP MENU
   =========================================*/
inline void btHelp() {
  BT.println(F("=== LFR BT === FIRM - v1.3.0"));
  BT.println(F("P0-100 -> KP"));
  BT.println(F("I0-100 -> KI"));
  BT.println(F("D0-100 -> KD"));
  BT.println(F("S0-100 -> SPEED"));
  BT.println(F("C r g b -> RGB"));
  BT.println(F("RUN / U=STOP / Q=CAL"));
  BT.println(F("B -> battery RGB"));
  BT.println(F("R -> read PID"));
}


/* =========================================
   COMMAND PROCESSOR
   =========================================*/
inline void btProcess(char *cmd) {

  BT.print(F("CMD="));
  BT.println(cmd);

  if (cmd[0] == 'P') {
    float v = atof(cmd + 1);
    float kp = mapFloat(v, 0, 100, 0, 5);
    setKp(kp);
    BT.println(F("KP OK"));
  }

  else if (cmd[0] == 'I') {
    float v = atof(cmd + 1);
    float ki = mapFloat(v, 0, 100, 0, 0.5);
    setKi(ki);
    BT.println(F("KI OK"));
  }

  else if (cmd[0] == 'D') {
    float v = atof(cmd + 1);
    float kd = mapFloat(v, 0, 100, 0, 25);
    setKd(kd);
    BT.println(F("KD OK"));
  }

  else if (cmd[0] == 'S') {
    float v = atof(cmd + 1);
    int spd = (int)mapFloat(v, 0, 100, 80, 255);
    setBaseSpeed(spd);
    BT.println(F("SPD OK"));
  }

  else if (cmd[0] == 'C') {
    int r, g, b;
    if (sscanf(cmd, "C %d %d %d", &r, &g, &b) == 3) {
      rgbSetBT(r, g, b);
      BT.println(F("RGB OK"));
    }
  }

  else if (!strcmp(cmd, "RUN")) {
    pidReset();
    setState(STATE_RUNNING);
    beepRunStart();
  }

  else if (!strcmp(cmd, "U")) {
    motorsBrake();
    rgbStopped();
    setState(STATE_STOPPED);
    beepStop();
  }

  else if (!strcmp(cmd, "Q")) {
    setState(STATE_CALIBRATING);
    beepCalStart();
  }

  else if (!strcmp(cmd, "B")) {
    showBatteryRGB();
    BT.println(F("BAT OK"));
  }

  else if (!strcmp(cmd, "R")) {
    printPID(BT);
  }

  else {
    btHelp();
  }
}


/* =========================================
   NON-BLOCKING UPDATE
   =========================================*/
inline void btUpdate() {

  while (BT.available()) {

    char c = BT.read();

    if (c == '\n') {

      if (btIndex == 0) return;

      btBuf[btIndex] = '\0';
      btProcess(btBuf);
      btIndex = 0;
    }
    else if (c != '\r' && btIndex < BT_BUF - 1) {
      btBuf[btIndex++] = c;
    }
  }
}


/* =========================================
   INIT
   =========================================*/
inline void btInit() {
  BT.begin(9600);
  delay(150);
  btHelp();
}

#endif
