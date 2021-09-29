/************************************************* ************************************************** ******
* Easy Starter Kit Lesson 13: Touch and debounce
* In this lesson you will learn how to prevent the touch sensor (or any button) from counting multiple 
* times without releasing it first.  When you are finished looking through this lesson try to modify
* it to include the other three buttons from lesson 12.
* 
* Use a 4 wire connector to connect the display to the yellow port that has a D4 and D2 in its pin label
* 
* Use a 6 wire connector to connect the touch sensor to the red port that has MOSI and MISO in its pin labels
*
* The following functions are available:
*
* touch.get();                  // return is the touch area corresponding Arduino pin number, if not then return -1
* touch.getLongPress();         // return long press the corresponding Arduino pin number, if not return -1
*
* disp.init();                  // initialization
* disp.display(int Decimal);    // display range: -999 ~ 9999
************************************************** ************************************************** *****/

#include <EasyStarterKitTouchSensor.h>    //include the touch sensor library
#include <EasyStarterKitTM1637.h>         //include the TM1637 display library
  
#define CLK 4                     //CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                     //DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);             //setup TM1637 4 digit display

#define TOUCH_OUT1 10             //the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13             //the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11             //the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12             //the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);    //setup touch object

int count = 0;              //global variable count

void setup(){               //setup() runs once when the program is first started
  disp.init();              //The initialization of the display
  disp.display(count);      //display counter number
}

void loop(){                              //loop() runs over and over
  int button;                             //local variable button                       
  button = touch.get();                   //get button touched and store in button
  if(button == TOUCH_OUT1){               //if touch the TCH1 area
    delay(10);                            //delay for 10ms
    if(touch.get() == TOUCH_OUT1){        //check it again to confirm
      count++;                            //add 1 to count
	    disp.display(count);              //display new count
    }
	while(touch.get() == TOUCH_OUT1);     //Wait for the button to be released
  }
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
