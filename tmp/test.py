#!/usr/bin/python
import microgear.client as microgear
import paho.mqtt.client as mqtt
from time import localtime, strftime
import requests
#import psutil
import time

# NETPIE appid and apikeys
appid = "AOI"
gearkey = "NBy45MlnWNntL3P"
gearsecret =  "ytfxYCfBTBoMT734Om7EXPkC5"

microgear.create(gearkey,gearsecret,appid,{'debugmode': False})

def connection():
  print "Now I am connected with netpie"


def subscription(topic,message):
  print topic+"="+message

def disconnect():
  print "disconnect is work"

microgear.setalias("raspi")
microgear.on_connect = connection
microgear.on_message = subscription
microgear.on_disconnect = disconnect
microgear.subscribe("/netTopic")
microgear.connect(False)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("outTopic")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
#    microgear.publish("netTopic",str(msg.payload))

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.79", 9883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
#while client.loop() == 0:
#    client.publish("outTopic","Hello")
#    microgear.publish("outTopic","Hi")
#    time.sleep(10)# sleep for 10 seconds before next call
