/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 20: I2C 1602 LCD Display Clock
* You can learn to use I2C 1602 LCD to display time.
*
* The following functions are available:

* Timer1.initialize(unsigned long microseconds); // set the timed length, the unit is microseconds
* Timer1.attachInterrupt(TimingISR); // set interrupt routine function name, is the timing interrupt entry

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string

* clock.begin(); // The clock is on and the function must be called first
* clock.startClock(void); // clock start, if you stop the clock need to call this function
* clock.stopClock(void); // clock stop timing
* clock.getTime(void); / / read the clock and time, will be saved in the clock class variable
* clock.fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second); // write minutes
* clock.fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day); // write clock
* clock.fillDayOfWeek(uint8_t _dow); // write week
* clock.setTime(void); // write the clock and time that have been written to the clock chip
************************************************** **************************************************/

#include <TimerOne.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitDS1307.h"
#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38

DS1307 clock;//define a object of DS1307 object

#define ON 1
#define OFF 0

uint8_t Update = ON;
uint8_t halfsecond = 0;

void setup() {
  clock.begin();
  
//  clock.fillByYMD(2019,8,3);//2017.08.09
//  clock.fillByHMS(15,13,10);//17:01:10
//  clock.fillDayOfWeek(SAT);//Saturday,The first three letters
//  clock.setTime();//write time to the RTC chip

  lcd.init();	 // initialize the lcd 
  lcd.backlight();//turn on the backlight
  
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR 
  
}

void loop() {

  if(Update == ON)
  {
    DataUpdate();
  }
  
}

void TimingISR()
{
  halfsecond++;
  if(halfsecond == 2)
  {
    Update = ON;
	halfsecond = 0;
  }
}


void DataUpdate()
{
  clock.getTime();
  lcd.setCursor(0, 0);//the first place of the first line
  lcd.print("Time: ");
  if(clock.hour < 10)lcd.print(" ");
  lcd.print(clock.hour, DEC);
  lcd.print(":");
  if(clock.minute < 10)lcd.print("0");
  lcd.print(clock.minute, DEC);
  lcd.print(":");
  if(clock.second < 10)lcd.print("0");
  lcd.print(clock.second, DEC);
  
  lcd.setCursor(0, 1);//the first place of the second line
  lcd.print("Date: ");
  lcd.print(clock.year+2000, DEC);
  lcd.print(".");
  if(clock.month < 10)lcd.print("0");
  lcd.print(clock.month, DEC);
  lcd.print(".");
  if(clock.dayOfMonth < 10)lcd.print("0");
  lcd.print(clock.dayOfMonth, DEC);
  
  
	
  Update = OFF;
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
