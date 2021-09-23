/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 22: Ultrasonic distance measurement and display
* You can learn to use  the ultrasonic distance measurement module to measure the distance 
* of the nearest obstacle in front, the maximum measurement is 500 cm, at least 3 cm, 
* the 4-digit display will display the integer part of value.
*
* The following functions are available:

* ultrasonic.readDistance();//Get the measured distance, and return a float type value in centimeters
* disp.init(); // initialization
* disp.display(int Decimal);   // display range: -999 ~ 9999
************************************************** **************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitUltrasonic.h"
#include "EasyStarterKitTM1637.h"

#define CLK 4//CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2//DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);


//module--- Arduino
//VCC   --- 5V
//GND   --- GND
#define TRIGGER_PIN  7//Trig pin of ultrasonic sensor module connect to D7 of Arduino
#define ECHO_PIN     8//Echo to D8
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);


void setup()
{
  disp.init();//The initialization of the display
}

void loop()
{
  float cmMsec;  
  cmMsec = ultrasonic.readDistance();//if out of range, it return -1;
  disp.display((int)cmMsec);//in centimeters
  delay(500);
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
