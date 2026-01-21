/*
    Compatibility header for Arduino and ESP-IDF
    Provides unified interface for both frameworks
*/
#ifndef HeatpumpIRCompat_h
#define HeatpumpIRCompat_h

#ifdef ARDUINO
// Arduino framework - use standard Arduino types and functions
#include <Arduino.h>

#elif defined(ESP_PLATFORM)
// ESP-IDF framework (non-Arduino)
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

// PROGMEM is a no-op on ESP32 (all memory is directly addressable)
#ifndef PROGMEM
#define PROGMEM
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#endif
#ifndef pgm_read_byte_near
#define pgm_read_byte_near(addr) (*(const uint8_t *)(addr))
#endif
#ifndef memcpy_P
#define memcpy_P(dst, src, n) memcpy(dst, src, n)
#endif
#ifndef PSTR
#define PSTR(s) (s)
#endif
#ifndef strcmp_P
#define strcmp_P(a, b) strcmp(a, b)
#endif
#ifndef sprintf_P
#define sprintf_P sprintf
#endif
#ifndef F
#define F(s) (s)
#endif

// Arduino type aliases
typedef bool boolean;
typedef uint8_t byte;

// GPIO definitions
#ifndef INPUT
#define INPUT   GPIO_MODE_INPUT
#endif
#ifndef OUTPUT
#define OUTPUT  GPIO_MODE_OUTPUT
#endif
#ifndef HIGH
#define HIGH    1
#endif
#ifndef LOW
#define LOW     0
#endif

// Timing functions
inline void delayMicroseconds(uint32_t us) {
  esp_rom_delay_us(us);
}

inline void delay(uint32_t ms) {
  vTaskDelay(pdMS_TO_TICKS(ms));
}

inline unsigned long micros() {
  return (unsigned long) esp_timer_get_time();
}

inline unsigned long millis() {
  return (unsigned long) (esp_timer_get_time() / 1000);
}

// GPIO functions
inline void pinMode(uint8_t pin, uint8_t mode) {
  gpio_config_t io_conf = {};
  io_conf.pin_bit_mask = (1ULL << pin);
  io_conf.mode = (gpio_mode_t) mode;
  io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
  io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
  io_conf.intr_type = GPIO_INTR_DISABLE;
  gpio_config(&io_conf);
}

inline void digitalWrite(uint8_t pin, uint8_t val) {
  gpio_set_level((gpio_num_t) pin, val);
}

inline int digitalRead(uint8_t pin) {
  return gpio_get_level((gpio_num_t) pin);
}

#else
#error "Unsupported platform - requires Arduino or ESP-IDF"
#endif

#endif // HeatpumpIRCompat_h
