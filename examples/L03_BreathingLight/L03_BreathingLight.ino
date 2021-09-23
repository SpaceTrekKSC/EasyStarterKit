/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 3: Breathing Light
* You can learn how to make breathing light. 
*
* The following functions are available:
*
* led.on();//turn on the led
* led.off();//turn off the led
* led.breathe(); // breathe(uint8_t speed = 1) default parameter is 1, and you can pass the number more than one to 
*                     // speed up the breath.
*
************************************************** ************************************************** *****/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitLED.h"

#define LED_PIN 6          //LED connect to D6 of Arduino, D6 is pwm pin
LED led(LED_PIN);          

void setup()
{
  led.on(); 
  delay(500);
  led.off();
  delay(1000);
}

void loop()
{
  led.breathe(); //The default speed is 3 seconds to breathe once
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/