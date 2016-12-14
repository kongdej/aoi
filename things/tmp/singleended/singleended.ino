#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1015 ads(0x48);     /* Use thi for the 12-bit version */

float voltage=0.0;

void setup(void) 
{
  Serial.begin(9600);  
  ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  ads.begin();
}

void loop(void) 
{
  int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);
  voltage= (adc0*2.0)/1000;
  Serial.print("AIN0: "); Serial.print(adc0); Serial.print("\tVoltage: "); Serial.println(voltage,7);
  voltage= (adc1*2.0)/1000;
  Serial.print("AIN1: "); Serial.print(adc1);Serial.print("\tVoltage: "); Serial.println(voltage,7);
  Serial.print("AIN2: "); Serial.println(adc2);
  Serial.print("AIN3: "); Serial.println(adc3);
  Serial.println(" ");
  
  delay(1000);
}
