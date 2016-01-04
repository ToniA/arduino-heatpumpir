#include <MitsubishiHeavyHeatpumpIR.h>


MitsubishiHeavyHeatpumpIR::MitsubishiHeavyHeatpumpIR()
{
  static const char PROGMEM model[] PROGMEM = "mitsubishi_heavy";
  static const char PROGMEM info[]  PROGMEM = "{\"mdl\":\"mitsubishi_heavy\",\"dn\":\"Mitsubishi Heavy\",\"mT\":18,\"xT\":30,\"fs\":5}"; ///////

  _model = model;
  _info = info;
}


void MitsubishiHeavyHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd)
{
  // Sensible defaults for the heat pump mode

  uint8_t powerMode     = MITSUBISHI_AIRCON2_MODE_ON;
  uint8_t operatingMode = MITSUBISHI_AIRCON2_MODE_HEAT;
  uint8_t fanSpeed      = MITSUBISHI_AIRCON2_FAN_AUTO;
  uint8_t temperature   = 23;
  uint8_t swingV        = 0;
  uint8_t swingH        = 0;

  (void)swingVCmd;
  (void)swingHCmd;

  if (powerModeCmd == 0)
  {
    powerMode = MITSUBISHI_AIRCON2_MODE_OFF;
  }

  switch (operatingModeCmd)
  {
    case MODE_AUTO:
      operatingMode = MITSUBISHI_AIRCON2_MODE_AUTO;
      break;
    case MODE_HEAT:
      operatingMode = MITSUBISHI_AIRCON2_MODE_HEAT;
      break;
    case MODE_COOL:
      operatingMode = MITSUBISHI_AIRCON2_MODE_COOL;
      break;
    case MODE_DRY:
      operatingMode = MITSUBISHI_AIRCON2_MODE_DRY;
      break;
    case MODE_FAN:
      operatingMode = MITSUBISHI_AIRCON2_MODE_FAN;
      break;
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = MITSUBISHI_AIRCON2_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = MITSUBISHI_AIRCON2_FAN1;
      break;
    case FAN_2:
      fanSpeed = MITSUBISHI_AIRCON2_FAN2;
      break;
    case FAN_3:
      fanSpeed = MITSUBISHI_AIRCON2_FAN3;
      break;
    case FAN_4:
      fanSpeed = MITSUBISHI_AIRCON2_FAN4;
      break;
    case FAN_5:
      fanSpeed = MITSUBISHI_AIRCON2_FAN5;
      break;
  }

  if ( temperatureCmd > 17 && temperatureCmd < 31)
  {
    temperature = (~((temperatureCmd - 17) << 4)) & 0xF0;
  }

  sendMitsubishiHeavy(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH);
}

void MitsubishiHeavyHeatpumpIR::sendMitsubishiHeavy(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH)
{
  (void)swingV;
  (void)swingH;

  uint8_t MitsubishiHeavyTemplate[] = { 0x52, 0xAE, 0xC3, 0x26, 0xD9, 0x7B, 0x00, 0x07, 0x00, 0x00, 0x00 };
  //                                       0     1     2     3     4     5     6     7     8     9    10

  // Vertical air flow + allergen + clean + 3D
  //MitsubishiHeavyTemplate[5] = powerMode | operatingMode | temperature;

  // Vertical air flow + fan speed
  MitsubishiHeavyTemplate[7] |= fanSpeed;

  // Power state + operating mode + fan speed
  MitsubishiHeavyTemplate[9] |= operatingMode | powerMode | temperature;

	// There is no checksum, but some bytes are inverted
	MitsubishiHeavyTemplate[6] = ~MitsubishiHeavyTemplate[5];
	MitsubishiHeavyTemplate[8] = ~MitsubishiHeavyTemplate[7];
	MitsubishiHeavyTemplate[10] = ~MitsubishiHeavyTemplate[9];

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  IR.mark(MITSUBISHI_AIRCON2_HDR_MARK);
  IR.space(MITSUBISHI_AIRCON2_HDR_SPACE);

  // Data
  for (uint8_t i=0; i<sizeof(MitsubishiHeavyTemplate); i++) {
    IR.sendIRbyte(MitsubishiHeavyTemplate[i], MITSUBISHI_AIRCON2_BIT_MARK, MITSUBISHI_AIRCON2_ZERO_SPACE, MITSUBISHI_AIRCON2_ONE_SPACE);
  }

  // End mark
  IR.mark(MITSUBISHI_AIRCON2_BIT_MARK);
  IR.space(0);
}