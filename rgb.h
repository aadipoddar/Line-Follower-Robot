#ifndef RGB_H
#define RGB_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"

/* =========================================
   OBJECT (global, but small enough for Nano)
   =========================================*/
static Adafruit_NeoPixel strip(RGB_COUNT, RGB_PIN, NEO_GRB + NEO_KHZ800);


/* =========================================
   INIT
   =========================================*/
inline void rgbInit() {
  strip.begin();
  strip.clear();
  strip.show();
}


/* =========================================
   BASIC COLOR SET
   =========================================*/
inline void rgbSet(uint8_t r, uint8_t g, uint8_t b) {
  for (uint8_t i = 0; i < RGB_COUNT; i++)
    strip.setPixelColor(i, strip.Color(r, g, b));

  strip.show();
}

inline void rgbOff() {
  rgbSet(0, 0, 0);
}


/* =========================================
   STATUS COLORS (competition standard)
   =========================================*/
inline void rgbReady()       { rgbSet(255, 255, 255); } // white
inline void rgbCalibrating() { rgbSet(255, 255, 255); }   // white
inline void rgbRunning()     { rgbSet(0, 255, 0); }     // green
inline void rgbStopped()     { rgbSet(0, 0, 255); }     // blue
inline void rgbError()       { rgbSet(255, 0, 0); }     // red


/* =========================================
   BOOT ANIMATION (short, lightweight)
   =========================================*/
inline void rgbBootAnimation() {

  strip.clear();

  // simple blue wipe
  for (uint8_t i = 0; i < RGB_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255));
    strip.show();
    delay(40);
  }

  delay(120);
}


/* =========================================
   RGB TEST CYCLE
   =========================================*/
inline void rgbTestCycle(uint16_t wait = 250) {
  rgbSet(255, 0, 0);   delay(wait);
  rgbSet(0, 255, 0);   delay(wait);
  rgbSet(0, 0, 255);   delay(wait);
  rgbSet(255, 255, 255); delay(wait);
  rgbOff();
}


/* =========================================
   BATTERY COLOR DISPLAY
   expects voltage already mapped to 0-100 %
   =========================================*/
inline void rgbBatteryLevel(uint8_t percent) {

  if (percent > 70) {
    rgbSet(0, 255, 0);      // green
  }
  else if (percent > 40) {
    rgbSet(255, 150, 0);    // orange/yellow
  }
  else {
    rgbSet(255, 0, 0);      // red
  }
}


/* =========================================
   BLUETOOTH CUSTOM COLOR
   =========================================*/
static uint8_t btR = 255;
static uint8_t btG = 255;
static uint8_t btB = 255;

inline void rgbSetBT(uint8_t r, uint8_t g, uint8_t b) {
  btR = r;
  btG = g;
  btB = b;
  rgbSet(btR, btG, btB);
}


/* =========================================
   BREATHING EFFECT (very light, non-blocking step)
   call repeatedly in loop
   =========================================*/
inline void rgbBreathStep() {

  static int brightness = 0;
  static int dir = 5;

  brightness += dir;

  if (brightness <= 0 || brightness >= 255)
    dir = -dir;

  uint8_t r = (btR * brightness) / 255;
  uint8_t g = (btG * brightness) / 255;
  uint8_t b = (btB * brightness) / 255;

  rgbSet(r, g, b);
}

#endif
