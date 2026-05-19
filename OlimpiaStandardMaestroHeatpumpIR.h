#ifndef OlimpiaStandardMaestroHeatpumpIR_h
#define OlimpiaStandardMaestroHeatpumpIR_h

#include <HeatpumpIR.h>

/**
 * Various options for the heatpump
 */
struct HeatpumpState {
  bool power;              // power state: true = on, false = off
  bool flapSwing;          // flap oscillation enabled
  uint8_t tempC;           // temperature in Celsius (18–30)
  uint8_t mode;            // MODE_COOL, MODE_HEAT, MODE_DRY, MODE_AUTO, MODE_FAN
  bool isFahrenheit;       // temperature unit: false = Celsius, true = Fahrenheit
  bool lowNoise;           // low noise mode
  uint8_t fanSpeed;        // FAN_LOW, FAN_MEDIUM, FAN_HIGH, FAN_AUTO
  bool eco;                // eco mode enabled
  bool displayHide;        // true to turn off the LED display on the unit
};

class OlimpiaStandardMaestroHeatpumpIR : public HeatpumpIR {
public:
  OlimpiaStandardMaestroHeatpumpIR();
  void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd) override;
  void send(IRSender& IR, uint8_t currentTemperature) override;
  void sendMaestro(IRSender &IR, const HeatpumpState &state);
};

#endif
