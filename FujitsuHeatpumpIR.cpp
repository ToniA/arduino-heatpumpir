#include <Arduino.h>
#include <FujitsuHeatpumpIR.h>

FujitsuHeatpumpIR::FujitsuHeatpumpIR()
{
}

FujitsuHeatpumpIR::FujitsuHeatpumpIR(const prog_char* model) : HeatpumpIR(model)
{
}

FujitsuHeatpumpIR::FujitsuHeatpumpIR(const prog_char* model, const prog_char* info) : HeatpumpIR(model, info)
{
}


void FujitsuHeatpumpIR::send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd)
{
  // Sensible defaults for the heat pump mode

  byte operatingMode = FUJITSU_AIRCON1_MODE_HEAT;
  byte fanSpeed = FUJITSU_AIRCON1_FAN_AUTO;
  byte temperature = 23;

  if (powerModeCmd == POWER_OFF)
  {
    operatingMode = FUJITSU_AIRCON1_MODE_OFF;
  }
  else
  {
    switch (operatingModeCmd)
    {
      case MODE_AUTO:
        operatingMode = FUJITSU_AIRCON1_MODE_AUTO;
        break;
      case MODE_HEAT:
        operatingMode = FUJITSU_AIRCON1_MODE_HEAT;
        break;
      case MODE_COOL:
        operatingMode = FUJITSU_AIRCON1_MODE_COOL;
        break;
      case MODE_DRY:
        operatingMode = FUJITSU_AIRCON1_MODE_DRY;
        break;
      case MODE_FAN:
        operatingMode = FUJITSU_AIRCON1_MODE_FAN;
        // When Fujitsu goes to FAN mode, it sets the low bit of the byte with the temperature. What is the meaning of that?
       break;
    }
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = FUJITSU_AIRCON1_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = FUJITSU_AIRCON1_FAN1;
      break;
    case FAN_2:
      fanSpeed = FUJITSU_AIRCON1_FAN2;
      break;
    case FAN_3:
      fanSpeed = FUJITSU_AIRCON1_FAN3;
      break;
    case FAN_4:
      fanSpeed = FUJITSU_AIRCON1_FAN4;
      break;
  }

  if ( temperatureCmd > 15 && temperatureCmd < 31)
  {
    temperature = temperatureCmd;
  }

  sendFujitsu(IR, operatingMode, fanSpeed, temperature);
}

void FujitsuHeatpumpIR::sendFujitsu(IRSender& IR, byte operatingMode, byte fanSpeed, byte temperature)
{
  // ON, HEAT, AUTO FAN, +24 degrees
  byte FujitsuTemplate[] = { 0x14, 0x63, 0x00, 0x10, 0x10, 0xFE, 0x09, 0x30, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00 };
  //                            0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15

  static const prog_uint8_t OFF_msg[] PROGMEM = { 0x14, 0x63, 0x00, 0x10, 0x10, 0x02, 0xFD };
  byte checksum = 0x00;

  // Set the operatingmode on the template message
  FujitsuTemplate[9] = operatingMode;

  // Set the temperature on the template message
  FujitsuTemplate[8] = (temperature - 16) << 4;

  // Set the fan speed on the template message
  FujitsuTemplate[10] = fanSpeed;

  // Calculate the checksum
  for (int i=0; i<15; i++) {
    checksum += FujitsuTemplate[i];
  }

  FujitsuTemplate[15] = (byte)(0x9E - checksum);

  // 40 kHz PWM frequency
  IR.setFrequency(40);

  // Header
  IR.mark(FUJITSU_AIRCON1_HDR_MARK);
  IR.space(FUJITSU_AIRCON1_HDR_SPACE);

  if (operatingMode == FUJITSU_AIRCON1_MODE_OFF) {
    // OFF
    for (int i=0; i<sizeof(OFF_msg); i++) {
      IR.sendIRByte(OFF_msg[i], FUJITSU_AIRCON1_BIT_MARK, FUJITSU_AIRCON1_ZERO_SPACE, FUJITSU_AIRCON1_ONE_SPACE);
    }
  } else {
    // Data
    for (int i=0; i<sizeof(FujitsuTemplate); i++) {
      IR.sendIRByte(FujitsuTemplate[i], FUJITSU_AIRCON1_BIT_MARK, FUJITSU_AIRCON1_ZERO_SPACE, FUJITSU_AIRCON1_ONE_SPACE);
    }
  }

  // End mark
  IR.mark(FUJITSU_AIRCON1_BIT_MARK);
  IR.space(0);
}