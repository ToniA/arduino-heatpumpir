/*
    Mitsubishi Heavy heatpump control (remote control P/N xxx)
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
#define MITSUBISHI_AIRCON2_MODE_OFF   0x00 // Power OFF
#define MITSUBISHI_AIRCON2_MODE_ON    0x08 // Power ON
#define MITSUBISHI_AIRCON2_FAN_AUTO   0xE0 // Fan speed 
#define MITSUBISHI_AIRCON2_FAN1       0xA0
#define MITSUBISHI_AIRCON2_FAN2       0x80
#define MITSUBISHI_AIRCON2_FAN3       0x60
#define MITSUBISHI_AIRCON2_FAN4       0x20 // High speed
#define MITSUBISHI_AIRCON2_FAN5       0x00 // Silent


class MitsubishiHeavyHeatpumpIR : public HeatpumpIR
{
  public:
    MitsubishiHeavyHeatpumpIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd);

  private:
    void sendMitsubishiHeavy(IRSender& IR, uint8_t powerMode, uint8_t operatingMode, uint8_t fanSpeed, uint8_t temperature, uint8_t swingVCmd, uint8_t swingHCmd);
};

#endif
