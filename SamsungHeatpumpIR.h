/*
    Samsung xxx / yyy heatpump control (remote control P/N zzz)
*/
#ifndef SamsungHeatpumpIR_h
#define SamsungHeatpumpIR_h

#include <Arduino.h>
#include <IRSender.h>
#include <HeatpumpIR.h>

// Samsung timing constants
#define SAMSUNG_AIRCON1_HDR_MARK   3000
#define SAMSUNG_AIRCON1_HDR_SPACE  9000
#define SAMSUNG_AIRCON1_BIT_MARK   540
#define SAMSUNG_AIRCON1_ONE_SPACE  1500
#define SAMSUNG_AIRCON1_ZERO_SPACE 450
#define SAMSUNG_AIRCON1_MSG_SPACE  1600

// Samsung codes
#define SAMSUNG_AIRCON1_MODE_AUTO  0x00 // Operating mode
#define SAMSUNG_AIRCON1_MODE_HEAT  0x40
#define SAMSUNG_AIRCON1_MODE_COOL  0x10
#define SAMSUNG_AIRCON1_MODE_DRY   0x20
#define SAMSUNG_AIRCON1_MODE_FAN   0x30
#define SAMSUNG_AIRCON1_MODE_OFF   0xB2 // Power OFF
#define SAMSUNG_AIRCON1_MODE_ON    0x92 // Power ON
#define SAMSUNG_AIRCON1_FAN_AUTO   0x01 // Fan speed
#define SAMSUNG_AIRCON1_FAN1       0x05 // * low
#define SAMSUNG_AIRCON1_FAN2       0x09 // * med
#define SAMSUNG_AIRCON1_FAN3       0x0B // * high


class SamsungHeatpumpIR : public HeatpumpIR
{
  public:
    SamsungHeatpumpIR();
    void send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd);

  private:
    void sendSamsung(IRSender& IR, byte powerMode, byte operatingMode, byte fanSpeed, byte temperature);
};

#endif
