/*
    Midea MSR1-12HRN1-QC2 + MOA1-12HN1-QC2 heatpump control (remote control P/N RG51M1/E)
    This heatpump is sold as 'Ultimate Pro Plus 13FP' in Finland, by www.ultimatemarket.com
*/
#ifndef MideaHeatpumpIR_h
#define MideaHeatpumpIR_h

#include <Arduino.h>
#include <IRSender.h>
#include <HeatpumpIR.h>

// Midea timing constants, Midea MSR1-12HRN1-QC2 + MOA1-12HN1-QC2, sold as Ultimate Pro Plus Basic 13FP in Finland (remote control P/N RG51M1/E)
#define MIDEA_AIRCON1_HDR_MARK       4350
#define MIDEA_AIRCON1_HDR_SPACE      4230
#define MIDEA_AIRCON1_BIT_MARK       520
#define MIDEA_AIRCON1_ONE_SPACE      1650
#define MIDEA_AIRCON1_ZERO_SPACE     550
#define MIDEA_AIRCON1_MSG_SPACE      5100

// MIDEA codes
#define MIDEA_AIRCON1_MODE_AUTO      0x10 // Operating mode
#define MIDEA_AIRCON1_MODE_HEAT      0x30
#define MIDEA_AIRCON1_MODE_COOL      0x00
#define MIDEA_AIRCON1_MODE_DRY       0x20
#define MIDEA_AIRCON1_MODE_FAN       0x60
#define MIDEA_AIRCON1_MODE_FP        0x70 // Not a real mode...
#define MIDEA_AIRCON1_MODE_OFF       0xFE // Power OFF - not real codes, but we need something...
#define MIDEA_AIRCON1_MODE_ON        0xFF // Power ON
#define MIDEA_AIRCON1_FAN_AUTO       0x02 // Fan speed
#define MIDEA_AIRCON1_FAN1           0x06
#define MIDEA_AIRCON1_FAN2           0x05
#define MIDEA_AIRCON1_FAN3           0x03


class MideaHeatpumpIR : public HeatpumpIR
{
  public:
    MideaHeatpumpIR();
    MideaHeatpumpIR(const prog_char* model);
    MideaHeatpumpIR(const prog_char* model, const prog_char* info);
	void send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd);

  private:
    void sendMidea(IRSender& IR, byte operatingMode, byte fanSpeed, byte temperature);
    void sendMidearaw(IRSender& IR, byte sendBuffer[]);
};

#endif
