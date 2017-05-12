# line and thingspeak
import time
import requests

#line notify
LINE_ACCESS_TOKEN="32xcreVbSqhuoOfPGfyIhji81k7PtKjnOmr1tz99h4L" #token
url = "https://notify-api.line.me/api/notify"

# Thingspeak
urlThingspeak = "https://api.thingspeak.com/update.json"
api_key_1 = "S9ZEMKADJDRSR4RX"
api_key_2 = "FMZM64TP5E80JKMQ"

ho = 0;
pump_state = 0;
while True:
	y,m,d,h,mi,s,wd,wy,isd = time.localtime() 

	# data published format
	#       [-Relay Status] [-Set Point-] [--------Time---------] [--------Data--------]
	# msg = [pump,v1,v2,v3],[sp1,sp2,sp3],[ton,toff,hstart,hstop],[volt,m1,m2,m3,m4,t,h]
	with open("data.log", "r") as f:
		data = f.read()
		dlist = data.split(',')

	if s%15==0:
		payload = {'api_key': api_key_1, 'field1' : dlist[12],'field2' : dlist[13],'field3' : dlist[14],'field4' : dlist[15],'field5' : dlist[16],'field6' : dlist[17],'field7' : dlist[11]}
		r = requests.post(urlThingspeak,params=payload,verify=False)
		print r

		time.sleep(5)  
	
		payload = {'api_key': api_key_2, 'field1' : dlist[0],'field2' : dlist[1],'field3' : dlist[2],'field4' : dlist[3],'field5' : dlist[4],'field6' : dlist[5],'field7' : dlist[6]}
		r = requests.post(urlThingspeak,params=payload,verify=False)
		print r
	if h != ho:
		#send line
		ho = h
		datas = str(h)+":"+str(mi)+"\n"
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
		print(resp.text)

	if dlist[0] != pump_state:
		pump_state = dlist[0]
		dmsg = str(h)+":"+str(mi)+"\n"
		dmsg += "Pump: "+ dlist[0]+"\n"
		dmsg += "Valve1: "+ dlist[1]+"\n"
		dmsg += "Valve2: "+ dlist[2]+"\n"
		dmsg += "Valve3: "+ dlist[3]+"\n"
		msg = {"message":dmsg} #message
		LINE_HEADERS = {'Content-Type':'application/x-www-form-urlencoded',"Authorization":"Bearer "+LINE_ACCESS_TOKEN}
		session = requests.Session()
		resp =session.post(url, headers=LINE_HEADERS, data=msg)
		print(resp.text)

	time.sleep(1)  
