#ifndef CONFIG_H
#define CONFIG_H

/* ========= PINS ========= */

// QTR sensors
#define S0 A1
#define S1 A2
#define S2 A3
#define S3 A0
#define S4 A7
#define S5 A6
#define SENSOR_COUNT 6

// TB6612
#define PWMA 9
#define AIN1 10
#define AIN2 11
#define PWMB 6
#define BIN1 7
#define BIN2 8

// Buttons
#define BTN1 2   // Cal / Run
#define BTN2 3   // Stop
#define BTN3 4   // Battery check

// RGB
#define RGB_PIN 12
#define RGB_COUNT 8

// Active buzzer
#define BUZZER_PIN 5

// Battery monitor
#define BAT_PIN A5   // via voltage divider

/* ========= DEFAULT CONTROL ========= */

#define BASE_SPEED_DEFAULT 160
#define KP_DEFAULT 1.2
#define KI_DEFAULT 0.0
#define KD_DEFAULT 8.0

/* ========= FINISH DETECTION ========= */
#define FINISH_THRESHOLD 900
#define FINISH_HOLD_MS 150

#endif
