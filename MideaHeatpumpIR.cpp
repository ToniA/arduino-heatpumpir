#include <Arduino.h>
#include <MideaHeatpumpIR.h>

MideaHeatpumpIR::MideaHeatpumpIR()
{
}

MideaHeatpumpIR::MideaHeatpumpIR(const prog_char* model) : HeatpumpIR(model)
{
}

MideaHeatpumpIR::MideaHeatpumpIR(const prog_char* model, const prog_char* info) : HeatpumpIR(model, info)
{
}


void MideaHeatpumpIR::send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd)
{
  // Sensible defaults for the heat pump mode
  byte operatingMode = MIDEA_AIRCON1_MODE_HEAT;
  byte fanSpeed = MIDEA_AIRCON1_FAN_AUTO;
  byte temperature = 23;

  switch (powerModeCmd)
  {
    case 0:
      // OFF is a special case
      operatingMode = MIDEA_AIRCON1_MODE_OFF;
      sendMidea(IR, operatingMode, fanSpeed, temperature);
      return;
  }

  switch (operatingModeCmd)
  {
    case MODE_AUTO:
      operatingMode = MIDEA_AIRCON1_MODE_AUTO;
      break;
    case MODE_HEAT:
      operatingMode = MIDEA_AIRCON1_MODE_HEAT;
      break;
    case MODE_COOL:
      operatingMode = MIDEA_AIRCON1_MODE_COOL;
      break;
    case MODE_DRY:
      operatingMode = MIDEA_AIRCON1_MODE_DRY;
      break;
    case MODE_FAN:
      operatingMode = MIDEA_AIRCON1_MODE_FAN;
      break;
    case MODE_MAINT:
      // Maintenance mode ('FP' on the remote) is a special mode on Midea
      operatingMode = MIDEA_AIRCON1_MODE_FP;
      sendMidea(IR, operatingMode, fanSpeed, temperature);
      return;
  }

  switch (fanSpeedCmd)
  {
    case FAN_AUTO:
      fanSpeed = MIDEA_AIRCON1_FAN_AUTO;
      break;
    case FAN_1:
      fanSpeed = MIDEA_AIRCON1_FAN1;
      break;
    case FAN_2:
      fanSpeed = MIDEA_AIRCON1_FAN2;
      break;
    case FAN_3:
      fanSpeed = MIDEA_AIRCON1_FAN3;
      break;
  }

  if ( temperatureCmd > 15 && temperatureCmd < 31)
  {
    temperature = temperatureCmd;
  }

  sendMidea(IR, operatingMode, fanSpeed, temperature);
}

// Send the Midea code
void MideaHeatpumpIR::sendMidea(IRSender& IR, byte operatingMode, byte fanSpeed, byte temperature)
{
  byte sendBuffer[3] = { 0x4D, 0x00, 0x00 }; // First byte is always 0x4D

  static const prog_uint8_t temperatures[] PROGMEM = {0, 8, 12, 4, 6, 14, 10, 2, 3, 11, 9, 1, 5, 13 };

  static const prog_uint8_t OffMsg[] PROGMEM = {0x4D, 0xDE, 0x07 };
  static const prog_uint8_t FPMsg[] PROGMEM =  {0xAD, 0xAF, 0xB5 };

  if (operatingMode == MIDEA_AIRCON1_MODE_OFF)
  {
    memcpy_P(sendBuffer, OffMsg, sizeof(sendBuffer));
  }
  else if (operatingMode == MIDEA_AIRCON1_MODE_FP)
  {
    memcpy_P(sendBuffer, FPMsg, sizeof(sendBuffer));
  }
  else
  {
    sendBuffer[1] = ~fanSpeed;

    if ( operatingMode == MIDEA_AIRCON1_MODE_FAN )
    {
      sendBuffer[2] = MIDEA_AIRCON1_MODE_DRY | 0x07;
    }
    else
    {
      sendBuffer[2] = operatingMode | temperatures[temperature-17];
    }
  }

  // Send the code
  sendMidearaw(IR, sendBuffer);
}

// Send the Midea raw code
void MideaHeatpumpIR::sendMidearaw(IRSender& IR, byte sendBuffer[])
{
  // 40 kHz PWM frequency
  IR.setFrequency(40);

  // Header
  IR.mark(MIDEA_AIRCON1_HDR_MARK);
  IR.space(MIDEA_AIRCON1_HDR_SPACE);

  // Six bytes, every second byte is a bitwise not of the previous byte
  for (int i=0; i<3; i++) {
    IR.sendIRByte(sendBuffer[i], MIDEA_AIRCON1_BIT_MARK, MIDEA_AIRCON1_ZERO_SPACE, MIDEA_AIRCON1_ONE_SPACE);
    IR.sendIRByte(~sendBuffer[i], MIDEA_AIRCON1_BIT_MARK, MIDEA_AIRCON1_ZERO_SPACE, MIDEA_AIRCON1_ONE_SPACE);
  }

  // Pause
  IR.mark(MIDEA_AIRCON1_BIT_MARK);
  IR.space(MIDEA_AIRCON1_MSG_SPACE);

  // Header, two last bytes repeated
  IR.mark(MIDEA_AIRCON1_HDR_MARK);
  IR.space(MIDEA_AIRCON1_HDR_SPACE);

  // Six bytes, every second byte is a bitwise not of the previous byte
  for (int i=0; i<3; i++) {
    IR.sendIRByte(sendBuffer[i], MIDEA_AIRCON1_BIT_MARK, MIDEA_AIRCON1_ZERO_SPACE, MIDEA_AIRCON1_ONE_SPACE);
    IR.sendIRByte(~sendBuffer[i], MIDEA_AIRCON1_BIT_MARK, MIDEA_AIRCON1_ZERO_SPACE, MIDEA_AIRCON1_ONE_SPACE);
  }

  // End mark
  IR.mark(MIDEA_AIRCON1_BIT_MARK);
  IR.space(0);
}