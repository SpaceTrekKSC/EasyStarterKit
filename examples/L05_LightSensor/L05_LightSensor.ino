/************************************************* *************************************************
* Easy Starter Kit Lesson 5:  Light Sensor
* In this lesson you will learn how to use the ambient light sensor. It is a photoresistor which 
* changes resistance based on how much light is hitting it.  The more light it is exposed to the
* lower its resistance will be.  The light sensor is a blue board with a red and silver sensor
* and a black microchip on it.
* 
* Use a 3 wire connector to connect the light sensor to the port that has an A0 in its pin labels.
*
* The following functions are available:
*
* lightsensor.getRes();         //it returns the resistance of the light sensor, the darker the ambient light, the larger the resistance.
*
************************************************** ************************************************** *****/

#include <math.h>                             //include the math.h library for using pow()
#include <EasyStarterKitLightSensor.h>        //include the light sensor library

#define LIGHTSENSOR_PIN A0                    //SIG pin of light sensor module connected to A0 of IO Shield, that is pin A0 of Arduino
LightSensor lightsensor(LIGHTSENSOR_PIN);     //setup the lightsensor object

float Rsensor = 0.0;                          //declare Rsensor as a global variable and initialize it to 0.0

void setup() {                                //setup() runs once when the program is first started
  Serial.begin(9600);                         //Set baud rate of serial port to be 9600bps 
                                              //on the upper right corner of Arduino IDE use the Serial Monitor to check the sensor data.
}
void loop() {                                 //start of loop(), loop() runs over and over again forever
  Rsensor = lightsensor.getRes();             //if Rsensor is larger than 40 KOhm, the ambient light is very dark.
                                              //if Rsensor is smaller than 10 KOhm, the ambient light is bright.
                                       
  Serial.println("The resistance is ");       //print this to the serial terminal with a new line character
  Serial.print(Rsensor,1);                    //print the Rsensor value to the serial terminal
  Serial.println(" KOhm");                    //print the units and a new line character to the serial terminal
  float lux;                                  //declare a variable named lux of type float
  lux = 325*pow(Rsensor,-1.4);                //calculate the approximate lux using this formula
  Serial.print("Luminescence is about ");     //it is just an approximate conversion to lux
  Serial.print(lux,1);                        //print the approximate lux value
  Serial.println(" lux");                     //print the units and a new line character
  delay(1000);                                //wait for 1 second before repeating
}

/*
 * Try experimenting with different amounts of light hitting the sensor and see how the 
 * resistance and lux values change.
 */

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
