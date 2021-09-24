/**************************************************************************************************
* Easy Starter Kit Lesson 3: Breathing Light
* 
* In this lesson you learn how to make the LED have a breathing effect.  It will cycle between dim
* and bright.  If you still have your LED and button connected from L02 you can leave them both 
* connected or remove the button if you would like.
* 
* Use a 3 wire connector to connect the LED to the port that has a D6 on its pin labels.
*  
*
* The following functions are available:
*
* led.on();         //turn on the led.  You can also use a number between 1 and 255 to control the
*                   //brightness like this led.on(127);
*                   
* led.off();        //turn off the led
* 
* led.breathe();    //sets the led to have a breathing effect.  You can change the speed of the
*                   //breathing by passing and integer number like this led.breath(3).
*                   //the maximum speed is 30
*
***************************************************************************************************/

#include <EasyStarterKitLED.h>  //include the led library

#define LED_PIN 6         //the LED is connect to D6 of Arduino, D6 is a pwm pin
LED led(LED_PIN);         //setup the led object

void setup(){             //setup() runs once when the program is first started
  led.on();               //turn on the led
  delay(500);             //wait half a second
  led.off();              //turn off the led
  delay(1000);            //wait half a second
}

void loop(){              //start of loop(), loop() runs over and over again forever
  led.breathe();          //The default speed is 3 seconds to breathe once
                          //breath() only cycles the LED once.  It happens repeatedly
                          //because it is in the loop() function.
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
