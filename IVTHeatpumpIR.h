/*
    IVT heatpump control for IVT AY-XP12FR-N, remote control CRMC-A673JBEZ (almost the same as Sharp)

    Also see: https://github.com/skarlsso/IRRemoteIVT/blob/master/IRRemoteIVT
*/
#ifndef IVTHeatpumpIR_h
#define IVTHeatpumpIR_h

#include <HeatpumpIR.h>
#include <SharpHeatpumpIR.h>


class IVTHeatpumpIR : public SharpHeatpumpIR
{
  public:
    IVTHeatpumpIR();
    void send(IRSender& IR, uint8_t powerModeCmd, uint8_t operatingModeCmd, uint8_t fanSpeedCmd, uint8_t temperatureCmd, uint8_t swingVCmd, uint8_t swingHCmd);
};

#endif
