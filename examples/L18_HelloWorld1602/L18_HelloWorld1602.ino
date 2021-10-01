/***************************************************************************************************
* Easy Starter Kit Lesson 18: I2C 1602 LCD Display
* In this lesson you will learn how to use the I2C 1602 LCD Display.  The 1602 display has 16 columns
* and 2 rows, hence 1602. The LCD is on a green board and has a blue and black display on it.  The 
* connector port is on the back of it.
* 
* Use a 4 wire connector and connect the LCD to one of the yellow ports labeled I2C
* 
* Make sure the voltage setting next to the USB port on the main board is set to 5 volts
*
* The following functions are available:

* lcd.init();                                   //I2C 1602 LCD screen initialization
* lcd.backlight();                              //turn on the back light
* lcd.noBacklight();                            //turn off the back light
* lcd.setCursor(uint8_t col, uint8_t row);      //set the cursor position,col is column
*                                               //column and row are 0 indexed so the first position is 0, the second is 1, etc.
* lcd.print(String str);                        //display a string. Strings that have enough characters to go past the end of the
*                                               //may exhibit odd behavior due buffer having more columns and/or rows than the display
* 
* The LCD library offers many more commands that are not covered in this lesson.  Experiment with them
* to discover what they do. Some of the more interesting ones are listed below.
* 
* lcd.clear();
* lcd.clearRow(uint8_t row);
* lcd.home();
* lcd.setCursor(uint8_t col, uint8_t row);
* lcd.cursor();
* lcd.noCursor();
* lcd.blink();
* lcd.noBlink();
* lcd.scrollDisplayLeft();
* lcd.scrollDisplayRight();
* lcd.autoscroll();
* lcd.noAutoscroll();
* 
****************************************************************************************************/

#include <Wire.h>                   //include the I2C library
#include <EasyStarterKit_LCD1602.h> //include LCD library

LiquidCrystal_I2C lcd(0x20);        // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							                      // if 0x20 does not work, please modify it to be 0x38

void setup(){                       // setup() runs once when the program starts
  lcd.init();                       // initialize the lcd 

  lcd.backlight();                  // turn on the back light
  lcd.setCursor(0, 0);              // set the cursor to column 0 and row 0
  lcd.print("Hello, world!");       // print Hello, world! on the display
  lcd.setCursor(0, 1);              // set the cursor to column 0 and row 1
  lcd.print("MakerBox");            // print MakerBox on the display
}

void loop(){                        // loop() is not used by this program but is needed by Arduino
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
