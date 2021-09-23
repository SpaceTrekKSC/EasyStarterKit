/************************************************* *************************************************
* Easy Starter Kit Lesson 2: Button and LED
* You can learn about the Button control method, how to turn on and turn off the LED. 
* 
* This lesson will show you how to use the button and LED.  The button is a blue board with a blue
* push button on it. The LED is a blue board and has a clear LED on it.
* 
* Use a 3 wire connector to connect the LED to the port that has a D^ on its pin labels.
* 
* Use a 3 wire connector to connect the button to the port that has A2 on its pin labels.
* 
*
* The following functions are available:
*
* led.on();             //turn on the led
* led.off();            //turn off the led
* button.get();         //return HIGH (true) when the button is pressed and LOW (false) when not pressed

*********************************************************************************************************/

#include "EasyStarterKitLED.h"
#include "EasyStarterKitSwitch.h"

#define LED_PIN 6               //LED connect to D6 of Arduino, D6 is pwm pin
LED led(LED_PIN);         

#define BUTTON_PIN A2           //SIG pin of button module connect to A2 of IO Shield, that is pin A2 of Arduino 
Switch button(BUTTON_PIN);      //

//setup() runs once when the program is first started
void setup(){
  //this program does not do anything in the setup() function, but
  //Arduino requires the setup() function to be in the program
}

void loop(){
  if(button.get()) led.on();//if it get high level, turn on the LED
  else led.off();           //otherwise, turn off the LED
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
