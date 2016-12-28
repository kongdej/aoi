#!/usr/bin/python

import microgear.client as microgear
import time
import serial
import RPi.GPIO as GPIO

# Raspery GPIO setup
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
pump_relay = 18
valve_relay = [23,24,25]

GPIO.setup(pump_relay, GPIO.OUT)
GPIO.output(pump_relay, False)
for i in range(0,3) :
  GPIO.setup(valve_relay[i], GPIO.OUT)
  GPIO.output(valve_relay[i], False)


sp=[-1,-1,-1] # set point
ton=10 # time on
toff=5 # time off
pump_state=0
toff_cnt=0
ton_cnt=0
hstart=8
hstop=15
mlist = [0.0,0.0,0.0]

with open("config.ini", "r") as f:
  config = f.read()
  clist = config.split(',')
  sp = [float(clist[0]),float(clist[1]),float(clist[2])]
  ton = int(clist[3])
  toff = int(clist[4])
  hstart = int(clist[5])
  hstop = int(clist[6])

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
  global ton,toff,hstart,hstop
  #print topic+"="+message
  if topic == "/AOI/cmd" :
    if message == '00':
      GPIO.output(pump_relay, False)
    elif message == '01':
      GPIO.output(pump_relay, True)
    if message == '10':
      GPIO.output(valve_relay[0], False)
    elif message == '11':
      GPIO.output(valve_relay[0], True)
    if message == '20':
      GPIO.output(valve_relay[1], False)
    elif message == '21':
      GPIO.output(valve_relay[1], True)
    if message == '30':
      GPIO.output(valve_relay[2], False)
    elif message == '31':
      GPIO.output(valve_relay[2], True)

  if topic == "/AOI/sp" :
    splist = message.split(',')
    for i in range(0,3):
      if len(splist[i]) > 0:
        sp[i] = float(splist[i])
      else:
        splist[i] = str(sp[i])

    if len(splist[3]) > 0:
      ton = int(splist[3])
    else:
      splist[3] = str(ton)
    
    if len(splist[4]) > 0:
      toff = int(splist[4])
    else:
      splist[4] = str(toff)

    if len(splist[5]) > 0:
      hstart = int(splist[5])
    else:
      splist[5] = str(hstart)

    if len(splist[6]) > 0:
      hstop = int(splist[6])
    else:
      splist[6] = str(hstop)

    message = ','.join(splist)

    with open("config.ini", "w") as f:
      f.write(message) 
    
def disconnect():
  print "disconnect is work"

microgear.setalias("raspi")
microgear.on_connect = connection
microgear.on_message = subscription
microgear.on_disconnect = disconnect
microgear.subscribe("/data");
microgear.subscribe("/cmd");
microgear.subscribe("/sp");
microgear.connect(False)


def checkMaxSpan(v):
  span = [0,0,0]
  for i in range(0,3) :
    span[i] = sp[i] - v[i]
  print "{0},{1},{2}".format(span[0],span[1],span[2])
  return span.index(max(span))

while True:
  # data published format
  #       [-Relay Status] [-Set Point-] [--------Time---------] [--------Data--------]
  # msg = [pump,v1,v2,v3],[sp1,sp2,sp3],[ton,toff,hstart,hstop],[volt,m1,m2,m3,m4,t,h]
  msg = str(GPIO.input(pump_relay))+','+str(GPIO.input(valve_relay[0]))+','+str(GPIO.input(valve_relay[1]))+','+str(GPIO.input(valve_relay[2]))+','
  msg += str(sp[0])+','+str(sp[1])+','+str(sp[2])+','+str(ton)+','+str(toff)+','+str(hstart)+','+str(hstop)+','
  msg += ser.readline()  
  print msg
  with open("data.log", "w") as f:
    f.write(msg) 

  datalist = msg.split(',')
  if len(datalist) == 18:
    microgear.publish("/data",msg)
    mlist = [float(datalist[12]),float(datalist[13]),float(datalist[14])] 
  
  if pump_state == 0 and toff_cnt <= 0:
    vno = checkMaxSpan(mlist)
    if (mlist[vno] < sp[vno]) :
      pump_state = 1
      GPIO.output(valve_relay[vno], True)
      GPIO.output(pump_relay, True)
      ton_cnt = ton

  elif pump_state == 1 and (mlist[vno] >= sp[vno] or ton_cnt <= 0):
    pump_state = 0
    GPIO.output(pump_relay, False)
    GPIO.output(valve_relay[vno], False)
    toff_cnt = toff

  if pump_state == 0:
    if toff_cnt <= 0 :
      toff_cnt = 0 
    else: 
      toff_cnt -= 1
    print "{0} OFF ({1})". format(vno,toff_cnt)
  else:
    if ton_cnt <= 0:
      ton_cnt = 0
    else:
      ton_cnt -= 1 
    print "{0} ON ({1})". format(vno,ton_cnt)

  time.sleep(1)  
