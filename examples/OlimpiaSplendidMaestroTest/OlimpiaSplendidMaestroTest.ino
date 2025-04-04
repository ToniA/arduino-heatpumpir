// #define USE_DISPLAY // uncomment if you want to use a display

#ifdef ESP8266
#define BTN_MODE   14 // D5  // GPIO14
#else
#define BTN_MODE   11 // more accessible on an arduino uno 
#endif
#define BTN_TEMPUP 12 // D6  // GPIO12
#define BTN_TEMPDN 13 // D7  // GPIO13

#ifdef USE_DISPLAY
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SDA 5
#define SCL 4
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
#endif

#include <IRSender.h>
#include <OlimpiaStandardMaestroHeatpumpIR.h>

OlimpiaStandardMaestroHeatpumpIR maestro;
HeatpumpState state;
IRSender *ir;

void displayState(const HeatpumpState &state) {
  #ifdef USE_DISPLAY
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(state.tempC);
  display.print(state.isFahrenheit ? "F" : "C");

  display.setCursor(0, 20);
  display.print("Mode: ");
  switch (state.mode) {
    case MODE_COOL: display.print("Cool"); break;
    case MODE_HEAT: display.print("Heat"); break;
    case MODE_DRY: display.print("Dry"); break;
    case MODE_FAN: display.print("Fan"); break;
    case MODE_AUTO: display.print("Auto"); break;
    default: display.print("Off"); break;
  }

  display.display();
  #endif
}

void setup() {
  Serial.begin(9600);
  #ifdef ESP8266
  ir = new IRSenderIRremoteESP8266(2); // GPIO2 = D4
  #else
  ir = new IRSenderPWM(3);  // Use pin 3 for IR LED output
  #endif

  Serial.println(F("Starting."));
  #ifdef USE_DISPLAY
  Wire.begin(SDA,SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();
  #endif
  Serial.println(F("Sending initial test."));

  pinMode(BTN_MODE, INPUT_PULLUP);
  pinMode(BTN_TEMPUP, INPUT_PULLUP);
  pinMode(BTN_TEMPDN, INPUT_PULLUP);

  state.power = true;
  state.mode = MODE_COOL;
  state.tempC = 23;
  state.fanSpeed = FAN_AUTO;
  state.flapSwing = false;
  state.eco = false;
  state.lowNoise = false;
  state.isFahrenheit = false;
  state.displayHide = false;

  displayState(state);
  maestro.sendMaestro(*ir, state);
}

void loop() {
  static unsigned long lastDebounce = 0;
  if (millis() - lastDebounce > 2500) {
    if (digitalRead(BTN_MODE) == LOW) {
      state.mode = (state.mode + 1) % 5; // Cycle through modes 0–4
      lastDebounce = millis();
      displayState(state);
      maestro.sendMaestro(*ir, state);
    } else if (digitalRead(BTN_TEMPUP) == LOW) {
      if (state.tempC < 30) state.tempC++;
      lastDebounce = millis();
      displayState(state);
      maestro.sendMaestro(*ir, state);
    } else if (digitalRead(BTN_TEMPDN) == LOW) {
      if (state.tempC > 15) state.tempC--;
      lastDebounce = millis();
      displayState(state);
      maestro.sendMaestro(*ir, state);
    }
  }
}
