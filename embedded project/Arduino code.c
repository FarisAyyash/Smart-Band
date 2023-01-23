#include <Wire.h>
#include "SoftwareSerial.h"
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000
SoftwareSerial bluetooth(10,11);
PulseOximeter pox;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    //Serial.println("Beat!");
}
 
void setup()
{
    Serial.begin(2400);
 
    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    if (!pox.begin()){
        for(;;);
   }
     pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
    // Register a callback for the beat detection
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
 
void loop()
{
    // Make sure to call update as fast as possible
    pox.update();
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        char strBuf[4];
        char HR=pox.getHeartRate();
        char OX=pox.getSpO2();
        char magic1=0xca;
        char magic2=0xc5;

        strBuf[0] = magic1;
        strBuf[1] = HR;
        strBuf[2] = OX;
        strBuf[3] = magic2;
        
        Serial.write(strBuf,4);
        tsLastReport = millis();
    }
}