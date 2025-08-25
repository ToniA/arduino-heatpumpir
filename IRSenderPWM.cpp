#include <Arduino.h>
#include <IRSender.h>

// ESP8266 does not have the Arduino PWM control registers

#if not defined ESP8266 && not defined ESP32 && not defined LIBRETINY

// Heavily based on Ken Shirriff's IRRemote library:
// https://github.com/shirriff/Arduino-IRremote
//
// For PWM on Arduino, see http://playground.arduino.cc/Main/TimerPWMCheatsheet

#if defined(SAM3X8E) || defined(SAM3X8H)
  // Arduino Due
  uint32_t IR_USE_PWM_PINMASK;
byte IR_USE_PWM_CH;
#endif

#if defined(ARDUINO_ARCH_RENESAS_UNO)
// Arduino Uno R4
#include "pwm.h"
PwmOut* senderPWM;
#endif

IRSenderPWM::IRSenderPWM(uint8_t pin)
  : IRSender(pin) {
#if defined(ARDUINO_ARCH_RENESAS_UNO)
  // Arduino Uno R4
  senderPWM = new PwmOut(_pin);
#else
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);  // When not sending PWM, we want it low

#if defined(SAM3X8E) || defined(SAM3X8H)
  // Arduino Due
  pmc_set_writeprotect(false);

  switch (_pin) {
    case 6:
      IR_USE_PWM_PINMASK = PIO_PC24;
      IR_USE_PWM_CH = 7;
      break;

    case 7:
      IR_USE_PWM_PINMASK = PIO_PC23;
      IR_USE_PWM_CH = 6;
      break;

    case 8:
      IR_USE_PWM_PINMASK = PIO_PC22;
      IR_USE_PWM_CH = 5;
      break;

    case 9:
      IR_USE_PWM_PINMASK = PIO_PC21;
      IR_USE_PWM_CH = 4;
      break;

    case 34:
      IR_USE_PWM_PINMASK = PIO_PC2;
      IR_USE_PWM_CH = 0;
      break;

    case 36:
      IR_USE_PWM_PINMASK = PIO_PC4;
      IR_USE_PWM_CH = 1;
      break;

    case 38:
      IR_USE_PWM_PINMASK = PIO_PC6;
      IR_USE_PWM_CH = 2;
      break;

    case 40:
      IR_USE_PWM_PINMASK = PIO_PC8;
      IR_USE_PWM_CH = 3;
      break;
  }
#endif
#endif
}

// Set the PWM frequency. The selected pin determines which timer to use
void IRSenderPWM::setFrequency(int frequency) {
#if defined(SAM3X8E) || defined(SAM3X8H)
  // Arduino Due
  pmc_enable_periph_clk(PWM_INTERFACE_ID);
  const uint32_t pwmval = (frequency)*2000;
  PWMC_ConfigureClocks(PWM_FREQUENCY * PWM_MAX_DUTY_CYCLE, pwmval, F_CPU);
  PIO_Configure(PIOC, PIO_PERIPH_B, IR_USE_PWM_PINMASK, PIO_DEFAULT);
  PWMC_ConfigureChannel(PWM_INTERFACE, IR_USE_PWM_CH, PWM_CMR_CPRE_CLKB, 0, 0);
  PWMC_SetPeriod(PWM_INTERFACE, IR_USE_PWM_CH, 2);
  PWMC_SetDutyCycle(PWM_INTERFACE, IR_USE_PWM_CH, 1);
#else

  uint8_t pwmval8 = F_CPU / 2000 / (frequency);
  uint16_t pwmval16 = F_CPU / 2000 / (frequency);

  switch (_pin) {
#if defined(AVR_ATmega1280) || defined(AVR_ATmega2560)
    // Arduino Mega
    case 9:
    // Fall-through to 10, timer2 controls both 9 and 10
    case 10:
      TCCR2A = _BV(WGM20);
      TCCR2B = _BV(WGM22) | _BV(CS20);
      OCR2A = pwmval8;
      OCR2B = pwmval8 / 3;
      break;
    case 11:
    // Fall-through to 12, timer1 controls both 11 and 12
    case 12:
      TCCR1A = _BV(WGM11);
      TCCR1B = _BV(WGM13) | _BV(CS10);
      ICR1 = pwmval16;
      OCR1A = pwmval16 / 3;
      OCR1B = pwmval16 / 3;
      break;
    case 44:
    // Fall-through to 46, timer 5 controls pins 44, 45 and 46 on Arduino Mega
    case 45:
    case 46:
      TCCR5A = _BV(WGM51);  // This gives correct data from pins 44,45,46 and similar range to pin 9
      TCCR5B = _BV(WGM53) | _BV(CS50);
      ICR5 = pwmval16;
      OCR5A = pwmval16 / 3;
      OCR5B = pwmval16 / 3;  // Also enable pin 45
      OCR5C = pwmval16 / 3;  // Also enable pin 44
#endif
#if defined(AVR_ATmega32U4)
    // Arduino Leonardo
    case 5:
      TCCR3A = (1 << WGM31);                               // Enable Fast PWM, Clear OC3A on compare
      TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS30);  // Mode 14, No prescaler
      ICR3 = 420;                                          // TOP value for 38 kHz PWM
      OCR3A = 210;                                         // 50% duty cycle
      break;
    case 9:
    case 10:
      TCCR1A = (1 << WGM11);                               // Enable Fast PWM, Clear OC1B on compare
      TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);  // Mode 14, No prescaler
      ICR1 = 420;                                          // TOP value for 38 kHz PWM
      OCR1B = 210;                                         // 50% duty cycle
      break;
    case 6:
    case 11:
      TCCR4B = (1 << CS40);   // No prescaler
      TCCR4C = 0;             // No special functions
      TCCR4D = (1 << WGM40);  // Enable Fast PWM mode
      TCCR4E = 0;             // No additional extensions
      OCR4A = 210;            // 50% duty cycle on pin 11
      OCR4D = 210;            // 50% duty cycle on pin 6
      break;
#endif
#if defined(AVR_ATmega8) || defined(AVR_ATmega328P)
    // Arduino Duemilanove or Uno R3
    case 3:
    // Fall-through to 11, timer2 controls both 3 and 11
    case 11:
      TCCR2A = _BV(WGM20);
      TCCR2B = _BV(WGM22) | _BV(CS20);
      OCR2A = pwmval8;
      OCR2B = pwmval8 / 3;
      break;
    case 9:
    // Fall-through to 10, timer1 controls both 9 and 10
    case 10:
      TCCR1A = _BV(WGM11);
      TCCR1B = _BV(WGM13) | _BV(CS10);
      ICR1 = pwmval16;
      OCR1A = pwmval16 / 3;
      OCR1B = pwmval16 / 3;
      break;
#endif
  }
#endif
}

// Send an IR 'mark' symbol, i.e. transmitter ON
void IRSenderPWM::mark(int markLength) {
#if defined(ARDUINO_ARCH_RENESAS_UNO)
  // Arduino Uno R4
  senderPWM->begin(38000.0f, 50.0f);  // Enable PWM at 38 kHz, 50% duty
#else
#if defined(SAM3X8E) || defined(SAM3X8H)
  // Arduino Due
  PWMC_EnableChannel(PWM_INTERFACE, IR_USE_PWM_CH);  // Enable pin PWM output
#else
  switch (_pin) {
#if defined(AVR_ATmega1280) || defined(AVR_ATmega2560)
    // Arduino Mega
    case 9:
      (TCCR2A |= _BV(COM2B1));  // Enable pin 3 PWM output
      break;
    case 11:
      (TCCR1A |= _BV(COM1A1));  // Enable pin 9 PWM output
      break;
    case 12:
      (TCCR1A |= _BV(COM1B1));  // Enable pin 10 PWM output
      break;
    case 10:
      (TCCR2A |= _BV(COM2A1));  // Enable pin 11 PWM output
      break;
    case 44:
      (TCCR5A |= _BV(COM5C1));  // Enable pin 44 PWM output on Arduino Mega
      break;
    case 45:
      (TCCR5A |= _BV(COM5B1));  // Enable pin 45 PWM output on Arduino Mega
      break;
    case 46:
      (TCCR5A |= _BV(COM5A1));  // Enable pin 46 PWM output on Arduino Mega
      break;
#endif
#if defined(AVR_ATmega32U4)
    // Arduino Leonardo
    case 5:
      TCCR3A |= _BV(COM3A1);  // Enable PWM on Pin 5
      break;
    case 6:
      TCCR4A |= (1 << COM4D1);  // Enable PWM on Pin 6
      break;
    case 9:
      TCCR1A |= _BV(COM1A1);  // Enable PWM on Pin 9
      break;
    case 10:
      TCCR1A |= _BV(COM1B1);  // Enable PWM on Pin 10
      break;
    case 11:
      TCCR4A |= (1 << COM4A1);  // Enable PWM on Pin 11
      break;
#endif
#if defined(AVR_ATmega8) || defined(AVR_ATmega328P)
    // Arduino Duemilanove or Uno R3
    case 3:
      (TCCR2A |= _BV(COM2B1));  // Enable pin 3 PWM output
      break;
    case 9:
      (TCCR1A |= _BV(COM1A1));  // Enable pin 9 PWM output
      break;
    case 10:
      (TCCR1A |= _BV(COM1B1));  // Enable pin 10 PWM output
      break;
    case 11:
      (TCCR2A |= _BV(COM2A1));  // Enable pin 11 PWM output
      break;
#endif
  }
#endif
#endif

  delayMicroseconds(markLength);
}

// Send an IR 'space' symbol, i.e. transmitter OFF
void IRSenderPWM::space(int spaceLength) {
#if defined(ARDUINO_ARCH_RENESAS_UNO)
  // Arduino Uno R4
  senderPWM->end();  // Disable PWM on Pin
#else
#if defined(SAM3X8E) || defined(SAM3X8H)
  // Arduino Due
  PWMC_DisableChannel(PWM_INTERFACE, IR_USE_PWM_CH);  // Disable pin PWM output
#else
  switch (_pin) {
#if defined(AVR_ATmega1280) || defined(AVR_ATmega2560)
    // Arduino Mega
    case 9:
      (TCCR2A &= ~(_BV(COM2B1)));  // Disable pin 3 PWM output
      break;
    case 11:
      (TCCR1A &= ~(_BV(COM1A1)));  // Disable pin 9 PWM output
      break;
    case 12:
      (TCCR1A &= ~(_BV(COM1B1)));  // Disable pin 10 PWM output
      break;
    case 10:
      (TCCR2A &= ~(_BV(COM2A1)));  // Disable pin 11 PWM output
      break;
    case 44:
      (TCCR5A &= ~(_BV(COM5C1)));  // Disable pin 44 PWM output on Arduino Mega
    case 45:
      (TCCR5A &= ~(_BV(COM5B1)));  // Disable pin 45 PWM output on Arduino Mega
    case 46:
      (TCCR5A &= ~(_BV(COM5A1)));  // Disable pin 46 PWM output on Arduino Mega
#endif
#if defined(AVR_ATmega32U4)
    // Arduino Leonardo
    case 5:
      TCCR3A &= ~(_BV(COM3A1));  // Disable PWM (Output forced LOW)
      break;
    case 6:
      TCCR4A &= ~(1 << COM4D1);  // Disable PWM (Output forced LOW)
      break;
    case 9:
      TCCR1A &= ~(_BV(COM1A1));  // Disable PWM (Output forced LOW)
      break;
    case 10:
      TCCR1A &= ~(_BV(COM1B1));  // Disable PWM (Output forced LOW)
      break;
    case 11:
      TCCR4A &= ~(1 << COM4A1);  // Disable PWM (Output forced LOW)
      break;
#endif
#if defined(AVR_ATmega8) || defined(AVR_ATmega328P)
    // Arduino Duemilanove or Uno R3
    case 3:
      (TCCR2A &= ~(_BV(COM2B1)));  // Disable pin 3 PWM output
      break;
    case 9:
      (TCCR1A &= ~(_BV(COM1A1)));  // Disable pin 9 PWM output
      break;
    case 10:
      (TCCR1A &= ~(_BV(COM1B1)));  // Disable pin 10 PWM output
      break;
    case 11:
      (TCCR2A &= ~(_BV(COM2A1)));  // Disable pin 11 PWM output
      break;
#endif
  }
#endif
#endif

  // Mitsubishi heatpump uses > 16383us spaces, and delayMicroseconds only works up to 2^14 - 1 us
  // Use the less accurate milliseconds delay for longer delays

  if (spaceLength < 16383) {
    delayMicroseconds(spaceLength);
  } else {
    delay(spaceLength / 1000);
  }
}

#endif  // ESP8266
