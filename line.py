import requests,json
import microgear.client as microgear
import sys

# NETPIE appid and apikeys
appid = "PudzaAOI"
gearkey = "i3AAVW7LSV7Proj"
gearsecret =  "uBKcM9TRIqUl0pd9H3Pz1zH6m"

microgear.create(gearkey,gearsecret,appid,{'debugmode': False})
def connection():
  print "Now I am connected with netpie"

def subscription(topic,message):
#  print topic+"="+message

#LINE
  LINE_ACCESS_TOKEN="32xcreVbSqhuoOfPGfyIhji81k7PtKjnOmr1tz99h4L" #token
  url = "https://notify-api.line.me/api/notify"

  datalist = message.split(',')
  datas = "\n"
  datas += "ZoneX: "+datalist[12]+"%\n"
  datas += "ZoneY: "+datalist[13]+"%\n"
  datas += "ZoneZ: "+datalist[14]+"%\n"
  datas += "ZoneR: "+datalist[15]+"%\n"
  datas += "Temperature: "+datalist[16]+"C\n"
  datas += "Humidity: "+datalist[17]+""
  datas += "BATT: "+datalist[11]+" V\n"
  msg = {"message":datas} #message
  LINE_HEADERS = {'Content-Type':'application/x-www-form-urlencoded',"Authorization":"Bearer "+LINE_ACCESS_TOKEN}
  session = requests.Session()
  resp = session.post(url, headers=LINE_HEADERS, data=msg)
  print resp
  sys.exit()

def disconnect():
  print "disconnect is work"

microgear.setalias("line")
microgear.on_connect = connection
microgear.on_message = subscription
microgear.on_disconnect = disconnect
microgear.subscribe("/data");
microgear.connect(True)

