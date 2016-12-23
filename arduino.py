#!/usr/bin/python

import microgear.client as microgear
import time
import serial

# Arduino Serial port
ser = serial.Serial('/dev/ttyUSB0',9600)

# NETPIE appid and apikeys
appid = "AOI"
gearkey = "DZj0xtc6o8HrX7u"
gearsecret =  "tFcW3484XcE7MydjMEOjVaYgi"

microgear.create(gearkey,gearsecret,appid,{'debugmode': False})

def connection():
  print "Now I am connected with netpie"

def subscription(topic,message):
#  print topic+"="+message
  if topic == "/PUDZAHydro/eccal" :
    ser.write("1"+message)  

def disconnect():
  print "disconnect is work"

microgear.setalias("raspi")
microgear.on_connect = connection
microgear.on_message = subscription
microgear.on_disconnect = disconnect
microgear.subscribe("/data");
microgear.connect(False)

while True:
  msg =  ser.readline()
  print msg
  datalist = msg.split(',')
  microgear.publish("/data",msg)
  if len(datalist) == 7:
    microgear.publish("/data",msg)
  elif msg.find(',') == -1:
    print msg
    microgear.publish("/data",msg)
  time.sleep(2)  
