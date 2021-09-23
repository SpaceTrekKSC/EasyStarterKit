/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 17: Knob control brightness
* You can learn use the knob(rotary angle sensor) to control the brightness of the display.
*
* The following functions are available:

* knob.getAngle();//read the angle of the knob, the range of angle is 0~280 degrees

* disp.init(); // initialization
* disp.display(int8_t DispData []); // display the numbers in the array
* // For example, the array is {1,2,3,4}, then show 1234
* disp.set(uint8_t brightness); // set the brightness, 0 ~ 7, the greater the value, the higher the brightness, 
                                       //the next display takes effect
* disp.point(0); // Turn off the display colon and the next display takes effect
* disp.point(1); // Turn on the display colon and the next display takes effect
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
  disp.point(1);//turn on the clock point;
}

void loop()
{
  int angle;
  angle = knob.getAngle();

  int brightness;
	/*The degrees is 0~280, should be converted to be 0~7 to control the*/
	/*brightness of display */
  brightness = map(angle, 0, FULL_ANGLE, 0, BRIGHTEST); 
  disp.set(brightness);
  int8_t ListDisp[4] = {8,8,8,8};
  disp.display(ListDisp);//display array
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
