#include <Arduino.h>

#include <FujitsuHeatpumpIR.h>
#include <PanasonicCKPHeatpumpIR.h>
#include <PanasonicHeatpumpIR.h>
#include <CarrierHeatpumpIR.h>
#include <MideaHeatpumpIR.h>
#include <MitsubishiHeatpumpIR.h>

IRSender irSender(3); // IR led on Duemilanove digital pin 3

// PROGMEM (i.e. these do not consume SRAM) strings for the heatpump names
// Would be nice to have these within the HeatpumpIR library, but it turned out that
// C++ classes cannot have 'PROGMEM' member variables
static const prog_char panaCKPModel[] PROGMEM = "panasonic_ckp";
static const prog_char panaCKPInfo[]  PROGMEM = "extra info about Panasonic CKP";
static const prog_char panaDKEModel[] PROGMEM = "panasonic_dke";
static const prog_char panaJKEModel[] PROGMEM = "panasonic_jke";
static const prog_char panaNKEModel[] PROGMEM = "panasonic_nke";
static const prog_char carrierModel[] PROGMEM = "carrier";
static const prog_char mideaModel[]   PROGMEM = "midea";
static const prog_char fujitsuModel[] PROGMEM = "fujitsu_awyz";
static const prog_char mitsuFDModel[] PROGMEM = "mitsubishi_fd";
static const prog_char mitsuFEModel[] PROGMEM = "mitsubishi_fe";

// Array with all supported heatpumps
// The constructor can be called without any parameters, in that case the methods 'model' and 'info' just return NULL
HeatpumpIR *heatpumpIR[] = {new PanasonicCKPHeatpumpIR(panaCKPModel, panaCKPInfo), new PanasonicDKEHeatpumpIR(panaDKEModel), new PanasonicJKEHeatpumpIR(panaJKEModel),
                            new PanasonicNKEHeatpumpIR(panaNKEModel), new CarrierHeatpumpIR(carrierModel), new MideaHeatpumpIR(mideaModel),
                            new FujitsuHeatpumpIR(fujitsuModel), new MitsubishiFDHeatpumpIR(mitsuFDModel), new MitsubishiFEHeatpumpIR(mitsuFEModel), NULL};

void setup()
{
  Serial.begin(9600);
  delay(500);

  Serial.println("Starting");
}

void loop()
{
  int i = 0;
  char buffer[100];

  do {
    // Send the same IR command to all supported heatpumps
    Serial.print("Sending IR to ");

    // Copy the 'model' from FLASH to SRAM for printing
    if (heatpumpIR[i]->model() != NULL) {
      strcpy_P(buffer, heatpumpIR[i]->model());
      Serial.println(buffer);
    }

	// Copy the 'info' from FLASH to SRAM for printing
    if (heatpumpIR[i]->info() != NULL) {
      strcpy_P(buffer, heatpumpIR[i]->info());
      Serial.println(buffer);
    }

    heatpumpIR[i]->send(irSender, POWER_ON, MODE_HEAT, FAN_2, 24, VDIR_UP, HDIR_AUTO);
    delay(500);
  }
  while (heatpumpIR[++i] != NULL);

  delay(2000);
}