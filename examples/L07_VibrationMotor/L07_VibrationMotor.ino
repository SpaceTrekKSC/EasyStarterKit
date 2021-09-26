/**************************************************************************************************
* Easy Starter Kit Lesson 7:  Vibration Motor
* In this lesson you will learn how to use the vibration motor.  The vibration motor is blue board
* with a black and silver circular component on it
* 
* Use a 3 wire connector to connect the rocker switch to blue port with A2 on its pin label
* 
* Use a 3 wire connector to connect the vibration motor to the blue port with D6 on its pin label
*
* The following functions are available:
*
* vibration.on();       //Turn on vibration
* vibration.off();      //Turn off vibration
* Rockerswitch.get();   //return HIGH or LOW
*
***************************************************************************************************/

#include <EasyStarterKitSwitch.h>             //include the switch library
#include <EasyStarterKitVibration.h>          //include the vibration motor library

#define VibrationMotor_PIN 6                  //the SIG pin of the module is connected with D6 of Arduino / OPEN-SMART UNO
Vibration vibration(VibrationMotor_PIN);      //setup the vibration object

#define ROCKER_PIN A2                         //SIG pin of Rocker Switch module connect to A2 of IO Shield, that is pin A2 of Arduino
Switch Rockerswitch(ROCKER_PIN);              //setup the rocker switch object

//setup() runs once when the program is first started
void setup(){
  //this program does not do anything in the setup() function, but
  //Arduino requires the setup() function to be in the program
}

void loop(){                                  //loop repeats over and over
  if(Rockerswitch.get()) vibration.on();      //if Rockerswitch.get() returns true turn on the vibration motor
  else vibration.off();                       //otherwise turn off the vibration motor
}
											
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
