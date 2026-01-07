#include <OlimpiaStandardMaestroHeatpumpIR.h>

// IR timing parameters
const uint16_t HEADER_MARK_LENGTH = 3200;
const uint16_t HEADER_SPACE_LENGTH = 1700;
const uint16_t BIT_MARK_LENGTH = 420;
const uint16_t ZERO_SPACE_LENGTH = 420;
const uint16_t ONE_SPACE_LENGTH = 1280;

#define NUM_BYTES 11  // Total command is 88 bits (11 bytes)

OlimpiaStandardMaestroHeatpumpIR::OlimpiaStandardMaestroHeatpumpIR() {
  static const char model[] PROGMEM = "olimpia";
  static const char info[] PROGMEM = 
  "{\"mdl\":\"olimpia\",\"dn\":\"Olimpia Splendid Maestro\","
    "\"mT\":15,\"xT\":30,"
    "\"fs\":4,"
    "\"modes\":[\"off\",\"cool\",\"heat\",\"dry\",\"fan_only\",\"auto\"],"
    "\"modeTemp\":{\"cool\":{\"min\":18,\"max\":30},\"heat\":{\"min\":15,\"max\":30}},"
    "\"features\":[\"eco\",\"lowNoise\",\"swing\",\"fahrenheit\"]}";
  _model = model;
  _info = info;
}

void OlimpiaStandardMaestroHeatpumpIR::send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd) {
  HeatpumpState state;
  state.power = (powerModeCmd == POWER_ON);
  state.mode = operatingModeCmd;
  state.fanSpeed = fanSpeedCmd;
  state.tempC = temperatureCmd;
  state.flapSwing = (swingVCmd == VDIR_SWING);
  state.eco = false; // Assuming eco mode is not controlled by this function
  state.lowNoise = (fanSpeedCmd == FAN_SILENT);
  state.isFahrenheit = false; // Assuming temperature is in Celsius
  state.displayHide = false; // Default

  sendMaestro(IR, state);
}

void OlimpiaStandardMaestroHeatpumpIR::send(IRSender& IR, uint8_t currentTemperature) {
  HeatpumpState state;
  state.power = true;
  state.mode = MODE_COOL; // assume cool
  state.fanSpeed = FAN_AUTO;
  state.tempC = currentTemperature;
  state.flapSwing = false;
  state.eco = false; 
  state.lowNoise = false;
  state.isFahrenheit = false; 
  state.displayHide = false; 

  sendMaestro(IR, state);
}

/*
  encodeMaestroState()

  Builds an 11-byte Maestro command directly in LSB-first order.
  Mapping (each byte is interpreted LSB-first by IRSender::sendIRbyte):

    - Byte 1 (index 1):
        * Bits 0-2: 3-bit power/mode field.
          - If power is off, the field is 0.
          - If power is on:
              - If (state.mode == MODE_COOL)  → encoded value = 0b001
              - If (state.mode == MODE_HEAT)  → encoded value = 0b010
              - If (state.mode == MODE_DRY)   → encoded value = 0b011
              - If (state.mode == MODE_FAN)   → encoded value = 0b100
              - If (state.mode == MODE_AUTO)  → encoded value = 0b101
              - Otherwise, default to 0.
        * Bits 3-4: 2-bit fan speed.
          - If (state.fanSpeed == FAN_1)     → 0b00
          - If (state.fanSpeed == FAN_2)  → 0b01
          - If (state.fanSpeed == FAN_3)    → 0b10
          - If (state.fanSpeed == FAN_AUTO)    → 0b11
          - Otherwise, default to 0.
        * Bit 5: flap swing flag.
        * Bit 6: low noise flag.
        * Bit 7: unused (0).

    - Byte 7 (index 7):
        * Eco mode is stored in bit 2.

    - Byte 9 (index 9): Temperature.
        * Bits 0-4: raw temperature value calculated as: raw = 2 * (tempC - 15)
        * Bit 5: Fahrenheit flag.
        * Bits 6-7: unused.
        
    - Byte 10 (index 10): Checksum
        * SUM of all previous bytes % 256
*/
void encodeMaestroState(const HeatpumpState &state, uint8_t outBytes[NUM_BYTES]) {
  // Initialize outBytes to 0.
  for (int i = 0; i < NUM_BYTES; i++) {
    outBytes[i] = 0;
  }

  // --- Byte 0 (index 0): Identifier ---
  outBytes[0] = 0x5A;
  
  // --- Byte 1 (index 1) ---
  uint8_t encodedMode = 0;
  if (state.power) {
    if (state.mode == MODE_COOL) {
      encodedMode = 0b001;
    } else if (state.mode == MODE_HEAT) {
      encodedMode = 0b010;
    } else if (state.mode == MODE_DRY) {
      encodedMode = 0b011;
    } else if (state.mode == MODE_FAN) {
      encodedMode = 0b100;
    } else if (state.mode == MODE_AUTO) {
      encodedMode = 0b101;
    }
  } // If power is off, encodedMode remains 0.
  
  uint8_t encodedFanSpeed = 0;
  if (state.fanSpeed == FAN_1) {
    encodedFanSpeed = 0b00;
  } else if (state.fanSpeed == FAN_2) {
    encodedFanSpeed = 0b01;
  } else if (state.fanSpeed == FAN_3) {
    encodedFanSpeed = 0b10;
  } else if (state.fanSpeed == FAN_AUTO) {
    encodedFanSpeed = 0b11;
  }
  
  uint8_t flapSwing = state.flapSwing ? 1 : 0;
  uint8_t lowNoise  = state.lowNoise ? 1 : 0;
  uint8_t displayHide = state.displayHide ? 1 : 0;
  
  // Construct Byte 1 directly in outBytes (LSB-first order):
  // Bits 0-2: encodedMode, Bits 3-4: encodedFanSpeed, Bit 5: flapSwing, Bit 6: lowNoise, Bit 7: displayHide
  outBytes[1] = (encodedMode) | (encodedFanSpeed << 3) | (flapSwing << 5) | (lowNoise << 6) | (displayHide << 7);
  
  // --- Byte 7 (index 7) ---
  // Set eco mode in bit 2.
  if (state.eco) {
    outBytes[7] |= (1 << 2);
  }
  
  // --- Byte 9 (index 9): Temperature ---
  // Calculate raw temperature value: raw = 2 * (tempC - 15)
  uint8_t rawTemp = 2 * (state.tempC - 15);
  uint8_t fahrenheitFlag = state.isFahrenheit ? 1 : 0;
  // Bits 0-4: rawTemp, Bit 5: Fahrenheit flag.
  outBytes[9] = (rawTemp & 0x1F) | (fahrenheitFlag << 5);

  // Checksum calculation
  uint8_t checksum = 0;
  for (int i = 0; i < 10; i++) {
    checksum += outBytes[i];  // wraps automatically at 256
  }
  outBytes[10] = checksum;

}

/*
  sendMaestro()

  Encodes the given HeatpumpState into an 11-byte Maestro IR command and sends it via the provided
  IRSender instance. The IRSender::sendIRbyte function transmits the bits in LSB-first order.
*/
void OlimpiaStandardMaestroHeatpumpIR::sendMaestro(IRSender &IR, const HeatpumpState &state) {
  uint8_t command[NUM_BYTES];
  encodeMaestroState(state, command);
  

  LOGLN(F("Starting to send IR"));
  // Set the frequency for the IR signal
  IR.setFrequency(38); // 38kHz frequency

  // Header
  IR.mark(HEADER_MARK_LENGTH);
  IR.space(HEADER_SPACE_LENGTH);

  uint8_t bitCount = 8;  // Each byte is 8 bits.  
  for (int i = 0; i < NUM_BYTES; i++) {
    IR.sendIRbyte(command[i], BIT_MARK_LENGTH, ZERO_SPACE_LENGTH, ONE_SPACE_LENGTH, bitCount);
  }

  // Stop bit
  IR.mark(BIT_MARK_LENGTH);
  IR.space(0); // Ensure the IR LED is turned off

}