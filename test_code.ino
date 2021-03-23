//Valve Pins
const int upstream_valve = 22;
const int downstream_valve_L = 5;
const int downstream_valve_R = 6;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode(upstream_valve, OUTPUT);
  pinMode(downstream_valve_L, OUTPUT);
  pinMode(downstream_valve_R, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println("High");
  digitalWrite(upstream_valve, HIGH);
  delay(500);
  digitalWrite(downstream_valve_L, HIGH);
  delay(500);
  digitalWrite(downstream_valve_R, HIGH);

  delay(2000);

  Serial.println("Low");
  digitalWrite(upstream_valve, LOW);
  delay(500);
  digitalWrite(downstream_valve_L, LOW);
  delay(500);
  digitalWrite(downstream_valve_R, LOW);

  delay(2000);

}
