#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// Thingspeak
#include "ThingSpeak.h"
unsigned long myChannelNumber = 156939;
const char * myWriteAPIKey = "ZTX7R3Q0IBPBQYT3";


const char* ssid     = "BAZ";
const char* password = "gearman1";
IPAddress ip(192,168,1,13);  //Node static IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

//WiFiClient client;

ESP8266WebServer server(80);
String mos;
void handleRoot() {
  while (Serial.available()) {
       mos = Serial.readString();
  }   
  String html = "hello from esp8266!";
  html += mos;
  server.send(200, "text/plain", "hello from esp8266! = " + mos);
}

void handleNotFound(){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(){
  Serial.begin(9600);

  Serial.println("Booting");  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
 // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);  
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
   server.handleClient();   
}
