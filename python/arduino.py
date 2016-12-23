#!/usr/bin/python

import microgear.client as microgear
import time
from datetime import datetime
#import serial

# Arduino Serial port
#ser = serial.Serial('/dev/ttyACM0',9600)

# NETPIE appid and apikeys
appid = "AOI"
gearkey = "DZj0xtc6o8HrX7u"
gearsecret =  "tFcW3484XcE7MydjMEOjVaYgi"

microgear.create(gearkey,gearsecret,appid,{'debugmode': False})

def connection():
  print "Now I am connected with netpie"

def subscription(topic,message):
  print topic+"="+message
#  if topic == "/PUDZAHydro/eccal" :
#    ser.write("1"+message)  

def disconnect():
  print "disconnect is work"

microgear.setalias("raspi")
microgear.on_connect = connection
microgear.on_message = subscription
microgear.on_disconnect = disconnect
microgear.subscribe("/data");
microgear.connect(False)

while True:
  microgear.publish("/data",str(datetime.now()),{'retain':True})
  time.sleep(2)

#  msg =  ser.readline()
#  print msg

#  microgear.chat("htmlgear",msg)
"""
  datalist = msg.split(',')
  if len(datalist) == 7:
    microgear.publish("/uno/amptemp",datalist[0])
    microgear.publish("/uno/amphum",datalist[1])
    microgear.publish("/uno/wtrtemp",datalist[2])
    microgear.publish("/uno/soilhum",datalist[3])
    microgear.publish("/uno/rain",datalist[4])
    microgear.publish("/uno/flow",datalist[5])
    microgear.publish("/uno/ec",datalist[6])
  elif msg.find(',') == -1:
    print msg
    microgear.publish("/eccalmsg",msg)
"""


