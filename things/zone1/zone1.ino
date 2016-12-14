#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 ads(0x48);     /* Use thi for the 12-bit version */
float v1=0.0;
float v2=0.0;

const char* ssid     = "ZAB";
const char* password = "Gearman1";
// Update these with values suitable for your network.
IPAddress ip(192,168,1,17);  //Node static IP
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

#define APPID   "AOI"
#define KEY     "r7P03akJkOtFOlM"
#define SECRET  "uwUVFN7vfrXdaefP8663i0qia"
#define ALIAS   "zone1"

WiFiClient client;

int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
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
    microgear.subscribe("/zone1/mosture/1");
    microgear.subscribe("/zone1/mosture/2");
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

    /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
    /* You may want to use other method that is more complicated, but provide better user experience */
    if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
    }

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
    ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
    ads.begin();
}

void loop() {
    ArduinoOTA.handle();
    int16_t adc0, adc1, adc2, adc3;


    /* To check if the microgear is still connected */
    if (microgear.connected()) {
        Serial.println("connected");

        /* Call this method regularly otherwise the connection may be lost */
        microgear.loop();

        if (timer >= 1000) {
            Serial.println("Publish...");

            /* Chat with the microgear named ALIAS which is myself */
            adc0 = ads.readADC_SingleEnded(0);
            adc1 = ads.readADC_SingleEnded(1);
            adc2 = ads.readADC_SingleEnded(2);
            adc3 = ads.readADC_SingleEnded(3);
            v1= (adc0*2.0)/1000;
            char buff1[50];
            dtostrf(v1,7, 3, buff1);
            v2= (adc1*2.0)/1000;
            char buff2[50];
            dtostrf(v2,7, 3, buff2);
            char msg[50];
            sprintf(msg, "%s,%s", buff1,buff2);
            microgear.publish("/zone1",msg);
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
