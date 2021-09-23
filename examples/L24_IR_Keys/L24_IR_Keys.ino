/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 24: Test infrared remote keys 
*
* Learn how to get keys code of the buttons on the IR remote and display on 1602 LCD.
*
* The following functions are available:
* IR.enableIRIn(); // Start the receiver
* IR.decode();//if no result, it return 0, otherwise it return 1; .
* IR.resume(); // so that it can receive the next value
* IR.isReleased();//if the button is not released yet, it return 0; otherwise it return 1;

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
* lcd.clear();//clear the sreen

*********************************************************************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitIRremote.h"
#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38
						
#define RECV_PIN 3//the SIG pin of Infrared Receiver is connected with D3 of OPEN-SMART UNO R3
IRrecv IR(RECV_PIN);

void setup()
{
  lcd.init();	 
  lcd.backlight();//turn on the backlight
  IR.enableIRIn(); // Start the receiver
}

void loop()
{
  if (IR.decode()) {//If a valid key code is detected
  
    if(IR.isReleased())//If it is not the repeat code for long press, then read out to display
	{
	  lcd.clear();
	  lcd.setCursor(0, 0);
      lcd.print("user code: 0x");
      lcd.print(IR.usercode,HEX);
	  lcd.setCursor(0, 1);
      lcd.print("key code: 0x");
      lcd.print(IR.keycode,HEX);
    }
    IR.resume(); //resume to receive again
  }
}


/*********************************************************************************************************
The end of file
*********************************************************************************************************/
