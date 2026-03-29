#ifndef STATE_H
#define STATE_H

/* =========================================
   ROBOT STATES
   =========================================*/
enum RobotState {
  STATE_BOOT = 0,
  STATE_READY,
  STATE_CALIBRATING,
  STATE_RUNNING,
  STATE_STOPPED
};

/* =========================================
   SINGLE GLOBAL STATE (REAL VARIABLE)
   =========================================*/
static RobotState currentState = STATE_BOOT;

/* =========================================
   HELPER
   =========================================*/
inline void setState(RobotState newState) {
  currentState = newState;
}

#endif
