#include <Arduino.h>
#include <HeatpumpIR.h>

HeatpumpIR::HeatpumpIR()
{
}

void HeatpumpIR::send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd)
{
}

// This is a virtual function, i.e. never called
const prog_char* HeatpumpIR::supportedModel()
{
}