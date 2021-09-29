/*********************************************************************************************************
* Easy Starter Kit Lesson 17: Knob Control Brightness
* 
* In this lesson you will learn how to control the brightness of your 4 digit display using the rotary knob.
* If you have everything attached from the previous lesson "L16_Knob_Angle" then you do not need to edit 
* the configuration.
* 
* Connect the 4 Digit Display to the yellow port with both D2 and D4 written next to it. 
* Connect the Rotary Knob to the blue port with A1 in its pin label. 
* 
* The following functions are available:
* 
* knob.getAngle();                        // gets the angle of the knob, the angle ranges from 0~280 degrees
* 
* disp.init();                            // initialize the 4 digit display
* disp.display(int8_t DispData []);       // display the numbers in the array
*                                         // For example, if the array is {1,2,3,4}, then display will read 1234
* disp.set(uint8_t brightness);           // set the brightness in range 0 ~ 7, 7 being the brightest                                        
*                                         // the next time display is changed, the brithness will change
* disp.point(0);                          // Turn off the display colon and the next display takes effect
* disp.point(1);                          // Turn on the display colon and the next display takes effect
*********************************************************************************************************/

#include "EasyStarterKitTM1637.h"   // include the display library
#include "EasyStarterKitKnob.h"     // include the rotary knob library
#define KNOB_PIN A1                 // SIG pin of Rotary Angle Sensor module connects to A1 of IO Shield, that is pin A1 of Arduino
Knob knob(KNOB_PIN);                // setup the knob object

#define CLK 4                       // CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                       // DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);               // setup display object

void setup(){                       // setup() runs once when the program starts
  disp.init();                      // The initialization of the display
  disp.point(1);                    // turn on the clock point;
}

void loop(){                        // loop() runs over and over
  int angle;                        // declare variable named angle of type int
  angle = knob.getAngle();          // set angle variable to the value of the knobs angle (0-280)

  int brightness;                   // declare variable named brightness of type int

                                                          // your angle is given within a range of 0-280
                                                          // the brightness has to be set in a range from 0-7
  brightness = map(angle, 0, FULL_ANGLE, 0, BRIGHTEST);   // here we use map to change the angle into a value between 0-7
  disp.set(brightness);                                   // set your brightness to our newly mapped value
  int8_t ListDisp[4] = {8,8,8,8};                         // create an array to be displayed, by default this is {8,8,8,8}
  disp.display(ListDisp);                                 // display the array of ints, by default this will display 8888
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
