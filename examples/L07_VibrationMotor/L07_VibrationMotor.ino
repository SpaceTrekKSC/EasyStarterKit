/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 7:  Vibration Motor
* You can learn how to control the vibration motor. 
*
* The following functions are available:
*
* vibration.on();//Turn on vibration
* vibration.off();//Turn off vibration
* Rockerswitch.get(); // return HIGH or LOW
*
************************************************** ************************************************** *****/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitSwitch.h"
#include "EasyStarterKitVibration.h"

#define VibrationMotor_PIN 6//the SIG pin of the module is connected with D6 of Arduino / OPEN-SMART UNO
Vibration vibration(VibrationMotor_PIN);

#define ROCKER_PIN A2 //SIG pin of Rocker Switch module connect to A2 of IO Shield, that is pin A2 of Arduino
Switch Rockerswitch(ROCKER_PIN);

void setup()
{
  
}
void loop()
{
  if(Rockerswitch.get()) vibration.on();
  else vibration.off();
}
											
/*********************************************************************************************************
The end of file
*********************************************************************************************************/