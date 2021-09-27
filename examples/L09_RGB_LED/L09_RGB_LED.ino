/**************************************************************************************************
* Easy Starter Kit Lesson 9: RGB LED control
* In this lesson you will learn how to control an RGB (Red, Green, Blue) LED. There are a few 
* different types of RGB LEDs. Some you connect to the pin for each color, others have additional
* digital hardware that allows you to connect to a single pin and control multiple LEDs at one.
* The one in this kit is digitally controlled. The RGB LED is on a white board with a semi translucent
* white LED and black microchip on it.
* 
* Use a 3 wire connector to connect the RGB LED to the blue port that has a D3 on its pin label
*
* The following functions are available:
*
* rgb.begin();                                                        //This initializes the RGB LED library.
* rgb.setPixelColor(uint8_t r, uint8_t g, uint8_t b, uint16_t n=0);   //r, g, b should be 0~255, n is the position of the RGB led but can be omitted for a single LEG.
* rgb.show();                                                         // after you have set the color of all the RGB leds, you should run this to display.

***************************************************************************************************/

#include <EasyStarterKitRGB.h>

#define RGB_PIN     3             //DIN pin of the module connect to D3 of Arduino
#define NUMpixcels  1             //number of RGB LED pixels 
RGB rgb(NUMpixcels, RGB_PIN);     //setup the RGB object

void setup(){                     //setup() runs once when the program is first started
  rgb.begin();                    //this initializes the RGB LED object.
}

void loop(){              
  /* How to use setPixelColor(r, g, b)              */
  /* Function: sets each of the colors in memory    */
  /* Parameters:  green:0~255                       */
  /*              red:0~255                         */
  /*              blue:0~255                        */

  rgb.setPixelColor(100, 0, 0);     // Red.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.

  rgb.setPixelColor(0, 100, 0);     // Green.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.

  rgb.setPixelColor(0, 0, 100);     // Blue.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.

  rgb.setPixelColor(100, 100, 0);   // Yellow.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.

  rgb.setPixelColor(0, 100, 100);   // CYAN.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.

  rgb.setPixelColor(100, 0, 100);   // MAGENTA.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.

  rgb.setPixelColor(100,100,100);   // WHITE.
  rgb.show();                       // This sends the updated pixel color to the hardware.
  delay(1000);                      // Delay 1000ms.
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
