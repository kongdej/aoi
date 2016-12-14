#include <SoftwareSerial.h>

int soil_moisturePin = A5;
SoftwareSerial ESPserial(3, 5); // RX | TX

void setup() {
  Serial.begin(9600);
  ESPserial.begin(9600);
}

void loop()
{
  while (ESPserial.available()) {
      Serial.print("<--");
      Serial.println(ESPserial.readString());
  }
  float mos = (1024-analogRead(soil_moisturePin))/1024.0*100.0;
  ESPserial.println(mos);
  Serial.println(mos);
  delay(2000);
}

