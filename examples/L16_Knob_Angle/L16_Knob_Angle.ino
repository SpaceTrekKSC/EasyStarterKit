/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 16: Knob angle display
* You can learn get the angle of the knob(rotary angle sensor) and display.
*
* The following functions are available:
*
* knob.getAngle();//read the angle of the knob, the range of angle is 0~280 degrees
* 
* disp.init(); // initialization
* disp.display(int Decimal);   // display range: -999 ~ 9999
************************************************** **************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>
#include "EasyStarterKitTM1637.h"
#include "EasyStarterKitKnob.h"
#define KNOB_PIN A1//SIG pin of Rotary Angle Sensor module connect to A0 of IO Shield, that is pin A0 of Arduino
Knob knob(KNOB_PIN);

#define CLK 4//CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2//DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);

void setup()
{
  disp.init();//The initialization of the display
}

void loop()
{
  int angle;
  angle = knob.getAngle();
  disp.display(angle);
  delay(500);
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
