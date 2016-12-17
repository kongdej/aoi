#include <ESP8266WiFi.h>
#include <MicroGear.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Wire.h>
#include <Adafruit_ADS1015.h>
Adafruit_ADS1115 ads;     /* Use thi for the 12-bit version */

// Light sensor : SCL-SCL(GPIO5)  D1, SDA-SDA(GPIO4)  D2, ADD-NC or GND 
//#include <BH1750.h>
//BH1750 lightMeter;

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

int relay[4] = {D5,D6,D7,D8};
int analogPin = A0;
int timer = 0;

void relayOperation(uint8_t* msg) {
  int rac = msg[1] == '1' ? HIGH:LOW;
  msg[1] = '\0';
  int rno = relay[String((char *)msg).toInt()];
  Serial.printf("Relay %d -> %d \r\n",rno,rac);
  digitalWrite(rno,rac);
}

MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.print(topic);
    Serial.println((char *)msg);
    if (String(topic) == "/AOI/cmd") {      
      relayOperation(msg);     
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
//      ESP.restart();
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
    ads.begin();
    for (int i=0; i<4; i++) {
      pinMode(relay[i], OUTPUT);
    }
//    lightMeter.begin(); 
}

void loop() {
    ArduinoOTA.handle();

    int16_t adc0, adc1, adc2, adc3, voltbit;
    float v1,v2,v3,v4,v5;
    char msg[100],buff1[50],buff2[50],buff3[50],buff4[50],buff5[50];

    if (microgear.connected()) {
        microgear.loop();

        if (timer >= 1000) {
            adc0 = ads.readADC_SingleEnded(0);
            adc1 = ads.readADC_SingleEnded(1);
            adc2 = ads.readADC_SingleEnded(2);
            adc3 = ads.readADC_SingleEnded(3);
            voltbit = analogRead(analogPin);
            v1 = 100 - ((adc0*0.1875)/1000)/3.3*100;
            v2 = 100 - ((adc1*0.1875)/1000)/3.3*100;
            v3 = 100 - ((adc2*0.1875)/1000)/3.3*100;
            v4 = 100 - ((adc3*0.1875)/1000)/3.3*100;
            v5 = (voltbit/1023.0)*3.3*5.0*1.031; //0.024
  //          uint16_t lux = lightMeter.readLightLevel();
            uint16_t lux=0;
            dtostrf(v1,3, 2, buff1);
            dtostrf(v2,3, 2, buff2);
            dtostrf(v3,3, 2, buff3);
            dtostrf(v4,3, 2, buff4);
            dtostrf(v5,3, 2, buff5);
            sprintf(msg, "%s,%s,%s,%s,%d,%d,%d,%d,%s,%d", buff1,buff2,buff3,buff4,digitalRead(relay[0]),digitalRead(relay[1]),digitalRead(relay[2]),digitalRead(relay[3]),buff5,lux);
            Serial.println(msg);
            microgear.publish("/data",msg);
            delay(500);
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
