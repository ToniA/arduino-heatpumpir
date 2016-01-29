/*
    Mitsubishi Heavy SRKxxZJ-S heatpump control (remote control P/N RKX502A001C)
*/
#ifndef MitsubishiHeavyHeatpumpIR_h
#define MitsubishiHeavyHeatpumpIR_h

#include <HeatpumpIR.h>


// Mitsubishi Heavy timing constants
#define MITSUBISHI_AIRCON2_HDR_MARK   3200 // 3200
#define MITSUBISHI_AIRCON2_HDR_SPACE  1600 // 1600
#define MITSUBISHI_AIRCON2_BIT_MARK   400  // 400
#define MITSUBISHI_AIRCON2_ONE_SPACE  1200 // 1200
#define MITSUBISHI_AIRCON2_ZERO_SPACE 400  // 400


// Mitsubishi Heavy codes
#define MITSUBISHI_AIRCON2_MODE_AUTO  0x07 // Operating mode
#define MITSUBISHI_AIRCON2_MODE_HEAT  0x03
#define MITSUBISHI_AIRCON2_MODE_COOL  0x06
#define MITSUBISHI_AIRCON2_MODE_DRY   0x05
#define MITSUBISHI_AIRCON2_MODE_FAN   0x04
#define MITSUBISHI_AIRCON2_MODE_OFF   0x08 // Power OFF
#define MITSUBISHI_AIRCON2_MODE_ON    0x00 // Power ON
#define MITSUBISHI_AIRCON2_FAN_AUTO   0xE0 // Fan speed
#define MITSUBISHI_AIRCON2_FAN1       0xA0
#define MITSUBISHI_AIRCON2_FAN2       0x80
#define MITSUBISHI_AIRCON2_FAN3       0x60
#define MITSUBISHI_AIRCON2_FAN4       0x20 // High speed
#define MITSUBISHI_AIRCON2_FAN5       0x00 // Silent

#define MITSUBISHI_AIRCON2_CLEAN_ON   0x00
#define MITSUBISHI_AIRCON2_CLEAN_OFF  0x20
#define MITSUBISHI_AIRCON2_CLN_OFF_ZM 0x60

#define MITSUBISHI_AIRCON2_VS_AUTO    0x0A // Vertical swing
#define MITSUBISHI_AIRCON2_VS_UP      0x02
#define MITSUBISHI_AIRCON2_VS_MUP     0x18
#define MITSUBISHI_AIRCON2_VS_MIDDLE  0x10
#define MITSUBISHI_AIRCON2_VS_MDOWN   0x08
#define MITSUBISHI_AIRCON2_VS_DOWN    0x00
#define MITSUBISHI_AIRCON2_VS_STOP    0x1A
#define MITSUBISHI_AIRCON2_HS_AUTO    0x4C // Horizontal swing - 3D AUTO
#define MITSUBISHI_AIRCON2_HS_MIDDLE  0x48
#define MITSUBISHI_AIRCON2_HS_LEFT    0xC8
#define MITSUBISHI_AIRCON2_HS_MLEFT   0x88
#define MITSUBISHI_AIRCON2_HS_MRIGHT  0x08
#define MITSUBISHI_AIRCON2_HS_RIGHT   0xC4
#define MITSUBISHI_AIRCON2_HS_STOP    0xCC
#define MITSUBISHI_AIRCON2_HS_LEFTRIGHT   0x84
#define MITSUBISHI_AIRCON2_HS_RIGHTLEFT   0x44
#define MITSUBISHI_AIRCON2_HS_3DAUTO  0x04


// MitsubishiHeavy model codes
#define MITSUBISHIHEAVY_ZJ 0
#define MITSUBISHIHEAVY_ZM 1


class MitsubishiHeavyHeatpumpIR : public HeatpumpIR
{
  protected: // Cannot create generic MitsubishiHeavy heatpump instances
    MitsubishiHeavyHeatpumpIR();
    uint8_t _mitsubishiModel;  // Tells whether this is ZJ or ZM (or other supported model...)

  public:
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd);
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd, bool cleanModeCmd);

  private:
    void sendMitsubishiHeavyZJ(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingVCmd, uint8_t swingHCmd, uint8_t cleanMode);
    void sendMitsubishiHeavyZM(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingVCmd, uint8_t swingHCmd, uint8_t cleanMode);
};

class MitsubishiHeavyZJHeatpumpIR : public MitsubishiHeavyHeatpumpIR
{
  public:
    MitsubishiHeavyZJHeatpumpIR();
};

class MitsubishiHeavyZMHeatpumpIR : public MitsubishiHeavyHeatpumpIR
{
  public:
    MitsubishiHeavyZMHeatpumpIR();
};


#endif
