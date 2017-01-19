import microgear.client as microgear 
import time
#import serial

# Arduino Serial port
#ser = serial.Serial('/dev/ttyUSB0',9600)

# NETPIE appid and apikeys
appid = "AOI"
gearkey = "DZj0xtc6o8HrX7u"
gearsecret =  "tFcW3484XcE7MydjMEOjVaYgi"

microgear.create(gearkey,gearsecret,appid,{'debugmode': False})

def connection():
  print "Now I am connected with netpie"
  microgear.setalias("raspi1")
#  microgear.subscribe("/msg");

def subscription(topic,message):
  print topic+"="+message
    
def disconnect():
  print "disconnect is work"

microgear.on_connect = connection
microgear.on_message = subscription
microgear.on_disconnect = disconnect
#microgear.subscribe("/cmd");
#microgear.subscribe("/sp");
microgear.connect(True)

#while True:
#  y,m,d,h,mi,s,wd,wy,isd = time.localtime() 
#  msg = ser.readline()  
#  microgear.publish("/data",msg) 
#  time.sleep(1)  
