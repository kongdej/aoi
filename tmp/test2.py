import time

sp=[25,50,75] # set point
ton=10 # time on
toff=5 # time off

testv = [10,40,60]
pump_state=0
valve_no=0
toff_cnt=0
ton_cnt=0

def checkMaxSpan(v,sp):
	span = [0,0,0]
	for i in range(0,3) :
		span[i] = sp[i] - v[i]
		#print span[i]
	
	return span.index(max(span))

while True:
	if pump_state == 0 and toff_cnt <= 0:
		valve_no = checkMaxSpan(testv,sp)
		pump_state = 1 
		ton_cnt = ton

	elif pump_state == 1 and (testv[valve_no] >= sp[valve_no] or ton_cnt <= 0):
		pump_state = 0
		toff_cnt = toff

	if pump_state == 0:
		toff_cnt -= 1
		print "off"
		print toff_cnt
	else:
		ton_cnt -= 1
		print "on"
		print ton_cnt

	time.sleep(1)
