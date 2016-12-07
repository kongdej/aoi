#include <SoftwareSerial.h>

int soil_moisturePin = A0;
SoftwareSerial ESPserial(2, 3); // RX | TX

void setup() {
  Serial.begin(9600);
  ESPserial.begin(9600);
}

void loop()
{
  delay(2000);

  float mos = (1024-analogRead(soil_moisturePin))/1024.0*100.0;
  Serial.println(mos);
  ESPserial.println(mos);
}

