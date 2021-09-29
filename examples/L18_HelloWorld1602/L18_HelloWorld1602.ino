/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 18: I2C 1602 LCD Display
* You can learn to use I2C 1602 LCD to display number, character.
*
* The following functions are available:

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
************************************************** **************************************************/

#include <Wire.h> 
#include <SoftwareSerial.h>

#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38

void setup()
{
  lcd.init();                      // initialize the lcd 

  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Hello, world!");
  lcd.setCursor(0, 1);
  lcd.print("MakerBox");
}

void loop()
{
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
