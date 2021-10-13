/*********************************************************************************************************
* Easy Starter Kit Lesson 24: Test infrared remote keys 
*
* In this lesson you will learn how to get the keys code of the buttons on the IR remote and display
* them on the 1602 LCD. The infrared receiver is on a blue board.
* 
* Use a 3 wire connector to connect the IR receiver to the blue port with D3 in its pin label
* Use a 4 wire connector to connect the LCD to one of the yellow I2C ports
*
* The following functions are available:
* IR.enableIRIn();                              / Start the receiver
* IR.decode();                                  //if no result, it return 0, otherwise it return 1; .
* IR.resume();                                  //so that it can receive the next value
* IR.isReleased();                              //if the button is not released yet, it return 0; otherwise it return 1;

* lcd.init();                                   //I2C 1602 LCD screen initialization
* lcd.backlight();                              //turn on the backlight
* lcd.noBacklight();                            //turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);      //set the cursor position,col is column
* lcd.print(String str);                         //display string
* lcd.clear();                                  //clear the sreen

*********************************************************************************************************/

#include <Wire.h>
#include <EasyStarterKitIRremote.h>
#include <EasyStarterKit_LCD1602.h>

LiquidCrystal_I2C lcd(0x20);              //set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							                            //if 0x20 does not work, please modify it to be 0x38
						
#define RECV_PIN 3                        //the SIG pin of Infrared Receiver is connected to D3
IRrecv IR(RECV_PIN);                      //setup the IR receiver object

void setup(){                             //setup() runs once when the program first starts
  lcd.init();                             //initialize the LCD
  lcd.backlight();                        //turn on the backlight
  IR.enableIRIn();                        //Start the receiver
}

void loop(){                              //loop() runs over and over
  if (IR.decode()){                       //If a valid key code is detected
    if(IR.isReleased()){                  //If it is not the repeat code for long press, then print out to display
      lcd.clear();                        //clear the LCD
      lcd.setCursor(0, 0);                //set the cursor to column 0, row 0
      lcd.print("user code: 0x");         //print user code text
      lcd.print(IR.usercode,HEX);         //print the user code in HEX
      lcd.setCursor(0, 1);                //set the cursor to column 0 row 1
      lcd.print("key code: 0x");          //print key code text
      lcd.print(IR.keycode,HEX);          //print the key code in HEX
    }
    IR.resume(); //resume to receive again
  }
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
