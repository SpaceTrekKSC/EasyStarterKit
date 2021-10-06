/*******************************************************************************************************
* Easy Starter Kit Lesson 20: I2C 1602 LCD Display Clock
* 
* In this lesson you will learn how to display the time from a Real Time Clock (RTC) on the 1602 LCD.
* It is similar to lesson 19 except now we are only adjusting the time if the RTC is not running and
* we are using the LCD display instead of the 4 digit display.
* 
* Use a 4 wire connector to connect the RTC to one of the yellow I2C ports
* Use a 4 wire connector to connect the LCD to the other yellow I2C port
*
* The following functions are available:
*
* Timer1.initialize(unsigned long microseconds);  //set the timed length, the unit is microseconds
* Timer1.attachInterrupt(TimingISR);              //set interrupt routine function name, is the timing interrupt entry
*
* lcd.init();                                     //I2C 1602 LCD screen initialization
* lcd.backlight();                                //turn on the backlight
* lcd.noBacklight();                              //turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);        //set the cursor position,col is column
* lcd.print(String str);                          //display string

* clock.begin();                  //The clock is on and the function must be called first
* clock.isrunning()               //returns true if the RTC is running or false if it is not
* clock.adjust(DateTime());       //adjusts the date and time on the RTC chip
* now = clock.now();              //sets the now object to the current date and time from the RTC
* 
* now.hour()                      //access the hour from the now object in 24 hour format
* now.twelveHour()                //access the hour from the now object in 12 hour format
* now.minute()                    //access the minute from the now object
* now.second()                    //access the second from the now object
* now.year()                      //access the year from the now object
* now.month()                     //access the month from the now object
* now.day()                       //access the day from the now object
* 
*******************************************************************************************************/

#include <TimerOne.h>                   //include the TimerOne library
#include <Wire.h>                       //include the I2C library
#include <RTClib.h>                     //include the Real Time Clock library
#include <EasyStarterKit_LCD1602.h>     //include the LCD library

LiquidCrystal_I2C lcd(0x20);            //set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
                                        //if 0x20 does not work, please modify it to be 0x38

RTC_DS1307 clock;                       //setup a RTC-DS1307 object named clock
DateTime now;                           //setup am object variable called now of user defined type DateTime which is defined in the RTClib library

#define ON 1                            //define ON to be 1 (true)
#define OFF 0                           //define OFF to be 0 (false)

volatile uint8_t Update = ON;           //declare and initialize the variable Update.  Update is used in an interrupt service routine
                                        //so it is good practice to specify that the variable is volatile.

void setup(){                           //setup runs once when the program is first started
  clock.begin();                        //initialize the RTC object called clock

  /***********************************************************************************/
  /* In lesson 19 we called clock.adjust() every time we ran the program.            */
  /* Now we are going to put it inside an if statement and only call it if the       */
  /* RTC is not running.  Remember, this is the time that the program was compiled   */
  /* so if you take the battery out it will reset the clock to the time the          */
  /* program was compiled                                                            */
  /***********************************************************************************/
  if(!clock.isrunning()){                               //if the clock is not running (i.e. it was reset by removing the battery)
    clock.adjust(DateTime(F(__DATE__), F(__TIME__)));   //update the RTC to the date and time of when the program was compiled

    //clock.adjust(DateTime(2021, 10, 6, 13, 14, 38));  //Or use this line to set the RTC with an explicit date & time.
                                                        //For example, to set the RTC to October 6, 2021 at 1:14:38pm
  }
  
  //or you can use them outside of the if statement to always reset the clock to the compile time
  //clock.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //clock.adjust(DateTime(2021, 10, 6, 13, 14, 38));

  lcd.init();                                 //initialize the lcd 
  lcd.backlight();                            //turn on the backlight
  
  Timer1.initialize(1000000);                 //timing for 1 second
  Timer1.attachInterrupt(TimingISR);          //declare the interrupt serve routine:TimingISR 
  
}

void loop(){              //loop repeats over and over
  if(Update == ON){       //if Update is set to ON
    DataUpdate();         //then update the display
  }
}

void TimingISR(){         //the timer interrupt function
  Update = ON;            //whenever the timer goes off set Update to on
}

void DataUpdate(){                          //a user defined function to update the LCD with the current time and date from the RTC
  now = clock.now();                        //get the time and date from the RTC and store it in the variable now
  lcd.setCursor(0, 0);                      //set the cursor to the first place of the first line
  lcd.print("Time: ");                      //print to "Time: "
  //if(now.hour() < 10)lcd.print(" ");        //print the leading space if needed
  //lcd.print(now.hour(), DEC);               //print the hour in 24 hour format
  if(now.twelveHour() < 10)lcd.print(" ");  //print the leading space if needed
  lcd.print(now.twelveHour(), DEC);         //print the hour in 12 hour format
  lcd.print(":");                           //print the colon between hour and minute
  if(now.minute() < 10)lcd.print("0");      //print the leading zero if needed
  lcd.print(now.minute(), DEC);             //print the minute
  lcd.print(":");                           //print the colon between minute and second
  if(now.second() < 10)lcd.print("0");      //print the leading zero if needed
  lcd.print(now.second(), DEC);             //print the second
  
  lcd.setCursor(0, 1);                      //the first place of the second line
  lcd.print("Date: ");                      //print "Date: "
  lcd.print(now.year(), DEC);               //print the year
  lcd.print("/");                           //print the / between year and month
  if(now.month() < 10)lcd.print("0");       //print the leading zero if needed
  lcd.print(now.month(), DEC);              //print the month
  lcd.print("/");                           //print the / between month and day
  if(now.day() < 10)lcd.print("0");         //print the leading zero if needed
  lcd.print(now.day(), DEC);                //print the day
  
  Update = OFF;                             //turn Update off
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
