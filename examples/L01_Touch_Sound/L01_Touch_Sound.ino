/************************************************* *************************************************
* Easy Starter Kit Lesson 01: Touch and sound
*
* This lesson will show you how to use the touch sensor and the active buzzer.  The touch sensor
* is a red board and has four touch areas on it labeled 1-4 and showing common media control
* symbols.  The MakerBox kit comes with two buzzers; an active and a passive. The buzzers look
* very similar but operate completely differently.  The active buzzer is the one that has the 
* white sticker on it.  It is on a blue board.
* 
* Use a 3 wire connector to connect the active buzzer to the blue port that has a D6 on its pin
* labels.
* 
* Use a 6 wire connector to connect the touch sensor to the red port that has MOSI and MISO on
* its pin labels.
* 
* The following functions are available: 
*
* buzzer.on();              //turn on the buzzer
* buzzer.off();             //turn off the buzzer
* touch.get();              //returns the corresponding Arduino pin number, if not touched then returns -1
* touch.getLongPress();     //returns long press for the corresponding Arduino pin number, if not touched then returns -1
*
************************************************** ************************************************** *****/

#include "EasyStarterKitTouchSensor.h"
#include "EasyStarterKitBuzzer.h"

#define BUZZER_PIN 6              //the SIG pin of the active Buzzer is connected to D6 of Arduino
Buzzer buzzer(BUZZER_PIN);        //setup the buzzer object

#define TOUCH_OUT1 10             //the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13             //the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11             //the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12             //the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);      //setup the touch object

//setup() runs once when the program is first started
void setup(){
  //this program does not do anything in the setup() function, but
  //Arduino requires the setup() function to be in the program
}

void loop(){                          //start of loop(), loop() runs over and over again forever
  int button;                         //declare a variable to store what button is being pressed
  button = touch.get();               //use the touch object to get what button is being pressed and store it in the variable named button
  
  if(button == TOUCH_OUT1){           //if the out1-THC1 area is pressed
    buzzer.on();                      //turn on the buzzer
  }
  else if(button == TOUCH_OUT2){      //else if the out2-THC2 area is pressed
    buzzer.off();                     //turn off the buzzer
  }
  else if(button == TOUCH_OUT3){      //else if the out3-THC3 area is pressed 
                                      //don't do anything
  }
  else if(button == TOUCH_OUT4){      //else if the out4-THC4 area is pressed
                                      //don't do anything
  }
 
  delay(50);                          //wait 50ms before finishing and going back to the top of loop()
}                                     //end of loop()


/*********************************************************************************************************
The end of file
*********************************************************************************************************/
