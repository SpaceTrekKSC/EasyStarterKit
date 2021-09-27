/**************************************************************************************************
* Easy Starter Kit Lesson 10: Slide Potentiometer and RGB LED
* In this lesson you will learn how to use the slide potentiometer.  You will take the slide 
* potentiometer input and map it into the brightness of the RED color on the RGB LED used in lesson 9. 
* As you move the slider the brightness of the LED will change.
* 
* Use a 3 pin connector to connect the RGB LED to the blue port with D3 in its pin label
* 
* Use a 3 wire connector to connect the slide potentiometer to the blue port with A3 in its pin label
*
* The following functions are available:
* 
* rgb.begin();                                                          //this initializes the RGB LED library.
* rgb.setPixelColor(uint8_t r, uint8_t g, uint8_t b, uint16_t n=0);     //r, g, b should be 0~255, n is the position of the RGB led but can be omitted for a single LEG.
* rgb.show();                                                           //after you have set the color of all the RGB leds, you should run this to display.
* slider.getPosition();                                                 //this will return the position of the slider, the value will be 0~100.

***************************************************************************************************/

#include <EasyStarterKitRGB.h>                  //include the RGB library
#include <EasyStarterKitSlider.h>               //include the slider library

#define RGB_PIN     3                           //DIN pin of the module connect to D3 of Arduino
#define NUMpixcels  1                           //number of RGB LED pixels 
RGB rgb(NUMpixcels, RGB_PIN);                   //setup the RGB object

#define SLIDER_PIN A3                           //SIG pin of Slide Potentiometer module connect to A3 of IO Shield, that is pin A3 of Arduino
Slider slider(SLIDER_PIN);                      //setup the slider object

int brightness = 100;                           //declare global variable brightness with initial value of 100

void setup(){                                   //setup() runs once when the program is first started
  rgb.begin();                                  //this initializes the RGB LED object
  rgb.setPixelColor(brightness, 0, 0);          //this sets the initial red brightness to 100
  rgb.show();                                   //this sends the updated pixel color to the hardware
}

void loop(){                                    //loop() runs over and over
  int position;                                 //declare local variable position
  position = slider.getPosition();              //gets the slider position and stores it in the local variable position
  brightness = map(position, 0, 100, 0, 255);   //converts the o-100 range of position into the needed 0-255 range of brightness
  
  rgb.setPixelColor(brightness, 0, 0);          //sets the RGB red value to the mapped brightness value
  rgb.show();                                   //this sends the updated pixel color to the hardware.
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
