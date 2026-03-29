#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>
#include "config.h"
#include "rgb.h"

/* =========================================
   VOLTAGE DIVIDER (47k / 10k)
   Ratio = (47 + 10) / 10 = 5.7
   =========================================*/
#define DIVIDER_RATIO 5.7f

/* Arduino Nano ADC reference */
#define ADC_REF 5.0f

/* 2S Li-Po limits */
#define BAT_FULL  8.4f
#define BAT_EMPTY 6.6f


/* =========================================
   READ BATTERY VOLTAGE
   =========================================*/
inline float batteryVoltage() {

  uint16_t raw = analogRead(BAT_PIN);

  float v = (raw / 1023.0f) * ADC_REF * DIVIDER_RATIO;

  return v;
}


/* =========================================
   BATTERY PERCENT (linear estimate)
   =========================================*/
inline uint8_t batteryPercent() {

  float v = batteryVoltage();

  if (v >= BAT_FULL)  return 100;
  if (v <= BAT_EMPTY) return 0;

  return (uint8_t)((v - BAT_EMPTY) * 100.0f / (BAT_FULL - BAT_EMPTY));
}


/* =========================================
   SHOW BATTERY USING RGB
   =========================================*/
inline void showBatteryRGB() {

  uint8_t p = batteryPercent();

  rgbBatteryLevel(p);

  delay(1500);   // show for 1.5 s
  rgbReady();
}

#endif
