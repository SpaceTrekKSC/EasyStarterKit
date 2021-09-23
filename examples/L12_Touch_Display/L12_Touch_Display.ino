/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 12: Touch and display
* You can learn about the touch pad control method, digital increase, reduce, clear operation of the digital display 
*
* The following functions are available:
*
* touch.get(); / / return is the touch area corresponding Arduino pin number, if not then return -1
* touch.getLongPress(); / / return long press the corresponding Arduino pin number, if not return -1
*
* disp.init(); // initialization
* disp.display(int Decimal);   // display range: -999 ~ 9999

************************************************** ************************************************** *****/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitTouchSensor.h"
#include "EasyStarterKitTM1637.h"
	
#define CLK 4//CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2//DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);

#define TOUCH_OUT1 10//out1-TCH1 area corresponds to the number increase
#define TOUCH_OUT2 13//out2-TCH2 area corresponds to the number decrease
#define TOUCH_OUT3 11//out3-TCH3 area corresponds to clear number to be "0"
#define TOUCH_OUT4 12//out4-TCH4 area corresponds to display maximum number "9999"
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);

int count = 0;

void setup()
{
  disp.init();//The initialization of the display
  disp.display(count);//display counter number
}

void loop()
{
  int button;
  button = touch.get();
  if(button == TOUCH_OUT1) //if touch the TCH1 area
  {
    count++;
  }
  else if(button == TOUCH_OUT2) //if touch the TCH2 area
  {
    count--;
  }
  else if(button == TOUCH_OUT3) //if touch the TCH3 area
  {
    count = 0;
  }
  else if(button == TOUCH_OUT4) //if touch the TCH4 area
  {
    count = 9999;
  }
  disp.display(count);
  delay(50);//
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
