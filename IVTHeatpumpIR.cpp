#include <IVTHeatpumpIR.h>


IVTHeatpumpIR::IVTHeatpumpIR() : SharpHeatpumpIR()
{
  static const char PROGMEM model[] PROGMEM = "ivt";
  static const char PROGMEM info[]  PROGMEM = "{\"mdl\":\"ivt\",\"dn\":\"IVT AY-XP12FR-N\",\"mT\":18,\"xT\":32,\"fs\":3,\"maint\":[10]}}";

  _model = model;
  _info = info; 
}

void IVTHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd)
{
  _sharpModel = MODEL_IVT;

  SharpHeatpumpIR::send(IR, powerModeCmd, operatingModeCmd, fanSpeedCmd, temperatureCmd, swingVCmd, swingHCmd);
}
