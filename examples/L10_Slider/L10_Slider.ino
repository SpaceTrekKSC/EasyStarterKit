/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 10: Slide Potentiometer and RGB LED
* You can learn to use the slide potentiometer to control the brightness of the RGB LED.
*
* The following functions are available:
* 
* rgb.begin();//This initializes the RGB LED library.
* rgb.setPixelColor(uint8_t r, uint8_t g, uint8_t b, uint16_t n=0);//r, g, b should be 0~255, n is the position of the RGB led.
* rgb.show(); // after you have set the color of all the RGB leds, you should run this to display.
* slider.getPosition();// This will return the position of the slider, the value should be 0~100.

*********************************************************************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitRGB.h"
#include "EasyStarterKitSlider.h"

#define RGB_PIN 3          //DIN pin of the module connect to D3 of Arduino
// How many Neorgb are attached to the Arduino
#define NUMpixcels      1
RGB rgb(NUMpixcels, RGB_PIN);

#define SLIDER_PIN A3//SIG pin of Slide Potentiometer module connect to A3 of IO Shield, that is pin A3 of Arduino
Slider slider(SLIDER_PIN);

int brightness = 100;
void setup()
{
  rgb.begin(); // This initializes the NeoPixel library.
  rgb.setPixelColor(brightness, 0, 0); // Red.
  rgb.show(); // This sends the updated pixel color to the hardware.
}

void loop()
{

  int position;
  position = slider.getPosition();//0~100
  brightness = map(position, 0, 100, 0, 255);
  
  rgb.setPixelColor(brightness, 0, 0); // Red.
  rgb.show(); // This sends the updated pixel color to the hardware.
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
