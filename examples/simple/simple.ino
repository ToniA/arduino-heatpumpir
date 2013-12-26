#include <Arduino.h>

#include <FujitsuHeatpumpIR.h>
#include <PanasonicCKPHeatpumpIR.h>
#include <PanasonicHeatpumpIR.h>
#include <CarrierHeatpumpIR.h>
#include <MideaHeatpumpIR.h>
#include <MitsubishiHeatpumpIR.h>

IRSender irSender(3); // IR led on Duemilanove digital pin 3

// Array with all supported heatpumps
HeatpumpIR *heatpumpIR[] = {new PanasonicCKPHeatpumpIR(), new PanasonicDKEHeatpumpIR(), new PanasonicJKEHeatpumpIR(), 
                            new PanasonicNKEHeatpumpIR(), new CarrierHeatpumpIR(), new MideaHeatpumpIR(), 
                            new FujitsuHeatpumpIR(), new MitsubishiHeatpumpIR(), NULL};

void setup()
{
  Serial.begin(9600);
  delay(500);

  Serial.println("Starting");
}

void loop()
{
  int i = 0;

  do {
    // Send the same IR command to all supported heatpumps
    heatpumpIR[i]->send(irSender, POWER_ON, MODE_HEAT, FAN_2, 24, VDIR_UP, HDIR_AUTO);
    delay(500);
  }
  while (heatpumpIR[++i] != NULL);

  delay(2000);
}