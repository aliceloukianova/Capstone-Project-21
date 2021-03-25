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
const int valve_1 = 4;
const int valve_2 = 5;
const int valve_3 = 6;
const int valve_4 = 7;

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
  pinMode(valve_1, OUTPUT);
  pinMode(valve_2, OUTPUT);
  pinMode(valve_3, OUTPUT);
  pinMode(valve_4, OUTPUT);

  firstTime = millis();

  Serial.println("in Setup");


}

void loop()
{

  //cur_cycle_time = secs_held % total_cycle_time;
  cur_cycle_time = (millis() / 1000) % total_cycle_time;
  //Serial.print("cur cycle time: "); Serial.print(cur_cycle_time); Serial.print(" of "); Serial.println(total_cycle_time);
  //Serial.print("Cur State: "); Serial.println(cur_state);

    if (cur_cycle_time < 4) {
      //Set state to state 1 if not already (Right Vacuum Regen)
      if (cur_state != 1) {
        Serial.println("In cycle part 1");
        //Tube A goes to buffer, Tube B is vacuumed
        digitalWrite(valve_1, LOW);
        digitalWrite(valve_2, HIGH);
        digitalWrite(valve_3, LOW);
        digitalWrite(valve_4, HIGH);
        cur_state = 1;
      }
      return;
    }
    if (cur_cycle_time < 8) {
      //Set state to state 2 if not already (Right Blowdown Regen)
      if (cur_state != 2) {
        Serial.println("In cycle part 2");
        //Tube A opens
        digitalWrite(valve_1, LOW);
        digitalWrite(valve_2, LOW);
        digitalWrite(valve_3, HIGH);
        digitalWrite(valve_4, HIGH);
        cur_state = 2;
      }
      return;
    }

        if (cur_cycle_time < 12) {
      //Set state to state 2 if not already (Right Blowdown Regen)
      if (cur_state != 3) {
        Serial.println("In cycle part 3");
        digitalWrite(valve_1, HIGH);
        digitalWrite(valve_2, HIGH);
        digitalWrite(valve_3, HIGH);
        digitalWrite(valve_4, LOW);
        cur_state = 3;
      }
      return;
    }

    else {
      if (cur_state != 4) {
        Serial.println("In cycle part 4");
        digitalWrite(valve_1, HIGH);
        digitalWrite(valve_2, LOW);
        digitalWrite(valve_3, HIGH);
        digitalWrite(valve_4, HIGH);
        cur_state = 4;
      }
      return;
    }



}