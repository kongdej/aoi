#!/usr/bin/python

import paho.mqtt.client as mqtt
from time import localtime, strftime
import requests
#import psutil
import time
import thingspeak

# Thingspeak
urlThingspeak = "https://api.thingspeak.com/update.json"
api_key = "ZTX7R3Q0IBPBQYT3"
channel_id = "156939"
channel = thingspeak.Channel(id=channel_id,write_key=api_key)
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("outTopic")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
 
    y,m,d,h,mi,s,wd,wy,isd = time.localtime() 

    if s % 15 == 0:
       station_id,datas = msg.payload.split('=')
       d1,d2 = datas.split(',');
       res = channel.update({1:str(d1),2:'100'})
       print res

#    text,no = msg.payload.split("\#")
#    payload = {'api_key': api_key, 'field1' : "1234"}
#    r = requests.post(urlThingspeak,params=payload,verify=False)

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("192.168.1.79", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.
client.loop_forever()
