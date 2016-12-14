#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 ads(0x48);     /* Use thi for the 12-bit version */

const char* ssid     = "BAZ";
const char* password = "gearman1";
// Update these with values suitable for your network.
IPAddress ip(192,168,1,17);  //Node static IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

#define APPID   "AOI"
#define KEY     "r7P03akJkOtFOlM"
#define SECRET  "uwUVFN7vfrXdaefP8663i0qia"
#define ALIAS   "nodemcu"

WiFiClient client;

float v1,v2,v3,v4;

int pump = D5;
int valve1 = D6;
int valve2 = D7;

int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.print(topic);
    Serial.println((char *)msg);
    if (String(topic) == "/AOI/cmd") {      
      if (msg[0] == '1') {
        Serial.print("PUMP -> ");        
        if (msg[1] == '1') {
          Serial.println("ON");
          digitalWrite(pump,HIGH);
          delay(500);
          if (digitalRead(pump) == HIGH) {       
            microgear.publish("/status","11");
          }
          else {
            microgear.publish("/status","10");            
          }
        }
        else if (msg[1] == '0') {
          Serial.println("OFF");                  
        }
      }
      if (msg[0] == '2') {
        Serial.print("VALVE 1 -> ");        
        if (msg[1] == '1') {
          Serial.println("ON");        
        }
        else if (msg[1] == '0') {
          Serial.println("OFF");                  
        }
      }
      if (msg[0] == '3') {
        Serial.print("VALVE 2 -> ");        
        if (msg[1] == '1') {
          Serial.println("ON");        
        }
        else if (msg[1] == '0') {
          Serial.println("OFF");                  
        }
      }
      
    }
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Found new member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();  
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.print("Lost member --> ");
    for (int i=0; i<msglen; i++)
        Serial.print((char)msg[i]);
    Serial.println();
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
    Serial.println("Connected to NETPIE...");
    /* Set the alias of this microgear ALIAS */
    microgear.setAlias(ALIAS);
    microgear.subscribe("/cmd");
}


void setup() {
    /* Add Event listeners */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(PRESENT,onFoundgear);
    microgear.on(ABSENT,onLostgear);
    microgear.on(CONNECTED,onConnected);
    Serial.begin(115200);
 
    /*-- OTA setup --*/ 
    Serial.println("Booting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.config(ip, gateway, subnet);  
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
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

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
//    ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    ads.begin();
}

void loop() {
    ArduinoOTA.handle();
    int16_t adc0, adc1, adc2, adc3;


    /* To check if the microgear is still connected */
    if (microgear.connected()) {
//        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
     //       Serial.println("Publish...");

            /* Chat with the microgear named ALIAS which is myself */
            adc0 = ads.readADC_SingleEnded(0);
            adc1 = ads.readADC_SingleEnded(1);
            adc2 = ads.readADC_SingleEnded(2);
            adc3 = ads.readADC_SingleEnded(3);

            char msg[100],buff1[50],buff2[50],buff3[50],buff4[50];

            v1 = 100 - ((adc0*3.0)/1000)/3.3*100;
            v2 = 100 - ((adc1*3.0)/1000)/3.3*100;
            v3 = 100 - ((adc2*3.0)/1000)/3.3*100;
            v4 = (adc3*3.0)/1000*5;
            dtostrf(v1,3, 2, buff1);
            dtostrf(v2,3, 2, buff2);
            dtostrf(v3,3, 2, buff3);
            dtostrf(v4,3, 2, buff4);
            sprintf(msg, "%s,%s,%s,%s", buff1,buff2,buff3,buff4);
            Serial.println(msg);
            microgear.publish("/data",msg);
            timer = 0;
        } 
        else timer += 100;
    }
    else {
        Serial.println("connection lost, reconnect...");
        if (timer >= 5000) {
            microgear.connect(APPID);
            timer = 0;
        }
        else timer += 100;
    }
    delay(100);
}
