/*  NETPIE ESP8266 basic asynchronous publish sample                            */
/*  More information visit : https://netpie.io             */

#include <ESP8266WiFi.h>
#include <MicroGear.h>
//#include "CMMC_Interval.hpp"

const char* ssid     = "BAZ";
const char* password = "gearman1";

#define APPID   "AOI"
#define KEY     "r7P03akJkOtFOlM"
#define SECRET  "uwUVFN7vfrXdaefP8663i0qia"
#define ALIAS   "zone1"


WiFiClient client;
//CMMC_Interval timer001;

//int timer = 0;
MicroGear microgear(client);

/* If a new message arrives, do this */
void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) {
/*  msg[msglen] = '\0';
  String _msg = String((char*)msg);
  Serial.print("Incoming message --> ");
  Serial.print(_msg);
  Serial.printf(" at [%lu] \r\n", millis());
*/
}

void onFoundgear(char *attribute, uint8_t* msg, unsigned int msglen) {
/*  Serial.print("Found new member --> ");
  for (int i = 0; i < msglen; i++) {
    Serial.print((char)msg[i]);
  }
  Serial.println();
*/
}

void onLostgear(char *attribute, uint8_t* msg, unsigned int msglen) {
/*     
 Serial.print("Lost member --> ");
  for (int i = 0; i < msglen; i++) {
    Serial.print((char)msg[i]);
  }
  Serial.println();
*/
}

/* When a microgear is connected, do this */
void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) {
  Serial.println("Connected to NETPIE...");
  microgear.setAlias(ALIAS);
  microgear.subscribe("/zone1");
}

void setup() {
  /* Add Event listeners */

  /* Call onMsghandler() when new message arraives */
  microgear.on(MESSAGE, onMsghandler);

  /* Call onFoundgear() when new gear appear */
  microgear.on(PRESENT, onFoundgear);

  /* Call onLostgear() when some gear goes offline */
  microgear.on(ABSENT, onLostgear);

  /* Call onConnected() when NETPIE connection is established */
  microgear.on(CONNECTED, onConnected);

  Serial.begin(9600);
  Serial.println("Starting...");

  /* Initial WIFI, this is just a basic method to configure WIFI on ESP8266.                       */
  /* You may want to use other method that is more complicated, but provide better user experience */
  if (WiFi.begin(ssid, password)) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
     }
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connecting to NETPIE.io");
  /* Initial with KEY, SECRET and also set the ALIAS here */
  microgear.init(KEY, SECRET, ALIAS);

  /* connect to NETPIE to a specific APPID */
  microgear.connect(APPID);
}

char msg[50];

void loop() {
  microgear.loop();
  if (microgear.connected()) {
    for(int i=0;i<50;i++) msg[i]=NULL;
    int i=0;
    while (Serial.available() > 0) {
      int inChar = Serial.read();
      msg[i++] = inChar;
      if (inChar == '\n') {
        microgear.publish("/zone1",msg );
        Serial.print(msg);
        i=0;
      }
    }        
  }
  else {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
    delay(2000);
  }
  delay(500);
}
