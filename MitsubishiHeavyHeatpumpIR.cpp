#include <MitsubishiHeavyHeatpumpIR.h>

// These are protected methods, i.e. generic MitsubishiHeavy instances cannot be created directly
MitsubishiHeavyHeatpumpIR::MitsubishiHeavyHeatpumpIR() : HeatpumpIR()
{
}


// The different models just set the model accordingly

MitsubishiHeavyZJHeatpumpIR::MitsubishiHeavyZJHeatpumpIR() : MitsubishiHeavyHeatpumpIR()
{
  static const char PROGMEM model[] PROGMEM = "mitsubishi_heavy_zj";
  static const char PROGMEM info[]  PROGMEM = "{\"mdl\":\"mitsubishi_heavy_zj\",\"dn\":\"Mitsubishi Heavy ZJ\",\"mT\":18,\"xT\":30,\"fs\":5}"; ///////

  _model = model;
  _info = info;

  _mitsubishiModel = MITSUBISHIHEAVY_ZJ;
}

MitsubishiHeavyZMHeatpumpIR::MitsubishiHeavyZMHeatpumpIR() : MitsubishiHeavyHeatpumpIR()
{
  static const char PROGMEM model[] PROGMEM = "mitsubishi_heavy_zm";
  static const char PROGMEM info[]  PROGMEM = "{\"mdl\":\"mitsubishi_heavy_zm\",\"dn\":\"Mitsubishi Heavy ZM\",\"mT\":18,\"xT\":30,\"fs\":5}"; ///////

  _model = model;
  _info = info;

  _mitsubishiModel = MITSUBISHIHEAVY_ZM;
}


void MitsubishiHeavyHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd)
{
  send(IR, powerModeCmd, operatingModeCmd, fanSpeedCmd, temperatureCmd, swingVCmd, swingHCmd, true);
}


void MitsubishiHeavyHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd, bool cleanModeCmd)
{
  // Sensible defaults for the heat pump mode

  uint8_t powerMode     = MITSUBISHI_AIRCON2_MODE_ON;
  uint8_t operatingMode = MITSUBISHI_AIRCON2_MODE_HEAT;
  uint8_t fanSpeed      = MITSUBISHI_AIRCON2_FAN_AUTO;
  uint8_t temperature   = 23;
  uint8_t swingV        = MITSUBISHI_AIRCON2_VS_AUTO;
  uint8_t swingH        = MITSUBISHI_AIRCON2_HS_AUTO;
  uint8_t cleanMode     = MITSUBISHI_AIRCON2_CLEAN_OFF;

  if (powerModeCmd == POWER_OFF)
  {
    powerMode = MITSUBISHI_AIRCON2_MODE_OFF;
  }

  if ( _mitsubishiModel == MITSUBISHIHEAVY_ZM ) {
    cleanMode = MITSUBISHI_AIRCON2_CLN_OFF_ZM;
  }

  if (cleanModeCmd == true && powerModeCmd == POWER_OFF && (operatingModeCmd == MODE_AUTO || operatingModeCmd == MODE_COOL || operatingModeCmd == MODE_DRY))
  {
    powerMode = MITSUBISHI_AIRCON2_MODE_ON;
    cleanMode = MITSUBISHI_AIRCON2_CLEAN_ON;
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
    if ( _mitsubishiModel == MITSUBISHIHEAVY_ZJ ) {
      temperature = (~((temperatureCmd - 17) << 4)) & 0xF0;
    } else {
      temperature = (~(temperatureCmd - 17) & 0x0F);
    }
  }

  switch (swingVCmd)
  {
    case VDIR_MANUAL:
      swingV = MITSUBISHI_AIRCON2_VS_STOP;
      break;
    case VDIR_SWING:
      swingV = MITSUBISHI_AIRCON2_VS_AUTO;
      break;
    case VDIR_UP:
      swingV = MITSUBISHI_AIRCON2_VS_UP;
      break;
    case VDIR_MUP:
      swingV = MITSUBISHI_AIRCON2_VS_MUP;
      break;
    case VDIR_MIDDLE:
      swingV = MITSUBISHI_AIRCON2_VS_MIDDLE;
      break;
    case VDIR_MDOWN:
      swingV = MITSUBISHI_AIRCON2_VS_MDOWN;
      break;
    case VDIR_DOWN:
      swingV = MITSUBISHI_AIRCON2_VS_DOWN;
      break;
  }

  switch (swingHCmd)
  {
    case HDIR_MANUAL:
      swingH = MITSUBISHI_AIRCON2_HS_STOP;
      break;
    case HDIR_SWING:
      swingH = MITSUBISHI_AIRCON2_HS_AUTO;
      break;
    case HDIR_MIDDLE:
      swingH = MITSUBISHI_AIRCON2_HS_MIDDLE;
      break;
    case HDIR_LEFT:
      swingH = MITSUBISHI_AIRCON2_HS_LEFT;
      break;
    case HDIR_MLEFT:
      swingH = MITSUBISHI_AIRCON2_HS_MLEFT;
      break;
    case HDIR_RIGHT:
      swingH = MITSUBISHI_AIRCON2_HS_RIGHT;
      break;
    case HDIR_MRIGHT:
      swingH = MITSUBISHI_AIRCON2_HS_MRIGHT;
      break;
  }

  if ( _mitsubishiModel == MITSUBISHIHEAVY_ZJ ) {
    sendMitsubishiHeavyZJ(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH, cleanMode);
  } else {
    sendMitsubishiHeavyZM(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH, cleanMode);
  }
}


void MitsubishiHeavyHeatpumpIR::sendMitsubishiHeavyZJ(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH, uint8_t cleanMode)
{
  uint8_t MitsubishiHeavyZJTemplate[] = { 0x52, 0xAE, 0xC3, 0x26, 0xD9, 0x11, 0x00, 0x07, 0x00, 0x00, 0x00 };
  //                                         0     1     2     3     4     5     6     7     8     9    10

  // Horizontal & vertical air flow + allergen + clean + 3D
  MitsubishiHeavyZJTemplate[5] |= swingH | (swingV & 0b00000010) | cleanMode;

  // Vertical air flow + fan speed
  MitsubishiHeavyZJTemplate[7] |= fanSpeed | (swingV & 0b00011000);

  // Power state + operating mode + temperature
  MitsubishiHeavyZJTemplate[9] |= operatingMode | powerMode | temperature;

	// There is no checksum, but some bytes are inverted
	MitsubishiHeavyZJTemplate[6] = ~MitsubishiHeavyZJTemplate[5];
	MitsubishiHeavyZJTemplate[8] = ~MitsubishiHeavyZJTemplate[7];
	MitsubishiHeavyZJTemplate[10] = ~MitsubishiHeavyZJTemplate[9];

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  IR.mark(MITSUBISHI_AIRCON2_HDR_MARK);
  IR.space(MITSUBISHI_AIRCON2_HDR_SPACE);

  // Data
  for (uint8_t i=0; i<sizeof(MitsubishiHeavyZJTemplate); i++) {
    IR.sendIRbyte(MitsubishiHeavyZJTemplate[i], MITSUBISHI_AIRCON2_BIT_MARK, MITSUBISHI_AIRCON2_ZERO_SPACE, MITSUBISHI_AIRCON2_ONE_SPACE);
  }

  // End mark
  IR.mark(MITSUBISHI_AIRCON2_BIT_MARK);
  IR.space(0);
}


void MitsubishiHeavyHeatpumpIR::sendMitsubishiHeavyZM(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH, uint8_t cleanMode)
{
  uint8_t MitsubishiHeavyZMTemplate[] = { 0x52, 0xAE, 0xC3, 0x1A, 0xE5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x7B, 0x00 };
  //                                         0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15    16    17    18

  // Power state + operating mode
  MitsubishiHeavyZMTemplate[5] |= operatingMode | powerMode | cleanMode;

  // Temperature
  MitsubishiHeavyZMTemplate[7] |= temperature;

  // Fan speed
  MitsubishiHeavyZMTemplate[9] |= fanSpeed;

  // Vertical air flow
  MitsubishiHeavyZMTemplate[11] |= swingH | (swingV & 0b00000010);

  // Horizontal air flow
  MitsubishiHeavyZMTemplate[13] |= (swingV & 0b00011000);

	// There is no checksum, but some bytes are inverted
	MitsubishiHeavyZMTemplate[6] = ~MitsubishiHeavyZMTemplate[5];
	MitsubishiHeavyZMTemplate[8] = ~MitsubishiHeavyZMTemplate[7];
	MitsubishiHeavyZMTemplate[10] = ~MitsubishiHeavyZMTemplate[9];
	MitsubishiHeavyZMTemplate[12] = ~MitsubishiHeavyZMTemplate[11];
	MitsubishiHeavyZMTemplate[14] = ~MitsubishiHeavyZMTemplate[13];
	MitsubishiHeavyZMTemplate[16] = ~MitsubishiHeavyZMTemplate[15];
	MitsubishiHeavyZMTemplate[18] = ~MitsubishiHeavyZMTemplate[17];

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Header
  IR.mark(MITSUBISHI_AIRCON2_HDR_MARK);
  IR.space(MITSUBISHI_AIRCON2_HDR_SPACE);

  // Data
  for (uint8_t i=0; i<sizeof(MitsubishiHeavyZMTemplate); i++) {
    IR.sendIRbyte(MitsubishiHeavyZMTemplate[i], MITSUBISHI_AIRCON2_BIT_MARK, MITSUBISHI_AIRCON2_ZERO_SPACE, MITSUBISHI_AIRCON2_ONE_SPACE);
  }

  // End mark
  IR.mark(MITSUBISHI_AIRCON2_BIT_MARK);
  IR.space(0);
}