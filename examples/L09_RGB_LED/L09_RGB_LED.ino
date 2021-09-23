/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 9: RGB LED control
* You can learn how to control the color of the RGB LED. 
*
* The following functions are available:
*
* rgb.begin();//This initializes the RGB LED library.
* rgb.setPixelColor(uint8_t r, uint8_t g, uint8_t b, uint16_t n=0);//r, g, b should be 0~255, n is the position of the RGB led.
* rgb.show(); // after you have set the color of all the RGB leds, you should run this to display.

*********************************************************************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>
#include "EasyStarterKitRGB.h"

#define RGB_PIN 3          //DIN pin of the module connect to D3 of Arduino
// How many Neorgb are attached to the Arduino
#define NUMpixcels      1
RGB rgb(NUMpixcels, RGB_PIN);

void setup() {

  rgb.begin(); // This initializes the RGB LED library.
}

void loop() {

  /*Function: Send RGB(the duty cycle) to tm1804	 */
  /*Parameters: green:0~255 				    */
  /*            red:0~255						    */
  /*            blue:0~255						    */

  rgb.setPixelColor(100, 0, 0); // Red.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.

  rgb.setPixelColor(0, 100, 0); // Green.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.

  rgb.setPixelColor(0, 0, 100); // Blue.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.

  rgb.setPixelColor(100, 100, 0); // Yellow.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.

  rgb.setPixelColor(0, 100, 100); // CYAN.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.

  rgb.setPixelColor(100, 0, 100); // MAGENTA.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.

  rgb.setPixelColor(100,100,100); // WHITE.
  rgb.show(); // This sends the updated pixel color to the hardware.
  delay(1000); // Delay 1000ms.
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
