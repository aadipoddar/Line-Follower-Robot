#include <Arduino.h>

/* ========= CORE ========= */
#include "config.h"
#include "state.h"

/* ========= HARDWARE ========= */
#include "buttons.h"
#include "buzzer.h"
#include "rgb.h"
#include "battery.h"
#include "motors.h"

/* ========= CONTROL ========= */
#include "qtr.h"
#include "pid.h"
#include "bt.h"

/* =========================================
   BOOT
   =========================================*/
void setup() {

  delay(200);

  buzzerInit();
  rgbInit();
  motorsInit();
  buttonsInit();
  qtrInit();
  btInit();

  beepBoot();
  rgbBootAnimation();
  rgbReady();

  setState(STATE_READY);
}


/* =========================================
   MAIN LOOP
   =========================================*/
void loop() {

  /* ---- Always read Bluetooth ---- */
  btUpdate();

  /* =====================================
     STATE MACHINE (SIMPLE & RELIABLE)
     =====================================*/
  switch (currentState) {

    /* ---------- READY ---------- */
    case STATE_READY:

      // Battery check
      if (btn1Held() || btn2Held() || btn3Held()) {
        showBatteryRGB();
        break;
      }

      // Button calibration
      if (btn1Pressed()) {
        currentState = STATE_CALIBRATING;
      }

      break;


    /* ---------- CALIBRATION ---------- */
    case STATE_CALIBRATING:

      beepCalStart();
      rgbCalibrating();

      calibrateSpin(2500);   // <<< wheels must spin here

      beepCalDone();
      rgbReady();

      currentState = STATE_READY;
      break;


    /* ---------- RUNNING ---------- */
    case STATE_RUNNING: {

      // Stop button
      if (btn2Pressed()) {
        motorsBrake();
        beepStop();
        rgbStopped();
        currentState = STATE_STOPPED;
        break;
      }

      int pos = readLinePosition();

      int left, right;
      pidToMotors(pos, left, right);
      setMotorSpeed(left, right);
      break;
    }


    /* ---------- STOPPED ---------- */
    case STATE_STOPPED:

      if (btn1Pressed()) {
        rgbReady();
        currentState = STATE_READY;
      }

      break;


    default:
      currentState = STATE_READY;
      break;
  }
}
