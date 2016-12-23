import RPi.GPIO as GPIO
import time

x=25
GPIO.setmode(GPIO.BCM)
GPIO.setup(x, GPIO.OUT)

GPIO.output(x, True)
print 'on ', x
time.sleep(.5)
GPIO.output(x, False)
print 'off ', x
