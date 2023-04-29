/*********************************************************************************************************
* Easy Starter Kit Lesson 21: Clock input
* 
* Okay, this one is going to be a bit more complicated.  You will learn how to piece several things we 
* have learned together to make a clock that you can adjust the date and time on from the touch sensor.
* The clock will only update its time when you change it or if the RTC has been reset from removing the
* battery. There is a basic User Interface (UI) to get user input from the touch sensor and display it
* on the LCD.
* 
* Use a 4 wire connector to connect the LCD to one of the yellow I2C ports
* Use a 4 wire connector to connect the RTC to the other yellow I1C port
* Use a 6 wire connector to connect the touch sensor to the red port that has MOSI and MISO in its pin labels
*
* The following library functions are used:
*
* timer1.initialize(unsigned long microseconds);    //set the timed length, the unit is subtle
* timer1.attachInterrupt (TimingISR);               //set interrupt routine function name, is the timing interrupt entry
* timer1.stop();                                    //Timer 1 stops timing
* timer1.start();                                   //Start Timer 1
*
* lcd.init();                                       //I2C 1602 LCD screen initialization
* lcd.backlight();                                  //turn on the backlight
* lcd.noBacklight();                                //turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);          //set the cursor position,col is column
* lcd.print(String str);                             //display string
* lcd.clear();                                      //clears the entire LCD
* lcd.clearRow(uint8_t row);                        //clear a row of the screen, row can be 0 or 1
* lcd.autoscroll();                                 //turns on autoscroll
* lcd.scrollDisplayLeft();                          //scrolls auto scroll left one character
* lcd.scrollDisplayRight();                         //scrolls auto scroll right one character
* lcd.noAutoscroll();                               //turns off autoscroll
* 
* clock.begin();                                    //The clock is on and the function must be called first
* clock.isrunning()                                 //returns true if the RTC is running or false if it is not
* clock.adjust(DateTime());                         //adjusts the date and time on the RTC chip
* now = clock.now();                                //sets the now object to the current date and time from the RTC
* 
* now.hour()                                        //access the hour from the now object in 24 hour format
* now.twelveHour()                                  //access the hour from the now object in 12 hour format
* now.minute()                                      //access the minute from the now object
* now.second()                                      //access the second from the now object
* now.year()                                        //access the year from the now object
* now.month()                                       //access the month from the now object
* now.day()                                         //access the day from the now object
* now.dayOfTheWeek()                                //get the day of the week number calculated from January 1st 2000
*
* touch.get();                                      // return is the touch area corresponding Arduino pin number, if not then return -1
* touch.getLongPress();                             // return long press the corresponding Arduino pin number, if not return -1
*********************************************************************************************************/

#include <TimerOne.h>                       //include the TimerOne library
#include <Wire.h>                           //include the I2C library
#include <RTClib.h>                         //include Real Time Clock library
#include <EasyStarterKitTouchSensor.h>      //include the touch sensor library
#include <EasyStarterKit_LCD1602.h>         //include the LCD display library

LiquidCrystal_I2C lcd(0x20);                //set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							                              //if 0x20 does not work, please modify it to be 0x38
                                            
RTC_DS1307 clock;                           //setup a RTC-DS1307 object named clock
DateTime now;                               //setup am object variable called now of user defined type DateTime which is defined in the RTClib library

char daysOfTheWeek[7][12] = {"Sun",         //declare a two dimension char array to store the English written days of the week, Sunday is in [0][X]
                             "Mon",         //Monday is in [1][X]
                             "Tue",         //Tuesday is in [2][X]
                             "Wed",         //Wednesday is in [3][X]
                             "Thu",         //Thursday is in [4][X]
                             "Fri",         //Friday is in [5][X]
                             "Sat"};        //Saturday is in [6][X]

#define TOUCH_OUT1 10                       //the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13                       //the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11                       //the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12                       //the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);      //setup the touch object


volatile uint8_t flag_flash = 0;              //flashing display flag, 0 = off, 1 = on
                                              //it is good practice to use volatile on variables 
                                              //used inside interrupt service routines

bool SET_DATE_TIME = false;                   //a true/false flag for when the user wants to set the date and time

uint32_t lcdUpdateTimer = 0;                  //an unsigned long integer variable for setting up a poled timer

void setup(){                                 //setup() runs once when the program is first started
  lcd.init();                                 //initialize the LCD   
  lcd.backlight();                            //turn on the backlight
  
  clock.begin();                                        //initialize the Real Time Clock (RTC)
  if(!clock.isrunning()){                               //if the clock is not running (i.e. it was reset by removing the battery)
    clock.adjust(DateTime(F(__DATE__), F(__TIME__)));   //update the RTC to the date and time of when the program was compiled
  }
  //clock.adjust(DateTime(F(__DATE__), F(__TIME__)));     //uncomment this line to force the RTC to be set to compilation time
                                                          //every time the program runs
  
  Timer1.initialize(500000);                    //initialize the timer for 500ms
  Timer1.attachInterrupt(TimingISR);            //attach TimingISR() to the interrupt
  Timer1.stop();                                //stop the timer
  flag_flash = 1;                               //

  lcd.clear();                                  //clear the display
  lcd.autoscroll();                             //turn on autoscroll
  lcd.setCursor(0, 0);                          //set the cursor to column 0, row 0
  lcd.print("Press 1 to set date and time");    //tell the user how to adjust the date and time
  uint32_t startupTimer = millis();             //a variable to use with millis() as a timer for the below loop
  while(millis() - startupTimer < 8000){        //loop for 8 seconds
    for(int i = 0; i < 3; i++){                 //loop 3 times
      lcd.scrollDisplayLeft();                  //scroll left one character
    }
    delay(700);                                 //wait 700ms
  }
  lcd.clear();                                  //clear the LCD display
  lcd.noAutoscroll();                           //turn off autoscroll

  lcdUpdateTimer = millis();                    //set the lcdUpdateTimer = to millis() to start the poled timer
                                                //millis() keeps a counter of how many milliseconds 
                                                //it has been since the processor started
}

void loop(){                                    //loop() runs over and over
  getUserInput();                               //check the user input

  if(SET_DATE_TIME){                            //if the user pressed the 1 then set the date and time
    lcd.clear();                                //clear the LCD
    setRTC();                                   //call the setRTC() function
    SET_DATE_TIME = false;                      //clear the SET_DATE_TIME flag
  }

  if(millis() - lcdUpdateTimer >= 1000){        //if it has been 1 second update the clock
    lcdUpdateTimer = millis();                  //rather than using an interrupt like Timer1
    printTime();                                //this is a poled timer so we have to check
  }                                             //it as often as possible
}                                               //end of loop()

void getUserInput(){                            //a user defined function for getting user input while
                                                //in the main loop of the program
                                                
  int button;                                   //a local variable for storing the button input (-1 if no button pressed)
  button = touch.get();                         //set button to the pressed button
  if(button != -1){                             //if there is a touch (-1 is no touch)
    delay(10);                                  //wait 10ms then
    if(button == touch.get()){                  //check if there is still the same touch
      switch(button){                           //if there was still the same touch perform a switch/case to deal with the button pressed
        case TOUCH_OUT1:                        //TOUCH_OUT1 is the set date and time button
          SET_DATE_TIME = true;                 //so set the SET_DATE_TIME flag to true so we call setRTC() this time through loop()
          break;
        default:                                //we don't care about the other buttons in this section
          break;                                //and it is good practice to always have a default case
      }
    }
    delay(10);                                  //wait 10ms
    while(button == touch.get());               //Wait for the button to be released
  }
}                                               //end getUserInput()
  
void printTime(){                               //user defined function to print the time from the RTC to the LCD  
  now = clock.now();                            //get the time and date from the RTC and store it in the object variable now

  /*
   * Notice that below we use leading zeros to ensure the thing we print is always the same
   * number of characters.  This allows us to skip using the lcd.clear() functions which 
   * cause the screen to flicker.  Because we always print the same number of characters we
   * ensure we always overwrite what was there previously, preventing extra characters
   * from being left on the display.  If we use lcd.clear() this function will cause a flicker
   * because it is being called every second to update the display.
   */

   //lcd.clear();           //uncomment this line to observe the LCD flicker from the lcd.clear() command
   
  //print the time on row 0
  lcd.setCursor(0, 0);                          //set the cursor to column 0, row 0
  if(now.hour() < 10) lcd.print("0");           //add a leading zero if needed
  lcd.print(now.hour());                        //print the hour
  lcd.print(":");                               //print a colon
  if(now.minute() < 10) lcd.print("0");         //add a leading zero if needed
  lcd.print(now.minute());                      //print the minute
  lcd.print(":");                               //print a colon
  if(now.second() < 10) lcd.print("0");         //add a leading zero if needed
  lcd.print(now.second());                      //print the second

  //print the date on row 1
  lcd.setCursor(0, 1);                          //set the cursor to column 0, row 1
  lcd.print(now.year());                        //we don't need to worry about a leading zero for a few thousand year so skip it
  lcd.print("/");                               //print a forward slash
  if(now.month() < 10) lcd.print("0");          //print a leading zero if needed
  lcd.print(now.month());                       //print the month
  lcd.print("/");                               //print a forward slash
  if(now.day() < 10) lcd.print("0");            //print a leading zero if needed
  lcd.print(now.day());                         //print the day
  lcd.print(" ");                               //print a space
  lcd.print(daysOfTheWeek[now.dayOfTheWeek()]); //print the three letter abbreviation of the day of the week
                                                //the RTClib library calculates the day of the week based from
                                                //January 1, 2000 and returns a number from 0 to 6. 0 is Sunday
                                                //6 is Saturday. The daysOfTheWeek[] array we setup earlier
                                                //contains the three letter abbreviations.
}                                               //end of printTime()

void setRTC(){                                      //user defined function to set the date and time on the RTC
  Timer1.start();                                   //start timer1 so the value being adjusted flashes

  int button;                                       //a variable to store the button press
  bool getInput = false;                            //a true/false flag to control the while loop
  uint16_t setDateTime[6] = {0};                    //an array to store the date and time as the user enters it
  
  char valueBeingSet[6][17] = { "Year:",            //an array of the text to be displayed in row 0
                                "Month:",           //as the user enters each value. We have 6 values
                                "Day:",             //to set and the display has 16 characters so we use
                                "Hour - (0-24):",   //17 to leave room for the null terminator. We actualy
                                "Minute:",          //only need it to be 15 to store the longest string
                                "Second:"};         //we have, the Hour string(14) plus the null terminator(1).
  
  
  now = clock.now();                                //get the current set time on the RTC
  setDateTime[0] = now.year();                      //fill setDateTime[] with the values read
  setDateTime[1] = now.month();                     //from the RTC as starting points for
  setDateTime[2] = now.day();                       //when the user modifies the values
  setDateTime[3] = now.hour();                      //
  setDateTime[4] = now.minute();                    //
  setDateTime[5] = now.second();                    //

  for(int i = 0; i < 6; i++){                       //a for loop to cycle through each of the values to be set
    lcd.clear();                                    //clear the LCD
    lcd.setCursor(0, 0);                            //set the cursor to column 0, row 0
    lcd.print(valueBeingSet[i]);                    //print the text for the value being set
    lcd.setCursor(0, 1);                            //set the cursor to column 0, row 1
    getInput = true;                                //set the getInput flag to true
    while(getInput){                                //loop while we are getting the input for this value
      button = touch.get();                         //store the initial button press
      if(button != -1){                             //if there is a touch (it wasn't -1)
        delay(10);                                  //wait 10 ms
        if(button == touch.get()){                  //then check if there is still the same touch
          switch(button){                           //a switch/case to act on different button presses
            case TOUCH_OUT2:                        //touch TCH2 area corresponds to increasing the value
              setDateTime[i]++;                     //
              break;                                //
            case TOUCH_OUT3:                        //touch TCH3 area corresponds to decreasing the value
              setDateTime[i]--;                     //
              break;                                //
            case TOUCH_OUT4:                        //touch TCH4 area corresponds to moving to the next value
              getInput = false;                     //
            default:                                //it is good practice to always have a default case
              break;                                //even if it doesn't do anything
          }
        }
        delay(10);                                  //wait 10 ms
        while(button == touch.get());               //Wait for the button to be released
      }
      
      //flash the value being modified
      lcd.setCursor(0, 1);
      if(flag_flash) lcd.print(setDateTime[i]);
      else lcd.clearRow(1);
    }
  }
  clock.adjust(DateTime(setDateTime[0],             //set the RTC to the values entered
                        setDateTime[1],
                        setDateTime[2],
                        setDateTime[3],
                        setDateTime[4],
                        setDateTime[5]));

  Timer1.stop();                                    //stop the timer so things don't flash once done setting the time
  flag_flash = 1;                                   //make sure it is left on for the next time we use it
  
  lcd.clear();                                      //clear the LCD
  lcd.setCursor(0, 0);                              //set the cursor to column 0, row 0
  lcd.print("Time and date");                       //print message that time and date have been set
  lcd.setCursor(0, 1);                              //set the cursor to column 0, row 1
  lcd.print("have been set!");                      //print the rest of the message
  delay(2000);                                      //give the user 2 seconds to read the message
  lcd.clear();                                      //clear the display
}

/**********Timer 1 interrupt routine*********/
void TimingISR(){
  flag_flash = !flag_flash;                         //alternate the flag between true/false (on/off)
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
