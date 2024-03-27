#include "GreeHeatpumpIR.h"

// This is a protected method, i.e. generic Gree instances cannot be created
GreeHeatpumpIR::GreeHeatpumpIR() : HeatpumpIR()
{
}

GreeGenericHeatpumpIR::GreeGenericHeatpumpIR() : GreeHeatpumpIR()
{
  static const char model[] PROGMEM = "gree";
  static const char info[]  PROGMEM = "{\"mdl\":\"gree\",\"dn\":\"Gree\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}

GreeYANHeatpumpIR::GreeYANHeatpumpIR() : GreeHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyan";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyan\",\"dn\":\"Gree YAN\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}

// Support for YAA1FB, FAA1FB1, YB1F2 remotes
GreeYAAHeatpumpIR::GreeYAAHeatpumpIR() : GreeHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyaa";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyaa\",\"dn\":\"Gree YAA\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}

// Support for YAC1FBF remote
GreeYACHeatpumpIR::GreeYACHeatpumpIR() : GreeiFeelHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyac";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyac\",\"dn\":\"Gree YAC\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}

// Support for YT1F remote
GreeYTHeatpumpIR::GreeYTHeatpumpIR() : GreeiFeelHeatpumpIR()
{
  static const char model[] PROGMEM = "greeyt";
  static const char info[]  PROGMEM = "{\"mdl\":\"greeyt\",\"dn\":\"Gree YT\",\"mT\":16,\"xT\":30,\"fs\":3}";

  _model = model;
  _info = info;
}

void GreeHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd, bool turboMode, bool iFeelMode)
{
  // Sensible defaults for the heat pump mode

  uint8_t powerMode = GREE_AIRCON1_POWER_ON;
  uint8_t operatingMode = GREE_AIRCON1_MODE_HEAT;
  uint8_t fanSpeed = GREE_AIRCON1_FAN_AUTO;
  uint8_t temperature = 21;
  uint8_t swingV = GREE_VDIR_AUTO;
  uint8_t swingH = GREE_HDIR_AUTO;


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

  switch (swingVCmd)
  {
    case VDIR_AUTO:
      swingV = GREE_VDIR_AUTO;
      break;
    case VDIR_SWING:
      swingV = GREE_VDIR_SWING;
      break;
    case VDIR_UP:
      swingV = GREE_VDIR_UP;
      break;
    case VDIR_MUP:
      swingV = GREE_VDIR_MUP;
      break;
    case VDIR_MIDDLE:
      swingV = GREE_VDIR_MIDDLE;
      break;
    case VDIR_MDOWN:
      swingV = GREE_VDIR_MDOWN;
      break;
    case VDIR_DOWN:
      swingV = GREE_VDIR_DOWN;
      break;
  }

  switch (swingHCmd)
  {
    case HDIR_AUTO:
      swingH = GREE_HDIR_AUTO;
      break;
    case HDIR_SWING:
      swingH = GREE_HDIR_SWING;
      break;
    case HDIR_LEFT:
      swingH = GREE_HDIR_LEFT;
      break;
    case HDIR_MLEFT:
      swingH = GREE_HDIR_MLEFT;
      break;
    case HDIR_MIDDLE:
      swingH = GREE_HDIR_MIDDLE;
      break;
    case HDIR_MRIGHT:
      swingH = GREE_HDIR_MRIGHT;
      break;
    case HDIR_RIGHT:
      swingH = GREE_HDIR_RIGHT;
      break;
  }

  if (temperatureCmd > 15 && temperatureCmd < 31)
  {
    temperature = temperatureCmd - 16;
  }

  sendGree(IR, powerMode, operatingMode, fanSpeed, temperature, swingV, swingH, turboMode, iFeelMode);
}

void GreeHeatpumpIR::generateCommand(uint8_t * buffer,
            uint8_t powerMode, uint8_t operatingMode,
            uint8_t fanSpeed, uint8_t temperature,
            uint8_t swingV, uint8_t swingH,
            bool turboMode, bool iFeelMode) {

  memset(buffer, 0, 8);

  // Set the Fan speed, operating mode and power state
  buffer[0] = fanSpeed | operatingMode | powerMode;

  // Set the temperature
  buffer[1] = temperature;
}

void GreeYANHeatpumpIR::generateCommand(uint8_t * buffer,
            uint8_t powerMode, uint8_t operatingMode,
            uint8_t fanSpeed, uint8_t temperature,
            uint8_t swingV, uint8_t swingH,
            bool turboMode, bool iFeelMode) {
  GreeHeatpumpIR::generateCommand(buffer,
      powerMode, operatingMode,
      fanSpeed, temperature,
      swingV, swingH,
      turboMode, iFeelMode);

  buffer[2] = turboMode ? 0x70 : 0x60;
  buffer[3] = 0x50;
  if (swingV == GREE_VDIR_SWING)
    swingV = GREE_VDIR_AUTO;
  buffer[4] = swingV;
  buffer[5] |= 0x20;
}

void GreeYAAHeatpumpIR::generateCommand(uint8_t * buffer,
            uint8_t powerMode, uint8_t operatingMode,
            uint8_t fanSpeed, uint8_t temperature,
            uint8_t swingV, uint8_t swingH,
            bool turboMode, bool iFeelMode) {
  GreeHeatpumpIR::generateCommand(buffer,
      powerMode, operatingMode,
      fanSpeed, temperature,
      swingV, swingH,
      turboMode, iFeelMode);

  buffer[2] = GREE_LIGHT_BIT; // bits 0..3 always 0000, bits 4..7 TURBO,LIGHT,HEALTH,X-FAN
  buffer[3] = 0x50; // bits 4..7 always 0101
  buffer[5] |= 0x20;
  buffer[6] = 0x20; // YAA1FB, FAA1FB1, YB1F2 bits 4..7 always 0010

  if (turboMode)
  {
    buffer[2] |= GREE_TURBO_BIT;
  }
  if (swingV == GREE_VDIR_SWING)
  {
    buffer[0] |= GREE_VSWING; // Enable swing by setting bit 6
  }
  else if (swingV != GREE_VDIR_AUTO)
  {
    buffer[5] = swingV;
  }
}

void GreeiFeelHeatpumpIR::generateCommand(uint8_t * buffer,
            uint8_t powerMode, uint8_t operatingMode,
            uint8_t fanSpeed, uint8_t temperature,
            uint8_t swingV, uint8_t swingH,
            bool turboMode, bool iFeelMode) {
  GreeHeatpumpIR::generateCommand(buffer,
      powerMode, operatingMode,
      fanSpeed, temperature,
      swingV, swingH,
      turboMode, iFeelMode);

  if (iFeelMode) {
    buffer[5] |= GREE_IFEEL_BIT;
  }
}

void GreeYACHeatpumpIR::generateCommand(uint8_t * buffer,
            uint8_t powerMode, uint8_t operatingMode,
            uint8_t fanSpeed, uint8_t temperature,
            uint8_t swingV, uint8_t swingH,
            bool turboMode, bool iFeelMode) {
  GreeiFeelHeatpumpIR::generateCommand(buffer,
      powerMode, operatingMode,
      fanSpeed, temperature,
      swingV, swingH,
      turboMode, iFeelMode);

  if (swingH == GREE_HDIR_AUTO)
    swingH = GREE_HDIR_SWING;

  buffer[4] |= (swingH << 4); // GREE_YT will ignore packets where this is set

  buffer[2] = GREE_LIGHT_BIT; // bits 0..3 always 0000, bits 4..7 TURBO,LIGHT,HEALTH,X-FAN
  buffer[3] = 0x50; // bits 4..7 always 0101
  buffer[5] |= 0x20;
  buffer[6] = 0x20; // YAA1FB, FAA1FB1, YB1F2 bits 4..7 always 0010

  if (turboMode)
  {
    buffer[2] |= GREE_TURBO_BIT;
  }
  if (swingV == GREE_VDIR_SWING)
  {
    buffer[0] |= GREE_VSWING; // Enable swing by setting bit 6
  }
  else if (swingV != GREE_VDIR_AUTO)
  {
    buffer[5] = swingV;
  }
}

void GreeYTHeatpumpIR::generateCommand(uint8_t * buffer,
            uint8_t powerMode, uint8_t operatingMode,
            uint8_t fanSpeed, uint8_t temperature,
            uint8_t swingV, uint8_t swingH,
            bool turboMode, bool iFeelMode) {
  GreeiFeelHeatpumpIR::generateCommand(buffer,
      powerMode, operatingMode,
      fanSpeed, temperature,
      swingV, swingH,
      turboMode, iFeelMode);

  buffer[2] = GREE_LIGHT_BIT | GREE_HEALTH_BIT; // HEALTH is always on for GREE_YT
  buffer[3] = 0x50; // bits 4..7 always 0101

  if (turboMode)
  {
    buffer[2] |= GREE_TURBO_BIT;
  }
  if (swingV == GREE_VDIR_SWING)
  {
    buffer[0] |= GREE_VSWING; // Enable swing by setting bit 6
    buffer[4] = swingV;
  }
}

void GreeHeatpumpIR::calculateChecksum(uint8_t * buffer) {
  buffer[7] = (((
   (buffer[0] & 0x0F) +
   (buffer[1] & 0x0F) +
   (buffer[2] & 0x0F) +
   (buffer[3] & 0x0F) +
   ((buffer[5] & 0xF0) >> 4) +
   ((buffer[6] & 0xF0) >> 4) +
   ((buffer[7] & 0xF0) >> 4) +
    0x0A) & 0x0F) << 4) | (buffer[7] & 0x0F);
}

void GreeYANHeatpumpIR::calculateChecksum(uint8_t * buffer) {
  buffer[7] = (
    (buffer[0] << 4) +
    (buffer[1] << 4) +
    0xC0);
}

// Send the Gree code
void GreeHeatpumpIR::sendGree(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingV, uint8_t swingH, bool turboMode, bool iFeelMode)
{
  uint8_t GreeTemplate[8];

  generateCommand(
      GreeTemplate,
      powerMode, operatingMode,
      fanSpeed, temperature,
      swingV, swingH,
      turboMode, iFeelMode);

  calculateChecksum(GreeTemplate);

  const auto & timings = getTimings();

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Send Header mark
  IR.mark(timings.hdr_mark);
  IR.space(timings.hdr_space);

  // Payload part #1
  for (int i=0; i<4; i++) {
    IR.sendIRbyte(GreeTemplate[i], timings.bit_mark, timings.zero_space, timings.one_space);
  }
  // Only three first bits of byte 4 are sent, this is always '010'
  IR.mark(timings.bit_mark);
  IR.space(timings.zero_space);
  IR.mark(timings.bit_mark);
  IR.space(timings.one_space);
  IR.mark(timings.bit_mark);
  IR.space(timings.zero_space);

  // Message space
  IR.mark(timings.bit_mark);
  IR.space(timings.msg_space);

  // Payload part #2
  for (int i=4; i<8; i++) {
    IR.sendIRbyte(GreeTemplate[i], timings.bit_mark, timings.zero_space, timings.one_space);
  }

  // End mark
  IR.mark(timings.bit_mark);
  IR.space(0);
}

// Sends current sensed temperatures, YAC remotes/supporting units only
void GreeiFeelHeatpumpIR::send(IRSender& IR, uint8_t currentTemperature)
{
  uint8_t GreeTemplate[] = { 0x00, 0x00 };

  GreeTemplate[0] = currentTemperature;
  GreeTemplate[1] = 0xA5;

  const auto & timings = getTimings();

  // 38 kHz PWM frequency
  IR.setFrequency(38);

  // Send Header mark
  IR.mark(timings.ifeel_hdr_mark);
  IR.space(timings.ifeel_hdr_space);

  // send payload
  IR.sendIRbyte(GreeTemplate[0], timings.ifeel_bit_mark, timings.zero_space, timings.one_space);
  IR.sendIRbyte(GreeTemplate[1], timings.ifeel_bit_mark, timings.zero_space, timings.one_space);

  // End mark
  IR.mark(timings.ifeel_bit_mark);
  IR.space(0);
}
