/*********************************************************************************************************
* Easy Starter Kit Lesson 16: Knob angle display
* 
* In this lesson you will learn how to manipulate the 4 Digit Display using the rotary knob. The program
* is super basic but you will see this combination of hardware used in higher levels later on in the course.
* 
* Connect the 4 Digit Display to the yellow port with both D2 and D4 written next to it. 
* Connect the Rotary Knob to the blue port with A1 in its pin label. 
*
* The following functions are available:
*
* knob.getAngle();                    // get the angle of the knob, the angles range from 0~280 degrees
* 
* disp.init();                        // initialize the display
* disp.display(int Decimal);          // display an integer in the range: -999 ~ 9999
*********************************************************************************************************/

#include <EasyStarterKitTM1637.h>     // include the display library
#include <EasyStarterKitKnob.h>       // include the knob library

#define KNOB_PIN A1                   // SIG pin of Rotary Angle Sensor module connect to A1 of IO Shield, that is pin A1 of Arduino
Knob knob(KNOB_PIN);                  // setup the rotary knob object

#define CLK 4                         // CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                         // DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);                 // setup the display object

void setup(){                         // setup() runs once when the program is first started
  disp.init();                        // The initialization of the display
}

void loop(){                          // loop() runs over and over
  int angle;                          // declare a variable named angle of type int
  angle = knob.getAngle();            // set angle to the value of the angle (0-280 degrees)
  disp.display(angle);                // show the angle value on the display
  delay(500);                         // wait for 500ms before running again
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
