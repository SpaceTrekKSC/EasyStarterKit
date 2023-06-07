/*********************************************************************************************************
* Easy Starter Kit Project 04: Ultrasonic Air Gate
* 
* For this project you will build an air gate for drones that uses the ultrasonic distance sensor
* to detect when something is close.  Once the drone flys close to the untrasonic sensor the LED
* will change from red to blue and the LCD will display the time that the gate was triggered. Use 
* the push button to reset the air gate and turn the LED red again.
*
* Make sure you use L21 to manually set the time on all of the Air Gates used for your drone races.
* There are no networking features for the MakerBox so multiple devices can't sync their times.
* You must make sure they are all manually set correctly ahead of time.
*
* You will use what you learned in lesson L02, L09, L20 and L22.
*
* Use a 3 wire connector to connect the push button to the blue port that has D3 on its pin labels
* Use a 3 wire connector to connect the RGB LED to the blue port that has a D5 on its pin label
* Use a 4 wire connector to connect the ultrasonic sensor to the yellow port with D7 and D8 on its pin label
* Use a 4 wire connector to connect the RTC to one of the yellow I2C ports
* Use a 4 wire connector to connect the LCD to the other yellow I2C port
*
*
* The following functions are available:
*
* button.get();                                 //return HIGH (true) when the button is pressed and LOW (false) when not pressed
*
* rgb.begin();                                  //this initializes the RGB LED object
* rgb.setPixelColor(r, g, b, n=0);              //r, g, b should be 0~255, n is the position of the RGB led but can be omitted for a single LEG.
* rgb.show();                                   //after you have set the color of all the RGB LEDs, you should run this to display.
*
* ultrasonic.readDistance();                    //Get the measured distance, and return a float type value in centimeters
*
* lcd.init();                                     //I2C 1602 LCD screen initialization
* lcd.backlight();                                //turn on the backlight
* lcd.noBacklight();                              //turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);        //set the cursor position,col is column
* lcd.print(String str);                          //display string
* lcd.clear();
*
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
* now.day()                       //access the day of the month from the now object
*
****************************************************************************************************/

//All libraries have been included for you
#include <EasyStarterKitSwitch.h>               //include the Switch library to use the push button
#include <EasyStarterKitRGB.h>                  //include the RGB library
#include <EasyStarterKitUltrasonic.h>           //include the ultrasonic sensor library
#include <TimerOne.h>                           //include the TimerOne library
#include <Wire.h>                               //include the I2C library
#include <RTClib.h>                             //include the Real Time Clock library
#include <EasyStarterKit_LCD1602.h>             //include the LCD library

//All objects have been setup for you
#define BUTTON_PIN 3                           //SIG pin of push button connect to A2 of Arduino
Switch button(BUTTON_PIN);                      //setup the button as a switch object

#define RGB_PIN     5                           //DIN pin of the module connect to D3 of Arduino
#define NUMpixcels  1                           //number of RGB LED pixels 
RGB rgb(NUMpixcels, RGB_PIN);                   //setup the RGB object

#define TRIGGER_PIN  7                          //Trig pin of ultrasonic sensor module connects to D7 of Arduino
#define ECHO_PIN     8                          //Echo pin of ultrasonic sensor module connects to D8 of Arduino
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);   //setup object for ultrasonic sensor

LiquidCrystal_I2C lcd(0x20);                    //set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
                                                //if 0x20 does not work, please modify it to be 0x38

RTC_DS1307 clock;                               //setup a RTC-DS1307 object named clock
DateTime now;                                   //setup am object variable called now of user defined type DateTime which is defined in the RTClib library

#define ON 1                                    //define ON to be 1 (true)
#define OFF 0                                   //define OFF to be 0 (false)

volatile uint8_t Update = ON;                   //declare and initialize the variable Update.  Update is used in an interrupt service routine
                                                //so it is good practice to specify that the variable is volatile.

bool Triggered = false;                         //a variable to keep track of if the gate has been triggered or not


//fixed value settings - Adjust these to change how your air gate works
#define GATE_DISTANCE 30.0                      //set our gate distance to 30cm (values between 20.0 to 100 should work well)

//global variables
float distanceCM = 0.0;                         //a variable to store the measured distance in

//setup function runs once at the begining of the program
void setup() {
  clock.begin();                                //initialize the RTC object called clock

  clock.adjust(DateTime(2023, 4, 10, 13, 10, 30));  //use this line to set the RTC with an explicit date & time. (year, month, day, hour, minute, seconds)

  rgb.begin();                                //this initializes the RGB LED object
  rgb.setPixelColor(200, 0, 0);               //start the gate with the RGB LED red
  rgb.show();                                 //show the color on the RGB LED

  lcd.init();                                 //initialize the lcd 
  lcd.backlight();                            //turn on the backlight

  Timer1.initialize(1000000);                 //timing for 1 second
  Timer1.attachInterrupt(TimingISR);          //declare the interrupt serve routine:TimingISR 

}//end of setup()

//loop function runs over and over in a loop
void loop() {
  distanceCM = ultrasonic.readDistance();           //read the ultrasonic sensor (if it is out of range, it returns -1)
  if(distanceCM < 0){                               //if it is out of range...
    distanceCM = 999.0;                             //change it to a large value so it does not trip the gate
  }

  if(!Triggered){           //if the gate in NOT triggered display the current time and check the gate
    if(Update == ON){       //if Update is set to ON
      DataUpdate();         //then update the display
    }

    //use an if() statment to check if distanceCM is less
    //than GATE_DISTANCE.  If it is then call the triggerGate()
    //function defined below loop().

    
  }
  else{             //else, if it has been triggered, check for a button press to reset it

    //use an if() statment to check button.get(). If it is
    //pressed then call the resetGate() function.

    
  }
}//end of loop()


//************************************************************************************************************************************
//*********************************User Defined Functions to make the gate work*******************************************************
//************************************************************************************************************************************
void triggerGate(){
  now = clock.now();                        //get the time and date from the RTC and store it in the variable now
  lcd.clear();                              //clear the LCD 
  lcd.setCursor(0, 0);                      //set the cursor to the first place of the first line
  lcd.print("Gate Time:");                  //print "Gate Time:" on the first line of the LCD
  lcd.setCursor(0, 1);                      //the first place of the second line
  if(now.hour() < 10)lcd.print(" ");        //print the leading space if needed
  lcd.print(now.hour(), DEC);               //print the hour in 24 hour format
  lcd.print(":");                           //print the colon between hour and minute
  if(now.minute() < 10)lcd.print("0");      //print the leading zero if needed
  lcd.print(now.minute(), DEC);             //print the minute
  lcd.print(":");                           //print the colon between minute and second
  if(now.second() < 10)lcd.print("0");      //print the leading zero if needed
  lcd.print(now.second(), DEC);             //print the second

  rgb.setPixelColor(0, 0, 200);             //turn the gate blue
  rgb.show();                               //update the RGB LED
  Triggered = true;                         //don't update the LCD to show the current time
}
//************************************************************************************************************************************
//************************************************************************************************************************************
void resetGate(){
  rgb.setPixelColor(200, 0, 0);             //turn the gate red
  rgb.show();                               //update the RGB LED
  lcd.clear();                              //clear the LCD
  Triggered = false;                        //start showing current time again
}
//************************************************************************************************************************************
//************************************************************************************************************************************
void DataUpdate(){                          //a user defined function to update the LCD with the current time and date from the RTC
  now = clock.now();                        //get the time and date from the RTC and store it in the variable now
  lcd.setCursor(0, 0);                      //set the cursor to the first place of the first line
  lcd.print("Current Time:");               //print "Current Time:" on the first line of the LCD
  lcd.setCursor(0, 1);                      //the first place of the second line
  if(now.hour() < 10)lcd.print(" ");        //print the leading space if needed
  lcd.print(now.hour(), DEC);               //print the hour in 24 hour format
  lcd.print(":");                           //print the colon between hour and minute
  if(now.minute() < 10)lcd.print("0");      //print the leading zero if needed
  lcd.print(now.minute(), DEC);             //print the minute
  lcd.print(":");                           //print the colon between minute and second
  if(now.second() < 10)lcd.print("0");      //print the leading zero if needed
  lcd.print(now.second(), DEC);             //print the second
  Update = OFF;                             //turn Update off
}
//************************************************************************************************************************************
//************************************************************************************************************************************
void TimingISR(){         //the timer interrupt function
  Update = ON;            //whenever the timer goes off set Update to on so the current time updates every second
}
