/*********************************************************************************************************
* Easy Starter Kit Project 01: Traffic Light
* 
* For this project you will build a simple traffic light that cycles between GREEN, YELLOW and RED.
* There will be a cross walk button that keeps the light GREEN for a longer period of time.
*
* You will use what you learned in lesson L09 as well as a new Arduino frunction attachInterrupt().
* The attachInterrupt function allwas us to capture a button press as any time during our program
* so we can detect the user pressing it even if our program is waiting in a delay() function.
*
* Use a 3 wire connector to connect the push button to the blue port that has D3 on its pin labels
* Only pins 2 and 3 work with attachInterrup so the button MUST be on pin D3.
*
* Use a 3 wire connector to connect the RGB LED to the blue port that has a D5 on its pin label
*
*
* The following functions are available:
*
* rgb.begin();                                                        //this initializes the RGB LED library.
* rgb.setPixelColor(uint8_t r, uint8_t g, uint8_t b, uint16_t n=0);   //r, g, b should be 0~255, n is the position of the RGB led but can be omitted for a single LEG.
* rgb.show();                                                         //after you have set the color of all the RGB LEDs, you should run this to display.
*
* delay(time);                                                        //this pauses the program for 'time' milliseconds
****************************************************************************************************/

//included libraries
#include <EasyStarterKitRGB.h>                  //include the RGB library

//object setup
#define BUTTON_PIN 3                            //SIG pin of push button connect to D3 of Arduino

#define RGB_PIN     5                           //DIN pin of the module connect to D5 of Arduino
#define NUMpixcels  1                           //number of RGB LED pixels 
RGB rgb(NUMpixcels, RGB_PIN);                   //setup the RGB object

//fixed value settings
#define GREEN_TIME        3000
#define CROSSWALK_TIME    20000
#define YELLOW_TIME       1000
#define RED_TIME          3000

//global variables
volatile bool Crosswalk = false;

//setup function runs once at the begining of the program
void setup() {
  pinMode(BUTTON_PIN, INPUT);     //this command tells Arduino that the button pin is going to be an input
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), CrosswalkISR, CHANGE);   //this command makes the CrosswalkIRS function be called
                                                                              //every time the button changes state, even if the program
                                                                              //is doing something else when it is pressed.
  rgb.begin();       //this initializes the RGB LED object
}//end of setup()

//loop function runs over and over in a loop
void loop() {
  if(Crosswalk){
    //use an if else statment to check if Crosswalk has been set to true
    //by the interrupt.  If it has been set to true, set it back to false
    //and make the RGB LED stay GREEN for CROSSWALK_TIME 

  }
  else{
    //else make the RGB LED stay GREEN for GREEN_TIME.
    
  }

  //set the RGB LED to YELLOW for YELLOW_TIME
  // (150, 200, 0) makes a nice yellow color
  //don't forget to show the RGB LED after you set the color
  

  //set the RGB LED to RED for RED_TIME
  //don't forget to show the RGB LED after you set the color
  
}//end of loop()

void CrosswalkISR(){
  Crosswalk = true;
}
