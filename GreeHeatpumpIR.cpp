#include <GreeHeatpumpIR.h>

GreeHeatpumpIR::GreeHeatpumpIR() : HeatpumpIR()
{
  static const char PROGMEM model[] PROGMEM = "gree";
  static const char PROGMEM info[]  PROGMEM = "{\"mdl\":\"gree\",\"dn\":\"Gree\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}


void GreeHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd , uint8_t fanSpeedCmd , uint8_t temperatureCmd , uint8_t swingVCmd , uint8_t swingHCmd )
{
  (void)swingVCmd;
  (void)swingHCmd;

  // Sensible defaults for the heat pump mode

  uint8_t powerMode = GREE_AIRCON1_POWER_ON;
  uint8_t operatingMode = GREE_AIRCON1_MODE_HEAT;
  uint8_t fanSpeed = GREE_AIRCON1_FAN_AUTO;
  uint8_t temperature = 21;
  uint8_t swingV=0;
  uint8_t swingH=0;

  if (powerModeCmd == POWER_OFF)
  {
    powerMode = GREE_AIRCON1_POWER_OFF;
  }
  else
  {
    switch (operatingModeCmd)
    {
      case MODE_AUTO:
        operatingMode = GREE_AIRCON1_MODE_AUTO;
        temperatureCmd = 25;        
        break;
      case MODE_HEAT:
        operatingMode = GREE_AIRCON1_MODE_HEAT;
        break;
      case MODE_COOL:
        operatingMode = GREE_AIRCON1_MODE_COOL;
        break;
      case MODE_DRY:
        operatingMode = GREE_AIRCON1_MODE_DRY;
        fanSpeedCmd = FAN_1;
        break;
      case MODE_FAN:
        operatingMode = GREE_AIRCON1_MODE_FAN;
        break;
    }
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = GREE_AIRCON1_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = GREE_AIRCON1_FAN1;
      break;
    case FAN_2:
      fanSpeed = GREE_AIRCON1_FAN2;
      break;
    case FAN_3:
      fanSpeed = GREE_AIRCON1_FAN3;
      break;
  }

  if (temperatureCmd > 15 && temperatureCmd < 31)
  {
    temperature = temperatureCmd - 16;
  }

  sendGree(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH);
}

// Send the Gree code
void GreeHeatpumpIR::sendGree(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV ,uint8_t swingH)
{
  (void)swingV;
  (void)swingH;

  uint8_t GreeTemplate[] = { 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x20, 0x00, 0x00 };
  //                            0     1     2     3     4     5     6     7     8

  uint8_t i;

  // Set the Fan speed, operating mode and power state
  GreeTemplate[0] = fanSpeed | operatingMode | powerMode;
  // Set the temperature
  GreeTemplate[1] = temperature;

  // Calculate the checksum
  GreeTemplate[8] = (
    (GreeTemplate[0] & 0x0F) + 
    (GreeTemplate[1] & 0x0F) + 
		(GreeTemplate[2] & 0x0F) + 
    (GreeTemplate[3] & 0x0F) + 
		(GreeTemplate[5] & 0xF0) >> 4 + 
    (GreeTemplate[6] & 0xF0) >> 4 + 
		(GreeTemplate[7] & 0xF0) >> 4 + 
     0x0A) & 0xF0;

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Send Header mark
  IR.mark(GREE_AIRCON1_HDR_MARK);
  IR.space(GREE_AIRCON1_HDR_SPACE);

  // Payload part #1
  for (i=0; i<4; i++) {
    IR.sendIRbyte(GreeTemplate[i], GREE_AIRCON1_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
  }

  // Only three first bits of byte 4 are sent, this is always '010'
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ZERO_SPACE);
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ONE_SPACE);
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_ZERO_SPACE);
  
  // Message space
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(GREE_AIRCON1_MSG_SPACE);

  // Payload part #2
  for (i=5; i<9; i++) {
    IR.sendIRbyte(GreeTemplate[i], GREE_AIRCON1_BIT_MARK, GREE_AIRCON1_ZERO_SPACE, GREE_AIRCON1_ONE_SPACE);
  }

  // End mark
  IR.mark(GREE_AIRCON1_BIT_MARK);
  IR.space(0);
}