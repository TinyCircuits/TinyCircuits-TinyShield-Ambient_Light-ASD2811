/*
  TinyCircuits Ambient Light TinyShield Example Sketch
  Using AMS TAOS TSL2572

  This example code is in the public domain.

  Written 25 October 2015
  By Ken Burns
  Modified 07 January 2019
  By Hunter Hykes

  https://TinyCircuits.com
*/

#include <Wire.h>

#define TSL2572_I2CADDR     0x39

#define GAIN_1X 0
#define GAIN_8X 1
#define GAIN_16X 2
#define GAIN_120X 3

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#include <SoftwareSerial.h>
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

//only use this with 1x and 8x gain settings
#define GAIN_DIVIDE_6 true 

int gain_val = 0;

void setup()
{
  Wire.begin();
  SerialMonitorInterface.begin(9600);
  TSL2572nit(GAIN_1X);
}


void loop()
{
  float AmbientLightLux = Tsl2572ReadAmbientLight();
  SerialMonitorInterface.print("Lux: ");
  SerialMonitorInterface.println(AmbientLightLux);
  
  delay(1000);
}


void TSL2572nit(uint8_t gain)
{
  Tsl2572RegisterWrite( 0x0F, gain );//set gain
  Tsl2572RegisterWrite( 0x01, 0xED );//51.87 ms
  Tsl2572RegisterWrite( 0x00, 0x03 );//turn on
  if(GAIN_DIVIDE_6)
    Tsl2572RegisterWrite( 0x0D, 0x04 );//scale gain by 0.16
  if(gain==GAIN_1X)gain_val=1;
  else if(gain==GAIN_8X)gain_val=8;
  else if(gain==GAIN_16X)gain_val=16;
  else if(gain==GAIN_120X)gain_val=120;
}


void Tsl2572RegisterWrite( byte regAddr, byte regData )
{
  Wire.beginTransmission(TSL2572_I2CADDR);
  Wire.write(0x80 | regAddr); 
  Wire.write(regData);
  Wire.endTransmission(); 
}


float Tsl2572ReadAmbientLight()
{     
  uint8_t data[4]; 
  int c0,c1;
  float lux1,lux2,cpl;

  Wire.beginTransmission(TSL2572_I2CADDR);
  Wire.write(0xA0 | 0x14);
  Wire.endTransmission();
  Wire.requestFrom(TSL2572_I2CADDR,4);
  for(uint8_t i=0;i<4;i++)
    data[i] = Wire.read();
     
  c0 = data[1]<<8 | data[0];
  c1 = data[3]<<8 | data[2];
  
  //see TSL2572 datasheet
  cpl = 51.87 * (float)gain_val / 60.0;
  if(GAIN_DIVIDE_6) cpl/=6.0;
  lux1 = ((float)c0 - (1.87 * (float)c1)) / cpl;
  lux2 = ((0.63 * (float)c0) - (float)c1) / cpl;
  cpl = max(lux1, lux2);
  return max(cpl, 0.0);
}
