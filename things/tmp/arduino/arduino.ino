#include <SoftwareSerial.h>

int soil_moisturePin1 = A0;
int soil_moisturePin2 = A1;
SoftwareSerial ESPserial(10, 11); // RX | TX

void setup() {
  Serial.begin(9600);
  ESPserial.begin(9600);
}

void loop()
{
  float mos1 = (1023-analogRead(soil_moisturePin1))/1023.0*100.0;
  float mos2 = (1023-analogRead(soil_moisturePin2))/1023.0*100.0;
  String msg =  String(mos1) + ',' + String(mos2);
  ESPserial.println(msg);
  Serial.println(msg);
  delay(10000);
}

