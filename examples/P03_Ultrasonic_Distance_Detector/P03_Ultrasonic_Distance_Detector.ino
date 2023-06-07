/*********************************************************************************************************
* Easy Starter Kit Project 03: Ultrasonic Distance Detector
* 
* For this project you will build a distance detector for robots or drones that uses the ultrasonic 
* distance sensor to detect when something is close.  Once the object is close to the untrasonic sensor 
* the LED will change from GREEN to RED. Once it moves away the led will turn back to GREEN. 
*
* You will use what you learned in lesson L09 and L22.
*
* Use a 3 wire connector to connect the RGB LED to the blue port that has a D5 on its pin label
* Use a 4 wire connector to connect the ultrasonic sensor to the yellow port with D7 and D8 on its pin label
*
*
* The following functions are available:
*
* rgb.begin();                                  //this initializes the RGB LED object
* rgb.setPixelColor(r, g, b, n=0);              //r, g, b should be 0~255, n is the position of the RGB led but can be omitted for a single LED.
* rgb.show();                                   //after you have set the color of all the RGB LEDs, you should run this to display.
*
* ultrasonic.readDistance();                    //Get the measured distance, and return a float type value in centimeters
*
****************************************************************************************************/

//All libraries have been included for you
#include <EasyStarterKitRGB.h>                  //include the RGB library
#include <EasyStarterKitUltrasonic.h>           //include the ultrasonic sensor library

//All objects have been setup for you
#define RGB_PIN     5                           //DIN pin of the module connect to D3 of Arduino
#define NUMpixcels  1                           //number of RGB LED pixels 
RGB rgb(NUMpixcels, RGB_PIN);                   //setup the RGB object

#define TRIGGER_PIN  7                          //Trig pin of ultrasonic sensor module connects to D7 of Arduino
#define ECHO_PIN     8                          //Echo pin of ultrasonic sensor module connects to D8 of Arduino
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);   //setup object for ultrasonic sensor

//fixed value settings - Adjust these to change how your distance detector works
#define DETECT_DISTANCE 50.0                    //set our distance to 50cm (values between 20.0 to 100 should work well)

//global variables
float distanceCM = 0.0;                         //a variable to store the measured distance in

//setup function runs once at the begining of the program
void setup() {
  rgb.begin();                                  //this initializes the RGB LED object
  rgb.setPixelColor(0, 200, 0);                 //start the gate with the LED GREEN
  rgb.show();                                   //update the RGB LED so it shows the new color
}//end of setup()

//loop function runs over and over in a loop
void loop() {
  distanceCM = ultrasonic.readDistance();           //read the ultrasonic sensor (if it is out of range, it returns -1)
  if(distanceCM < 0){                               //if it is out of range...
    distanceCM = 999.0;                             //change it to a large value so it does not trip the gate
  }

  //use an if() statment to check if distanceCM is less
  //than DETECT_DISTANCE.  If it is then set the RGB LED
  //to RED (200, 0, 0).  Else set it to GREEN.  Remember 
  //to show the LED after you set the color.

  



  delay(20);                                        //a short delay to slow the loop down so it only runs 50 times per second
}//end of loop()
