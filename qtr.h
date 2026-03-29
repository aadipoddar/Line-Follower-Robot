#ifndef QTR_H
#define QTR_H

#include <Arduino.h>
#include "config.h"
#include "motors.h"

/* =========================================
   STORAGE
   =========================================*/

static uint16_t sensorMin[SENSOR_COUNT];
static uint16_t sensorMax[SENSOR_COUNT];
static uint16_t sensorVal[SENSOR_COUNT];

/* Pin order */
static const uint8_t sensorPins[SENSOR_COUNT] = {
  S0, S1, S2, S3, S4, S5
};


/* =========================================
   INIT
   =========================================*/

inline void qtrInit() {
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    sensorMin[i] = 1023;
    sensorMax[i] = 0;
  }
}


/* =========================================
   RAW READ
   =========================================*/

inline void readRawSensors() {
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    sensorVal[i] = analogRead(sensorPins[i]);
  }
}


/* =========================================
   CALIBRATION UPDATE
   =========================================*/

inline void updateCalibration() {

  readRawSensors();

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {

    if (sensorVal[i] < sensorMin[i])
      sensorMin[i] = sensorVal[i];

    if (sensorVal[i] > sensorMax[i])
      sensorMax[i] = sensorVal[i];
  }
}


/* =========================================
   SPIN CALIBRATION (360°)
   =========================================*/

inline void calibrateSpin(uint16_t durationMs = 2500) {

  uint32_t start = millis();

  while (millis() - start < durationMs) {

    spinInPlace(120);
    updateCalibration();

    delay(5);
  }

  motorsStop();
}



/* =========================================
   CALIBRATED VALUE (0 → 1000)
   =========================================*/

inline uint16_t readCalibrated(uint8_t i) {

  uint16_t val = analogRead(sensorPins[i]);

  uint16_t minV = sensorMin[i];
  uint16_t maxV = sensorMax[i];

  if (maxV <= minV) return 0;

  int32_t scaled = (int32_t)(val - minV) * 1000 / (maxV - minV);

  if (scaled < 0) scaled = 0;
  if (scaled > 1000) scaled = 1000;

  return (uint16_t)scaled;
}


/* =========================================
   POSITION (0 → 5000)
   =========================================*/

inline int readLinePosition() {

  uint32_t weightedSum = 0;
  uint32_t total = 0;

  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {

    uint16_t v = readCalibrated(i);
    sensorVal[i] = v;  // keep for finish detection

    weightedSum += (uint32_t)v * (i * 1000);
    total += v;
  }

  /* ---- Line lost protection ---- */
  static int lastPos = 2500;

  if (total < 50) {
    return lastPos < 2500 ? 0 : 5000;
  }

  int pos = weightedSum / total;
  lastPos = pos;

  return pos;
}

#endif
