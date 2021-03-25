#include <SPI.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>
#include <EEPROM.h>
//Referring to code from: https://github.com/ejlabs/arduino-nitrox-analyzer 


int firstTime; // how long since the button was first pressed 

//Cycle Time Variables
bool initialization = false;
const int vacuum_time = 4; //Length of vacuum is held during regeneration period
const int blowdown_time = 4; //Length of blowdown is held during regeneration period

int secs_held;

const int total_cycle_time = 2* (vacuum_time + blowdown_time);
double cur_cycle_time;

int cur_state = 0;

//Valve Pins
const int upstream_valve = 4;
const int downstream_valve_L = 5;
const int downstream_valve_R = 6;

float o2_reading;
float pt1_reading;
float pt2_reading;

//Sensor pins - Analog input
Adafruit_ADS1115 ads(0x48);
const int pt1_pin = A1;
const int pt2_pin = A2;

//Sensor value modifiers
float o2_multiplier = 1.0F;
float PT_multiplier = 0.0977F;


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
  ads.setGain(GAIN_SIXTEEN);
  Serial.begin(9600);

}

 void read_sensor(int pt_pin, float var) 
 {
  float voltage = analogRead(pt_pin);
  var = voltage * PT_multiplier;
  //Serial.print("Voltage: ");
  //Serial.println(var);
 }

void loop()
{

 //Read Sensors
 read_oxygen_sensor(o2_reading);
 Serial.print("O2 Sensor:"); Serial.println(o2_reading);
 read_sensor(pt1_pin, pt1_reading);
 Serial.print("PT1: "); Serial.println(pt1_reading);
 read_sensor(pt2_pin, pt2_reading);
 Serial.print("PT2: "); Serial.println(pt2_reading);

  if (initialization == false) {
    firstTime = millis();
    initialization = true;
  }

  secs_held = (millis() - firstTime) / 1000;
  cur_cycle_time = secs_held % total_cycle_time;

    //Check for each of 4 possible states and modify valves accordingly
    if (cur_cycle_time < vacuum_time) {
      //Set state to state 1 if not already (Right Vacuum Regen)
      if (cur_state != 1) {
        Serial.println("In cycle part 1");
        digitalWrite(upstream_valve, HIGH);
        digitalWrite(downstream_valve_L, LOW);
        digitalWrite(downstream_valve_R, HIGH);
        cur_state = 1;
      }
    }
    else if (cur_cycle_time < vacuum_time + blowdown_time) {        
      //Set state to state 2 if not already (Right Blowdown Regen)
      if (cur_state != 2) {

        Serial.println("In cycle part 2");
        digitalWrite(upstream_valve, HIGH);
        digitalWrite(downstream_valve_L, LOW);
        digitalWrite(downstream_valve_R, LOW);
        cur_state = 2;       
      }
    }
    else if (cur_cycle_time < 2*vacuum_time + blowdown_time) {
      //Set state to state 3 if not already (Left Vacuum Regen)
      if (cur_state != 3) {
        Serial.println("In cycle part 3");
        digitalWrite(upstream_valve, LOW);
        digitalWrite(downstream_valve_L, HIGH);
        digitalWrite(downstream_valve_R, LOW);
        cur_state = 3;       
      }
    }
    else {
      //Set state to state 4 if not already (Left Blowdown Regen)
      if (cur_state != 4) {
        Serial.println("In cycle part 4");
        digitalWrite(upstream_valve, LOW);
        digitalWrite(downstream_valve_L, LOW);
        digitalWrite(downstream_valve_R, LOW);
        cur_state = 4;       
      }
    }



}