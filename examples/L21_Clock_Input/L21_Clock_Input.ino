/*********************************************************************************************************
* Easy Starter Kit Lesson 19: Clock input
* 
* Note: Make sure you have plugged the CR1220 battery onto the battery holder.
* 
* Learn to use the touch button to enter the year, month, day, week, hour, minute, to initialize the clock module data,
*  and you can see whether your setting is ok from the serial monitor of Arduino IDE.
*
* The following functions are available:

* timer1.initialize(unsigned long microseconds);    // set the timed length, the unit is subtle
* timer1.attachInterrupt (TimingISR);               // set interrupt routine function name, is the timing interrupt entry
* timer1.stop();                                    // Timer 1 stops timing
* timer1.start();                                   // Start Timer 1

* lcd.init();                                       // I2C 1602 LCD screen initialization
* lcd.backlight();                                  //turn on the backlight
* lcd.noBacklight();                                //turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);          // set the cursor position,col is column
* lcd.print(Sring str);                             //display string
* lcd.clearRow(uint8_t row);                        //clear a row of the sreen, row can be 0 or 1

* clock.begin();                                    // The clock is on and the function must be called first
* clock.startClock(void);                           // clock start, if you stop the clock need to call this function
* clock.stopClock(void);                            // clock stop timing
* clock.getTime(void);                              // read the clock and time, will be saved in the clock class variable
* clock.fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);     // write minutes
* clock.fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);        // write clock
* clock.fillDayOfWeek(uint8_t _dow);                // write week
* clock.setTime(void);                              // write the clock and time that have been written to the clock chip

* touch.get();                                      // return is the touch area corresponding Arduino pin number, if not then return -1
* touch.getLongPress();                             // return long press the corresponding Arduino pin number, if not return -1
************************************************** ************************************************** *****/

#include <TimerOne.h>                       //include the TimerOne library
#include <Wire.h>                           //include the I2C library
#include <EasyStarterKitTouchSensor.h>      //include the touch sensor library
#include <EasyStarterKitDS1307.h>           //include the real time clock library
#include <EasyStarterKit_LCD1602.h>         //include the LCD display library

LiquidCrystal_I2C lcd(0x20);                //set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							                              //if 0x20 does not work, please modify it to be 0x38

DS1307 clock;                               //define a object of DS1307 class

#define TOUCH_OUT1 10                       //the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13                       //the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11                       //the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12                       //the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);      //setup the touch object


uint8_t flag_flash = 0;                     //flashing display flag, 0 = off, 1 = on
uint8_t flag_ok = 0;                        //confirm flag, 1 = just press the OK button
uint8_t flag_next = 0;                      //next step flag, 1 = just press the NEXT button
//uint8_t flag_change = 0;                    //digital modification flag, 1 = number can be modified by key, 0 = number can not be modified by key
uint8_t flag_write = 0;                     //1 = You can write the clock and time to the DS1307 clock module

bool SET_DATE_TIME = false;                 

uint8_t next = 1;                           // Each time you press the button, it increments by one, and if it is equal to 7 it returns to 1
#define YEAR      1 
#define MONTH     2
#define DAY       3
#define DAYOFWEEK 4
#define HOUR      5
#define MINUTE    6

int num = 9999;

uint32_t lcdUpdateTimer = 0;

void setup(){

  lcd.init();	                            //initialize the lcd   
  lcd.backlight();                        //turn on the backlight
  clock.begin();                          //initialize the Real Time Clock (RTC)
  Timer1.initialize(500000);              //initialize the timer for 500ms
  Timer1.attachInterrupt(TimingISR);      //attach TimingISR() to the interrupt
  Timer1.stop();                          //stop the timer
  flag_flash = 1;                         //
  
  clock.year = 2015;                      // The clock initial value
  clock.month = 6;
  clock.dayOfMonth = 15;
  clock.dayOfWeek = 4;
  clock.hour = 12;                        // The time initial value is set to 12:30
  clock.minute = 30;

  lcd.clear();
  lcd.autoscroll();
  lcd.setCursor(0, 0);
  lcd.print("Press 1 to set date and time");
  uint32_t startupTimer = millis();
  while(millis() - startupTimer < 8000){
    for(int i = 0; i < 3; i++){
      lcd.scrollDisplayLeft();
    }
    delay(800);
  }
  lcd.clear();
  lcd.noAutoscroll();

  lcdUpdateTimer = millis();
}

void loop(){

  getUserInput();             //check the user input

  if(SET_DATE_TIME){          //if the user pressed the 1 then set the date and time
    lcd.clear();
    setDateTime();
    SET_DATE_TIME = false;
  }

  if(millis() - lcdUpdateTimer >= 1000){    //if it has been 1 second update the clock
    lcdUpdateTimer = millis();
    printTime();
  }
}

void getUserInput(){
  int button;
  button = touch.get();
  if(button != -1){                           //if there is a touch
    delay(10);
    if(button == touch.get()){                //Check if there is still the same touch
      switch(button){
        case TOUCH_OUT1:                      //TOUCH_OUT1 is the set date and time button
          SET_DATE_TIME = true;
          break;
        default:                              //we don't care about the other buttons in this section
          break;
      }
    }
    delay(10);
    while(button == touch.get());               //Wait for the button to be released
  }
}
  
void printTime(){                               //user defined function to print the time from the RTC to the LCD
  String row0 = "";                             //a string variable for row0 of the LCD
  String row1 = "";                             //a string variable for row1 of the LCD
  String hourStr = "";                          //a string variable for the hours
  String minuteStr = "";                        //a string variable for the minutes
  String secondStr = "";                        //a string variable for the seconds
  String monthStr = "";                         //a string variable for the month
  String dayStr = "";                           //a string variable for the day of the month
  String dayNameStr = "";                       //a string variable for the week day 
  
  clock.getTime();                              //get the time from the RTC chip and store it in the clock object variables

  if(clock.hour < 10){                          //if the hour is less than 10
    hourStr = "0" + String(clock.hour, DEC);    //add a leading "0" so it always displays two digits
  }
  else{                                         //it was 10 or larger so...
    hourStr = String(clock.hour, DEC);          // no need to add the leading "0"
  }
  if(clock.minute < 10){                          //if the minute is less than 10
    minuteStr = "0" + String(clock.minute, DEC);  //add a leading "0"
  }
  else{
    minuteStr = String(clock.minute, DEC);
  }
  if(clock.second < 10){                          //if the second is less than 10
    secondStr = "0" + String(clock.second, DEC);  //add a leading "0"
  }
  else{
    secondStr = String(clock.second, DEC);
  }
  row0 = hourStr + ":" + minuteStr + ":" + secondStr;   //build the string for row 0
  lcd.setCursor(0, 0);                                  //set the cursor to column 0, row 0
  lcd.print(row0);                                      //print row0

  if(clock.month < 10){                                 //if the month is less than 10
    monthStr = "0" + String(clock.month, DEC);          //add a leading "0"
  }
  else{
    monthStr = String(clock.month, DEC);
  }
  if(clock.dayOfMonth < 10){                            //if the day of the month is less than 10
    dayStr = "0" + String(clock.dayOfMonth, DEC);       //add a leading "0"
  }
  else{
    dayStr = String(clock.dayOfMonth, DEC);
  }
  row1 = String(clock.year+2000, DEC) + "/" + monthStr + "/" + dayStr + " ";        //build row1 string
                                                                                    //Note the RTC can only store up to 99 for the year 
                                                                                    //so we need to add the melenium and centery to it
  switch(clock.dayOfWeek){                    //add the weekday to row1 string
    case MON: row1 = row1 + "MON"; break;
    case TUE: row1 = row1 + "TUE"; break;
    case WED: row1 = row1 + "WED"; break;
    case THU: row1 = row1 + "THU"; break;
    case FRI: row1 = row1 + "FRI"; break;
    case SAT: row1 = row1 + "SAT"; break;
    case SUN: row1 = row1 + "SUN"; break;
    default: row1 = row1 + "ERR"; break;
  }
  lcd.setCursor(0, 1);                        //set the cursor to column 0, row 1
  lcd.print(row1);                            //print row1
}

void setDateTime(){
  Timer1.start();                               //start timer1 so the value being adjusted flashes
  while(true){                                  //setup an endless loop so this function can get user input and act on it
    int button;                                 //a variable to store the initial button press
    button = touch.get();                       //store the initial button press
    if(button != -1){                           //if there is a touch (wasn't -1)
      delay(10);                                //wait 10 ms
      if(button == touch.get()){                //then check if there is still the same touch
        switch(button){                         //a switch/case to act on different button presses
//          case TOUCH_OUT1:                      //TOUCH_OUT1 is the confirm button (ok)
//            if(next == 6)flag_write = 1;
//            flag_ok = 1;
//            flag_change = 0;                    //Has been confirmed, can not be adjusted
//            break;
          
          case TOUCH_OUT2:                      //touch TCH2 area corresponds to the number increase
//            if(flag_change)num++;               //touch TCH2 area corresponds to the number increase
            num++;
            break;     
          
          case TOUCH_OUT3:                      //touch TCH3 area corresponds to the number decrease
//            if(flag_change)num--;               //touch TCH3 area corresponds to the number decrease
            num--;
            break;
  
          case TOUCH_OUT4:                      //touch TCH4 area corresponds to the next button
            next++;                             //increment next 
            flag_next = 1;                      //set the flag to move to the next value
            flag_ok = 1;
//            flag_change = 1;                    //You can adjust the number
            if(next == 7)flag_write = 1;        //if next = 7 then we have set all six values and need to finish
            break;                              //touch TCH4 area, each time you press the button will enter the next set process
      
          default:
            break;
        }
        if(next == 8)next = 1;
      }
      delay(10);
      while(button == touch.get());               //Wait for the button to be released
    }                                           //end if touched
  
    //flash the value being modified
    lcd.setCursor(0, 1);
    if(flag_flash) lcd.print(num);
    else lcd.clearRow(1);
  
    if(flag_ok){
        switch(next-1){
          case YEAR: clock.year=num; break;
          case MONTH: clock.month=num;break;
          case DAY: clock.dayOfMonth=num;break;
          case DAYOFWEEK: clock.dayOfWeek=num;break;
          case HOUR: clock.hour=num;break;
          case MINUTE: clock.minute=num;break;
          default: break;
        }
        flag_ok = 0;
      }
    
    if((next!=0) && (next < 8)){                //check to make sure next is within bounds
      if(flag_next){                            //if we are moving to the next value                                     
        flag_next = 0;                          //clear the next flag so this doesn't get repeated
        lcd.clearRow(0);                        //clear row 0 of LCD
        
        switch(next){                           //switch/case for the value we are on
          case YEAR:                            //net num and print year from RTC
            num = clock.year+2000; 
            lcd.print("year is ");
            break;
          case MONTH:                           //set num and print month from RTC
            num = clock.month;
            lcd.print("month is ");
            break;
          case DAY:                             //set num and print day from RTC
            num = clock.dayOfMonth;
            lcd.print("day Of Month is ");
            break;
          case DAYOFWEEK:                       //set num and print week day from RTC
            num = clock.dayOfWeek;
            lcd.print("day Of Week is ");
            break;
          case HOUR:                            //set num and print hour from RTC
            num = clock.hour;
            lcd.print("hour is ");
            break;
          case MINUTE:                          //set num and print minute from RTC
            num = clock.minute;
            lcd.print("minute is ");
            break;
          default: break;
        }//end of switch/case
      }//end if(flag_next)
    
                                           //end if(flag_ok)
    
      if(flag_write){                       //once all values have been set flag_write will be 1 (true)
        num = 0;
        flag_write = 0;
        clock.fillByYMD(clock.year,clock.month,clock.dayOfMonth);       //
        clock.fillDayOfWeek(clock.dayOfWeek);                           //
        clock.fillByHMS(clock.hour,clock.minute,0);                     //
        clock.setTime();                                                //write time to the RTC chip
        
        Timer1.stop();                      //stop the timer so things don't flash once done setting the time
        flag_flash = 1;                     //make sure it is left on
        
        lcd.clear();                        //clear the LCD
        lcd.setCursor(0, 0);                //set the cursor to column 0, row 0
        lcd.print("Time and date");         //print message
        lcd.setCursor(0, 1);                //set the cursor to column 0, row 1
        lcd.print("have been set!");        //print the rest of the message
        delay(2000);                        //give the user 2 seconds to read the message
        lcd.clear();                        //clear the display
        return;                             //return from the setDateTime() function
      }                                     //end if(flag_write)
    }                     
  }             //end of while(true)
}     //end of setDateTime()


/**********Timer 1 interrupt routine*********/
void TimingISR(){
  flag_flash = !flag_flash;
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
