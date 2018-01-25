# arduino-heatpumpir

Original location: https://github.com/ToniA/arduino-heatpumpir

For concrete examples, see the MySensors (Arduino + nRF24 radio) and ESPEasy (ESP8266 modules) integrations:
* https://github.com/mysensors/MySensorsArduinoExamples/tree/master/examples/HeatpumpIRController
* https://github.com/ToniA/ESPEasy/blob/master/ESPEasy/_P115_HeatpumpIR.ino

An Arduino library to control pump/split unit air conditioner. Currently supporting at least these models:

* Ballu
* Carrier 42NQV035G / 38NYV035H2 (Carrier remote control P/N WH-L05SE)
* Daikin RXS25G2V1B / FVXS25FV1B (Remote control P/N ARC452A1)
* Hisense AUD (remote control Y-H1-01,  Y-H1-02(E), Y-J1, Y-E4-07) probably AUC model
* Hyundai (Remote Control P/N Y512F2)
   * This is probably a generic Gree model
* Fujitsu Nocria AWYZ14 (remote control P/N AR-PZ2)
   * Also Fujitsu remote controls RY3-AR and AR-RCE1E
* IVT remote control CRMC-A673JBEZ
* Midea MSR1-12HRN1-QC2 + MOA1-12HN1-QC2, sold as Ultimate Pro Plus Basic 13FP in Finland (Midea remote control P/N RG51M1/E)
   * Also other Midea models sold as 'Ultimate', even some inverter models
   * Also Onnline (sold through Onninen) has been reported to work
* Mitsubishi Heavy SRKxxZJ-S (Remote control P/N RKX502A001C)
* Mitsubishi Heavy SRKxxZM-S (Remote Control P/N RLA502A700B)
* Mitsubishi MSZ FD-25, probably also FD-35 (remote control P/N KM09D 0052376)
   * Also FH series has been confirmed to work
* Panasonic E9/E12-CKP (Panasonic remote control P/N A75C2295)
* Panasonic E9/E12-DKE (Panasonic remote control P/N A75C2616)
* Panasonic E9/E12-JKE and E9/E12-NKE
* Samsung
   * AQV12PSBN / AQV09ASA
   * Samsung FJM (RJ040F2HXEA / 2XMH026FNEA), Remote Control P/N ARH-465
* Sharp AY-ZP40KR (remote control P/N CRMC-A788JBEZ), possibly also IVT
* Toshiba Daiseikai (Toshiba remote control P/N WH-TA01EE).
   * Fully compatible with CarrierNQV functions instead of Daiseikai functions.
   * Tested with: RAS-10G2KVP-E RAS-10G2AVP-E and RAS-13G2KVP-E RAS-13G2AVP-E



## Instructions

* Download the library, and place it under your personal Arduino 'libraries' directory, under directory 'HeatpumpIR'
* See the example sketches

### ESP8266 support

This library also supports ESP8266. Just change the IR send method from 'IRSenderPWM' to 'IRSenderBitBang':

    IRSenderBitBang irSender(1);     // IR led on ESP8266 digital pin 1

* Note that depending on your board, certain GPIO's might not be available. For example on NodeMCU, the usable GPIO's are D1 (GPIO5), D2 (GPIO4), D6 (GPIO12), D7 (GPIO13) and D3 (GPIO0).

### Arduino example

![Schema](https://raw.github.com/ToniA/arduino-heatpumpir/master/arduino_irsender.png)

## How to contribute

All these heatpump / A/C models have been reverse-engineered, by decoding the IR signal from the remote control. See the sketch https://github.com/ToniA/Raw-IR-decoder-for-Arduino for more information.

First the protocol needs to be understood
* The frame structure, and the timings of the symbols
* The bits which signal these
   * ON/OFF state
   * Operating mode (heat, cool, dry, fan, auto)
   * Temperature
   * Fan speed
   * Airflow direction etc. settings
   * And last, but not least, the algorithm to calculate the checksum of the IR signal
   
Once a working decoder module has been written, writing a transmitter module into HeatpumpIR is easy. I can help in coding the modules, and I'm also happy to receive pull requests.
