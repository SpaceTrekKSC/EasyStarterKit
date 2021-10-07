/*********************************************************************************************************
* Easy Starter Kit Lesson 22: Ultrasonic distance measurement and display
* 
* In this lesson you will learn how to use the ultrasonic sensor to measure distance.
* The sensors maximum distance is 500cm and its minimum distance is 3cm.  You will output
* the measured distance to the 4 digit seven segment display.  The ultrasonic sensor is
* on a blue board with two silver cylinders that have a screen mesh on them.
* 
* Experiment pointing the sensor at various types of objects from different distances and
* get a feel for how the sensor preforms.
* 
* Use a 4 wire connector to connect the 4 digit display to the yellow port with D4 and D2 on its pin label
* Use a 4 wire connector to connect the ultrasonic sensor to the yellow port with D7 and D8 on its pin label
*
* The following functions are available:

* ultrasonic.readDistance();    //Get the measured distance, and return a float type value in centimeters
* disp.init();                  // initialization
* disp.display(int Decimal);    // display range: -999 ~ 9999
************************************************** **************************************************/

#include <EasyStarterKitUltrasonic.h>   //include the ultrasonic sensor library
#include <EasyStarterKitTM1637.h>       //include the 4 digit display library

#define CLK 4                 //CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                 //DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);         //setup object for the 4 digit display

#define TRIGGER_PIN  7        //Trig pin of ultrasonic sensor module connects to D7 of Arduino
#define ECHO_PIN     8        //Echo pin of ultrasonic sensor module connects to D8 of Arduino
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);   //setup object for ultrasonic sensor

void setup(){                   //setup runs once when the program first starts
  disp.init();                  //initialize the 4 digit display
}

void loop(){                                //loop repeats over and over
  float distanceCM;                         //a variable to store the measured distance in
  distanceCM = ultrasonic.readDistance();   //if out of range, it return -1;
  disp.display((int)distanceCM);            //in centimeters
  delay(500);                               //wait half a second
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/