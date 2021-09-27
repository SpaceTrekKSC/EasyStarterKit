/**************************************************************************************************
* Easy Starter Kit Lesson 12: Touch and display
* In this lesson you will use the touch pad to control the number shown on the 4 digit display. 
*
* Use a 4 wire connector to connect the display to the yellow port that has a D4 and D2 in its pin label
* 
* Use a 6 wire connector to connect the touch sensor to the red port that has MOSI and MISO in its pin labels
*
* The following functions are available:
*
* touch.get();                  // return is the touch area's corresponding Arduino pin number, if not then return -1
* touch.getLongPress();         // return long press the corresponding Arduino pin number, if not return -1
* disp.init();                  // initialization
* disp.display(int Decimal);    // display range: -999 ~ 9999

***************************************************************************************************/

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

int count = 0;                    //declare a global variable count

void setup(){                     //setup() runs once when the program is first started
  disp.init();                    //The initialization of the display
  disp.display(count);            //display counter number
}

void loop(){                      //loop() runs over and over
  int button;                     //declare the variable button
  button = touch.get();           //get the button touched and set button to it
  if(button == TOUCH_OUT1){       //if touched the out1-TCH1 area
    count++;                      //add 1 to the count
  }
  else if(button == TOUCH_OUT2){  //if touched the out2-TCH2 area
    count--;                      //remove 1 from the count
  }
  else if(button == TOUCH_OUT3){  //if touched the out3-TCH3 area
    count = 0;                    //reset the count to 0
  }
  else if(button == TOUCH_OUT4){  //if touched the out4-TCH4 area
    count = 9999;                 //set the count to 9999
  }
  disp.display(count);            //display count on the 4 digit display
  delay(50);                      //wait 50 ms to prevent a single touch from being counted multiple times
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
