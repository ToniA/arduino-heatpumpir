/*
    Mitsubishi MSZ FD-25 heatpump control (remote control P/N KM09D 0052376)
*/
#ifndef MitsubishiHeatpumpIR_h
#define MitsubishiHeatpumpIR_h

#include <Arduino.h>
#include "IRSender.h"
#include "HeatpumpIR.h"


// Mitsubishi MSZ FD-25 timing constants (remote control P/N KM09D 0052376)
#define MITSUBISHI_AIRCON1_HDR_MARK   3500
#define MITSUBISHI_AIRCON1_HDR_SPACE  1700
#define MITSUBISHI_AIRCON1_BIT_MARK   430
#define MITSUBISHI_AIRCON1_ONE_SPACE  1250
#define MITSUBISHI_AIRCON1_ZERO_SPACE 390
#define MITSUBISHI_AIRCON1_MSG_SPACE  17500

// Mitsubishi codes
#define MITSUBISHI_AIRCON1_MODE_AUTO  0x60 // Operating mode
#define MITSUBISHI_AIRCON1_MODE_HEAT  0x48
#define MITSUBISHI_AIRCON1_MODE_COOL  0x58
#define MITSUBISHI_AIRCON1_MODE_DRY   0x50
#define MITSUBISHI_AIRCON1_MODE_OFF   0x00 // Power OFF
#define MITSUBISHI_AIRCON1_MODE_ON    0x20 // Power ON
#define MITSUBISHI_AIRCON1_FAN_AUTO   0xB8 // Fan speed - mixed with vertical swing...
#define MITSUBISHI_AIRCON1_FAN1       0x79
#define MITSUBISHI_AIRCON1_FAN2       0x7A
#define MITSUBISHI_AIRCON1_FAN3       0x7B
#define MITSUBISHI_AIRCON1_FAN4       0x7C


class MitsubishiHeatpumpIR : public HeatpumpIR
{
  public:
    MitsubishiHeatpumpIR();
    void send(IRSender& IR, byte powerModeCmd, byte operatingModeCmd, byte fanSpeedCmd, byte temperatureCmd, byte swingVCmd, byte swingHCmd);

  private:
    void sendMitsubishi(IRSender& IR, byte powerMode, byte operatingMode, byte fanSpeed, byte temperature, byte swingVCmd, byte swingHCmd);
};

#endif
