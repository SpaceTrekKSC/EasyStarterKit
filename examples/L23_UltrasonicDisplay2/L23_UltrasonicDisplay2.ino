/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 23: Ultrasonic distance measurement and display Part 2
* You can learn to use  the ultrasonic distance measurement module to measure the distance 
* of the nearest obstacle in front, the maximum measurement is 500 cm, at least 3 cm, 
* the I2C 1602 LCD will display the value.
*
* The following functions are available:

* ultrasonic.readDistance();//Get the measured distance, and return a float type value in centimeters
* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.clear();     //clear the display
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
************************************************** **************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitUltrasonic.h"
#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38

//module--- Arduino
//VCC   --- 5V
//GND   --- GND
#define TRIGGER_PIN  7//Trig pin of ultrasonic sensor module connect to D7 of Arduino
#define ECHO_PIN     8//Echo to D8
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);


void setup()
{
  lcd.init();	 
  lcd.backlight();//turn on the backlight
}

void loop()
{
  float cmMsec;  
  cmMsec = ultrasonic.readDistance();//if out of range, it return -1;
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(cmMsec,0);
  lcd.print(" cm");
  delay(1000);
}
