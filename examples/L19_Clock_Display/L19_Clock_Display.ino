/*********************************************************************************************************
* Easy Starter Kit Lesson 19: Clock display
*
* In this lesson you will learn how to display the time from the DS1307 Real Time CLock (RTC) on the 4 digit
* seven segment display. The real time clock has the capability of keeping accurate time, even when the Arduino
* board has been powered off.  It is a convenient way to store the real time and date and access it from an 
* Arduino device.  The RTC is on a blue board and has a silver button battery on it.  The display is on a blue 
* board and has a 4 digit seven segment display on it.
*
* Use a four wire connector to connect the display to the yellow port that has D4 and D2 in its pin labels
* Use a four wire connector to connect the RTC to one of the yellow I2C ports
*
* The following functions are available:
*
* timer1.initialize(unsigned long microseconds);      //set the timed length, the unit is subtle
* timer1.attachInterrupt (TimingISR);                 //set interrupt routine function name, is the timing interrupt entry
*
* disp.init();                                        //initialization
* disp.display(int8_t DispData []);                   //display the numbers in the array
*                                                     //For example, the array is {1,2,3,4}, then show 1234
*                                         
* disp.display(uint8_t BitAddr, int8_t DispData);     //bit by bit to display numbers, characters, BitAddr range is 0 ~ 3,
*                                                     //Parameter DispData range is 0 ~ 17 represents 0 ~ 9, A, b, c, d, E, F, -, space
* disp.display(int Decimal);                          //display range: -999 ~ 9999
* disp.clearDisplay();                                //clear the display
* disp.set(uint8_t brightness);                       //set the brightness the next time display takes effect  
*                                                     //0 ~ 7, the greater the value, the higher the brightness
*                                                     
* disp.point(0);                                      //Turn off the display colon the next time display takes effect
* disp.point(1);                                      //Turn on the display colon the next time display takes effect
*
* clock.begin();                                                      // The clock is on and the function must be called first
* clock.startClock(void);                                             // clock start, if you stop the clock need to call this function
* clock.stopClock(void);                                              // clock stop timing
* clock.getTime(void);                                                // read the clock and time, will be saved in the clock class variable
* clock.fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);   // write minutes
* clock.fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);      // write clock
* clock.fillDayOfWeek(uint8_t _dow);                                  // write week
* clock.setTime(void);                                                // write the clock and time that have been written to the clock chip
*
*********************************************************************************************************/

#include <TimerOne.h>                           //include timer one library
#include <Wire.h>                               //include I2C library
#include <EasyStarterKitTM1637.h>               //include 4 digit display library
#include <EasyStarterKitDS1307.h>               //include Real Time Clock library

DS1307 clock;                                   //setup DS1307 Real TIme Clock (RTC) object

#define CLK 4                                   //CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                                   //DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);                           //setup 4 digit display object

#define ON 1                                    //define ON as 1 (true)
#define OFF 0                                   //define OFF as 0 (false)

int8_t TimeDisp[] = {0x00,0x00,0x00,0x00};      //an array for displaying to the 4 digit display
unsigned char ClockPoint = 1;                   //a variable for to display the colon or not
unsigned char Update;                           //a variable for to update the display or not

void setup(){                                   //setup runs once when the program is first started
  disp.init();                                  //initialize the display
  clock.begin();                                //initialize the RTC
  Timer1.initialize(500000);                    //timing for 500ms
  Timer1.attachInterrupt(TimingISR);            //declare the interrupt serve routine:TimingISR  
}

void loop(){                                    //loop runs over and over 
  if(Update == ON){                             //if it is time to update the display
    TimeUpdate();                               //update the time in the display array TimeDisp
    disp.display(TimeDisp);                     //display the display array TimeDisp
  }
}

void TimingISR(){                               //the function attached to the timer interrupt
  Update = ON;                                  //turn Update on
  ClockPoint = !ClockPoint;                     //cycle the colon on the display
}

void TimeUpdate(void){                          //user defined function to update the display array TimeDisp
  if(ClockPoint)disp.point(POINT_ON);           //if ClockPoint is 1 show the colon
  else disp.point(POINT_OFF);                   //if ClockPoint is 0 turn off the colon
  clock.getTime();                              //update the time from the RTC
  TimeDisp[0] = clock.hour / 10;                //get the tens digit from the hour
  TimeDisp[1] = clock.hour % 10;                //get the ones digit from the hour
  TimeDisp[2] = clock.minute / 10;              //get the tens digit from the minute
  TimeDisp[3] = clock.minute % 10;              //get the ones digit from the minute
  Update = OFF;                                 //don't update again until the timer sets update to ON
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
