/*********************************************************************************************************
* Easy Starter Kit Lesson 23: Ultrasonic distance measurement and display Part 2
* 
* In this lesson you will learn how to use the ultrasonic sensor to measure distance.
* The sensors maximum distance is 500cm and its minimum distance is 3cm.  You will output
* the measured distance to the LCD display.  The ultrasonic sensor is on a blue board
* with two silver cylinders that have a screen mesh on them.
* 
* Use a 4 wire connector to connect the LCD to one of the yellow I2C ports
* Use a 4 wire connector to connect the ultrasonic sensor to the yellow port with D7 and D8 on its pin label
* 
* The following functions are available:
*
* ultrasonic.readDistance();                  //Get the measured distance, and return a float type value in centimeters
*                                             //returns - 1 if out of range
*                                             
* lcd.init();                                 //I2C 1602 LCD screen initialization
* lcd.clear();                                //clear the display
* lcd.backlight();                            //turn on the backlight
* lcd.noBacklight();                          //turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);    //set the cursor position
* lcd.print(String str);                       //display string
************************************************** **************************************************/

#include <Wire.h>
#include <EasyStarterKitUltrasonic.h>
#include <EasyStarterKit_LCD1602.h>

LiquidCrystal_I2C lcd(0x20);            //set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							                          //if 0x20 does not work, please modify it to be 0x38

#define TRIGGER_PIN  7                  //Trig pin of ultrasonic sensor module connect to D7 of Arduino
#define ECHO_PIN     8                  //Echo pin of ultrasonic sensor module connect to D8 of Arduino
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);   //setup object for ultrasonic sensor

void setup(){                           //setup runs once when the program first starts
  lcd.init();	                          //initialize the LCD
  lcd.backlight();                      //turn on the backlight
}

void loop(){                            //loop repeats over and over
  float cmMsec;                         //a variable to store the measured distance in
  cmMsec = ultrasonic.readDistance();   //if out of range, it return -1;
  lcd.setCursor(0, 0);                  //set the cursor to column 0, row 0
  lcd.clear();                          //clear the lcd
  lcd.print(cmMsec,0);                  //print the measured distance
  lcd.print(" cm");                     //print the units
  delay(1000);                          //wait 1 second
}

/*
 * This program will cause the LCD to flicker.  See if you can use what you learned in lesson 21
 * to prevent the flickering.  Hint, you will need to account for up to two additional digits and
 * the negative sign.
 */
 
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
