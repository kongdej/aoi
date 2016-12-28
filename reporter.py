# IFTTT
import time
import requests

event = "pudza_report"
url = "https://maker.ifttt.com/trigger/%s/with/key/dsFC-2tGEmWDjeXYVEx5m6" % event

# Thingspeak
urlThingspeak = "https://api.thingspeak.com/update.json"
api_key_1 = "S9ZEMKADJDRSR4RX"
api_key_2 = "FMZM64TP5E80JKMQ"

while True:
	# data published format
	#       [-Relay Status] [-Set Point-] [--------Time---------] [--------Data--------]
	# msg = [pump,v1,v2,v3],[sp1,sp2,sp3],[ton,toff,hstart,hstop],[volt,m1,m2,m3,m4,t,h]
	with open("data.log", "r") as f:
		data = f.read()
		dlist = data.split(',')

	payload = {'api_key': api_key_1, 'field1' : dlist[12],'field2' : dlist[13],'field3' : dlist[14],'field4' : dlist[15],'field5' : dlist[16],'field6' : dlist[17],'field7' : dlist[11]}
	r = requests.post(urlThingspeak,params=payload,verify=False)
	print r

	time.sleep(15)  
	
	payload = {'api_key': api_key_2, 'field1' : dlist[0],'field2' : dlist[1],'field3' : dlist[2],'field4' : dlist[3],'field5' : dlist[4],'field6' : dlist[5],'field7' : dlist[6]}
	r = requests.post(urlThingspeak,params=payload,verify=False)
	print r

	time.sleep(15)  
