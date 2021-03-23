#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_ADS1015.h>
#include <EEPROM.h>
//Referring to code from: https://github.com/ejlabs/arduino-nitrox-analyzer 


 int firstTime; // how long since the button was first pressed 

//Cycle Time Variables
bool initialization = true;
const int vacuum_time = 4; //How long vacuum is held during regeneration period
const int blowdown_time = 4; //How long blowdown is held during regeneration period

int secs_held;

const int total_cycle_time =  2 * (vacuum_time + blowdown_time);
double cur_cycle_time;

int cur_state = 0;

//Valve Pins
const int upstream_valve = 4;
const int downstream_valve_L = 5;
const int downstream_valve_R = 6;

float pt1_reading;
float pt2_reading;

//Sensor pins - Analog input
//Adafruit_ADS1115 ads(0x48);
const int pt1_pin = A1;
const int pt2_pin = A2;

//Sensor value modifiers
float PT_multiplier = 0.0977F;

void setup()
{
  //Set Amplifier Gain
  //ads.begin(); // ads1115 start
  //ads.setGain(GAIN_SIXTEEN);
  Serial.begin(9600);
  pinMode(upstream_valve, OUTPUT);
  pinMode(downstream_valve_L, OUTPUT);
  pinMode(downstream_valve_R, OUTPUT);

  firstTime = millis();

  Serial.println("in Setup");
  

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
 //read_oxygen_sensor(o2_reading);
 //Serial.print("O2 Sensor:"); Serial.println(o2_reading);
 read_sensor(pt1_pin, pt1_reading);
 read_sensor(pt2_pin, pt2_reading);
 Serial.print("PT1: "); Serial.print(pt1_reading); Serial.print(", PT2: "); Serial.println(pt2_reading);

  //Serial.print("First time: "); Serial.println(firstTime);
  //Serial.println(millis());
  //secs_held = (millis() - firstTime) / 1000;
  
  //cur_cycle_time = secs_held % total_cycle_time;
  cur_cycle_time = (millis() / 1000) % total_cycle_time;
  Serial.print("cur cycle time: "); Serial.print(cur_cycle_time); Serial.print(" of "); Serial.println(total_cycle_time); 
  Serial.print("Cur State: "); Serial.println(cur_state);
  
    if (cur_cycle_time < 4) {
      //Set state to state 1 if not already (Right Vacuum Regen)
      if (cur_state != 1) {
        Serial.println("In cycle part 1");
        digitalWrite(upstream_valve, LOW);
        digitalWrite(downstream_valve_L, LOW);
        digitalWrite(downstream_valve_R, LOW);
        cur_state = 1;
      }
      return;
    }
    if (cur_cycle_time < 8) {        
      //Set state to state 2 if not already (Right Blowdown Regen)
      if (cur_state != 2) {
        Serial.println("In cycle part 2");
        digitalWrite(upstream_valve, LOW);
        digitalWrite(downstream_valve_L, HIGH);
        digitalWrite(downstream_valve_R, HIGH);
        cur_state = 2;       
      }
      return;
    }

        if (cur_cycle_time < 12) {        
      //Set state to state 2 if not already (Right Blowdown Regen)
      if (cur_state != 3) {
        Serial.println("In cycle part 2");
        digitalWrite(upstream_valve, HIGH);
        digitalWrite(downstream_valve_L, HIGH);
        digitalWrite(downstream_valve_R, LOW);
        cur_state = 3;       
      }
      return;
    }

    else {
      if (cur_state != 4) {
        Serial.println("In cycle part 4");
        digitalWrite(upstream_valve, HIGH);
        digitalWrite(downstream_valve_L, LOW);
        digitalWrite(downstream_valve_R, HIGH);
        cur_state = 2;       
      }
      return;
    }
  
     

}
