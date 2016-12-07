#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

// Thingspeak
#include "ThingSpeak.h"
unsigned long myChannelNumber = 196411;
const char * myWriteAPIKey = "NNWUTPXGLI7JKN7F";


const char* ssid     = "BAZ";
const char* password = "gearman1";
//IPAddress ip(192,168,1,10);  //Node static IP
//IPAddress gateway(192,168,1,1);
//IPAddress subnet(255,255,255,0);

WiFiClient client;

unsigned long previousMillis = 0;     // will store last time LED was updated
const long interval = 2000;           // interval at which to blink (milliseconds)

int soil_moisturePin = A0;

void setup(){
  Serial.begin(9600);

  /*-- OTA setup DON'T MODIFY--*/ 
  Serial.println("Booting");
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
//  WiFi.config(ip, gateway, subnet);
   
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  
  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  /*-- END OTA Setup --*/
  ThingSpeak.begin(client);

}

void loop() {
   ArduinoOTA.handle();
  /** Start loop **/
    String msg = "";
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
       previousMillis = currentMillis;
       float mos = (1024-analogRead(soil_moisturePin))/1024.0*100.0;
       msg = String(mos);
       Serial.println(msg);
       ThingSpeak.setField(7,msg);
       ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);  
    }      
}
