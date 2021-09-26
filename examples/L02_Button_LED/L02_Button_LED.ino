/**************************************************************************************************
* Easy Starter Kit Lesson 2: Button and LED 
* 
* This lesson will show you how to use the button and LED.  The button is a blue board with a blue
* push button on it. The LED is a blue board and has a clear LED on it.
* 
* Use a 3 wire connector to connect the LED to the port that has a D6 on its pin labels.
* 
* Use a 3 wire connector to connect the button to the port that has A2 on its pin labels.
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
* button.get();     //return HIGH (true) when the button is pressed and LOW (false) when not pressed
*
*********************************************************************************************************/

#include <EasyStarterKitLED.h>      //include the LED library
#include <EasyStarterKitSwitch.h>   //include the Switch library

#define LED_PIN 6                   //LED connect to D6 of Arduino, D6 is pwm pin
LED led(LED_PIN);                   //setup the led as a LED object

#define BUTTON_PIN A2               //SIG pin of button module connect to A2 of IO Shield, that is pin A2 of Arduino 
Switch button(BUTTON_PIN);          //setup the button as a switch object

//setup() runs once when the program is first started
void setup(){
  //this program does not do anything in the setup() function, but
  //Arduino requires the setup() function to be in the program
}

void loop(){                    //loop repeats over and over
  if(button.get()) led.on();    //if button.get() returns HIGH level, turn on the LED
  else led.off();               //otherwise, turn off the LED
}

/*
* Note: If you use the led.breath() function here it will complete one full breath cycle, so there will
* be a delay before it turns off when you release the button
*/

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
