/*
  TinyCircuits Ambient Light TinyShield Example Sketch
  Using AMS TAOS TSL2572

  This example code is in the public domain.

  Written 25 October 2015
  By Ken Burns
  Modified 31 December 2020
  By Hunter Hykes

  https://TinyCircuits.com
*/

#include <Wire.h>
#include <TSL2572.h>      // For TSL2572 ambient light sensor

TSL2572 light_sensor;

#if defined (ARDUINO_ARCH_AVR)
#define SerialMonitorInterface Serial
#include <SoftwareSerial.h>
#elif defined(ARDUINO_ARCH_SAMD)
#define SerialMonitorInterface SerialUSB
#endif

int gain_val = 0;

void setup()
{
  Wire.begin();
  SerialMonitorInterface.begin(9600);

  //***************************************
  // SETTINGS & ADJUSTMENTS 
  //***************************************
  //  light_sensor.init(GAIN_1X);
  //  light_sensor.init(GAIN_8X);
    light_sensor.init(GAIN_16X);
  //  light_sensor.init(GAIN_120X);
}

void loop()
{
  float AmbientLightLux = light_sensor.readAmbientLight();
  SerialMonitorInterface.print("Lux: ");
  SerialMonitorInterface.println(AmbientLightLux);
  
  delay(1000);
}
