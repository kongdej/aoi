#include "DHT.h"
#define DHTTYPE DHT21   // DHT 22  (AM2302), AM2321
#define DHTPIN 3 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  float soil_moisture[4] = {0,0,0,0};
  float h_air = dht.readHumidity();
  float t_air = dht.readTemperature();

  if (isnan(h_air) || isnan(t_air)) {
    Serial.println("Failed to read from DHT sensor!");
    t_air = -1;
    h_air = -1;
  }

  for(int i; i<4; i++) {
    soil_moisture[i] = (1023-analogRead(i))/1023.0*100.0;
    Serial.print(soil_moisture[i]);
    Serial.print(',');
  }
  Serial.print(t_air);
  Serial.print(',');
  Serial.println(h_air);

  delay(2000);
}
