/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 4: Rocker Switch
* You can learn how to use rocker switch to control LED.
*
* The following functions are available:
*
* led.on();//turn on the led
* led.off();//turn off the led
* Rockerswitch.get(); // return HIGH or LOW
*
************************************************** ************************************************** *****/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitLED.h"
#include "EasyStarterKitSwitch.h"


#define LED_PIN 6          //LED connect to D6 of Arduino, D6 is pwm pin
LED led(LED_PIN);

#define ROCKER_PIN A2 //SIG pin of Rocker Switch module connect to A2 of IO Shield, that is pin A2 of Arduino
Switch Rockerswitch(ROCKER_PIN);

void setup()
{
  
}

void loop()
{
  if(Rockerswitch.get()) led.on();
  else led.off();
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
