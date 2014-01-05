#include <Arduino.h>
#include <HeatpumpIR.h>

HeatpumpIR::HeatpumpIR()
{
}

// This is a virtual function, i.e. never called
void HeatpumpIR::send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd)
{
}

// Heatpump model and info getters
const prog_char* HeatpumpIR::model()
{
  return _model;
}

const prog_char* HeatpumpIR::info()
{
  return _info;
}