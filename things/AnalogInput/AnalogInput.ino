int sensorPin = A0;    // select the input pin for the potentiometer

void setup() {
  Serial.begin(9600);
  
}

void loop() {
  // read the value from the sensor:
  int sensorValue = analogRead(sensorPin);
  float volt = sensorValue/1024.0*3.3*5.0*1.031;
  Serial.println(sensorValue);
  Serial.println(volt);
  delay(1000);
}
