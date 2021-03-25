#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <EEPROM.h>
//Referring to code from: https://github.com/ejlabs/arduino-nitrox-analyzer


float o2_reading;

Adafruit_ADS1115 ads(0x48);

//Sensor value modifiers
float o2_multiplier = 1.0F;

//Read Oxygen with relevant oxygen_multiplier
void read_oxygen_sensor(float &o2_percentage) {
  int16_t millivolts = 0;
  millivolts = ads.readADC_Differential_0_1();
  o2_percentage = o2_multiplier*millivolts;
}


void setup()
{
  //Set Amplifier Gain
  ads.begin(); // ads1115 start
  Serial.begin(9600);
  ads.setGain(GAIN_SIXTEEN);
  
  Serial.println("in Setup");
}
void loop()
{
 //Read Sensors
 read_oxygen_sensor(o2_reading);
 Serial.print("O2 Sensor:"); Serial.println(o2_reading);
}