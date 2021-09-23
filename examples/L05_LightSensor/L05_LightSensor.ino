/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 5:  Light Sensor
* You can learn how to know the ambient light is dark or bright. 
*
* The following functions are available:
*
* lightsensor.get();//it return the resistance of the light sensor, the darker the ambient light, the larger the resistance.
*
*
* Modified by: Andrew Gafford 16 June 2021
* 
************************************************** ************************************************** *****/

#include <math.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitLightSensor.h"

#define LIGHTSENSOR_PIN A0//SIG pin of Rocker Switch module connect to A0 of IO Shield, that is pin A0 of Arduino


//Object Instances
LightSensor lightsensor(LIGHTSENSOR_PIN);


//Global Variables
float Rsensor = 0.0;     //Declare Rsensor as a global variable and initialize it to 0.0

void setup() {
  Serial.begin(9600); //Set baud rate of serial port to be 9600bps 
                      //on the upper right corner of Arduino IDE use the Serial Monitor to check the sensor data.
}
void loop() {
  Rsensor = lightsensor.getRes();      //if Rsensor is larger than 40 KOhm, the ambient light is very dark.
                                       //if Rsensor is smaller than 10 KOhm, the ambient light is bright.
  Serial.println("The resistance of the Light sensor is "); 
  Serial.print(Rsensor,1);
  Serial.println(" KOhm");
  float lux;
  lux = 325*pow(Rsensor,-1.4);
  Serial.print("Illuminance is about "); // may be it is not exactly right
  Serial.print(lux,1);
  Serial.println(" lux");
  delay(1000);  
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/