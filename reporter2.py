import time
import requests

#line notify
LINE_ACCESS_TOKEN="32xcreVbSqhuoOfPGfyIhji81k7PtKjnOmr1tz99h4L" #token
url = "https://notify-api.line.me/api/notify"

# Thingspeak
urlThingspeak = "https://api.thingspeak.com/update.json"
api_key_1 = "S9ZEMKADJDRSR4RX"
api_key_2 = "FMZM64TP5E80JKMQ"

# data published format
#       [-Relay Status] [-Set Point-] [--------Time---------] [--------Data--------]
# msg = [pump,v1,v2,v3],[sp1,sp2,sp3],[ton,toff,hstart,hstop],[volt,m1,m2,m3,m4,t,h]
with open("/root/aoi/data1.log", "r") as f:
	data = f.read()
	dlist = data.split(',')
	#send line
	datas = "\n"
	datas += "ZoneX: "+dlist[12]+"%\n"
	datas += "ZoneY: "+dlist[13]+"%\n"
	datas += "ZoneZ: "+dlist[14]+"%\n"
	datas += "ZoneR: "+dlist[15]+"%\n"
	datas += "Temperature: "+dlist[16]+"C\n"
	datas += "Humidity: "+dlist[17]+""
	datas += "BATT: "+dlist[11]+"v\n"
	msg = {"message":datas} #message
	LINE_HEADERS = {'Content-Type':'application/x-www-form-urlencoded',"Authorization":"Bearer "+LINE_ACCESS_TOKEN}
	session = requests.Session()
	resp =session.post(url, headers=LINE_HEADERS, data=msg)
	f.close()

