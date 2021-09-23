/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 34: Speak temperature
NOTE!!! First of all you should download the voice resources from our google drive:
https://drive.google.com/drive/folders/13cnW83AlHeHzdwrgioymGj7KpKZ1wekA?usp=sharing

Then unzip it and find the 01 and 02 folder and put them into your TF card (should not larger than 32GB). 

* You can learn how to speak the temperature you get from the temperature sensor
   according to the value and the filename of digit /beep tone.
   And only when you touch TCH1 area, it will speak temperature.
*
* The following functions are available:

* timer1.initialize(unsigned long microseconds); // set the timed length, the unit is subtle
* timer1.attachInterrupt (TimingISR); // set interrupt routine function name, is the timing interrupt entry

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
* lcd.print(double n, int digits);//display a double float number

* temper.getTemperature(); / / get the temperature value, the return value is a value that retains one decimal place.

* displayTemperature(float temperature); // display temperature

* touch.get(); / / return is the touch area corresponding Arduino pin number, if not then return -1

mp3.getStatus();//get the status of the MP3 module, could be STOP, PLAY, PAUSE, FORWARD, REWIND
mp3.setVolume(int8_t vol);//vol is 0~0x1e, 30 adjustable level
mp3.playWithFileName(int8_t directory, int8_t file);//play a song according to the folder name and prefix of its file name
                                                            //foler name must be 01 02 03...09 10...99
                                                            //prefix of file name must be 001...009 010...255

************************************************** **************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>
#include "EasyStarterKitMP3.h"
#include "EasyStarterKitTemperature.h"
#include "EasyStarterKitTouchSensor.h"
#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38
						
#define ON 1
#define OFF 0
unsigned char Update;//Update flag, 1 = re-acquire time and display, 0 = no action

#define TOUCH_OUT1 10//out1-TCH1 area corresponds to start speaker temperature
#define TOUCH_OUT2 13//out2-TCH2 area not use now
#define TOUCH_OUT3 11//out3-TCH3 area not use now
#define TOUCH_OUT4 12//out4-TCH4 area not use now
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);

#define NTC_PIN A1  //SIG pin of NTC module connect to A1 of IO Shield, that is pin A1 of Arduino
Temperature temper(NTC_PIN); // initialize an Temperature object "temper" for temperature

#define MP3_RX 4//RX of Serial MP3 module connect to D4 of Arduino
#define MP3_TX 2//TX to D2
MP3 mp3(MP3_RX, MP3_TX);
int8_t volume = 0x1e;//0~0x1e (30 adjustable level)
int8_t folderName = 2;//folder name must be 01 02 03 04 ...
int8_t fileName = 1; // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2//offset between number and file name, such as file name of 0 is 002, 1 is 003

void setup()
{
  delay(500);//Requires 500ms to wait for the MP3 module to initialize
  lcd.init();	 // initialize the lcd 
  lcd.backlight();//turn on the backlight
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR 
  mp3.setVolume(volume);
  delay(50);
}

void loop()
{
  
  int button;
  uint8_t flag_speak = 0;//Broadcast flag, 0 = not speak, 1 = to speak
  button = touch.get();
  if(button == TOUCH_OUT1) //if touch the TCH1 area
  {
    delay(10);//delay for 10ms
    if(touch.get() == TOUCH_OUT1)//check it again
    {
      flag_speak = 1;
    }
	while(touch.get() == TOUCH_OUT1);//Wait for the button to be released
  }
  float celsius;
  if(Update == ON)//Update the display 
  {
	Update =OFF;
    celsius = temper.getTemperature();//get temperature
    displayTemperature(celsius);//
  }
  
  if(flag_speak)
  {
    SpeakTemp(celsius);
	flag_speak = 0;
  }
  delay(100);
}
void SpeakTemp(float temp)
{
  if(temp >= 1000)return;
  else if(temp <= -1000)return;
  
  uint8_t addr[10] = {0};
  uint8_t next = 0;
  addr[next++] = 31;//031 before speak time
  addr[next++] = 35;//035 opensmart temperature
  if(temp < 0)
  {
    temp = abs(temp);
	addr[next++] = 40;//040 minus
  }
  int t = temp;//Get the integer part of the temperature
  uint8_t flag_hundred;
  if(t >= 100)
  	{
  	  flag_hundred = 1;
  	  addr[next++] = t / 100 + NUM_OFFSET;//digit before hundred
	  addr[next++] = 30;//030 hundred
	  t %= 100;
  	}
  else flag_hundred = 0;
  if(t != 0) 
  {
  	if(flag_hundred)addr[next++] = 38;//038 and
  	if(t < 20)
  	{
  	  addr[next++] = t + 2;
  	}
    else
  	{
  	  addr[next++] = t / 10 + 20;
	  t %= 10;
	  if(t != 0)addr[next++] = t + NUM_OFFSET;
  	}
  }
 
  addr[next++] = 1;//001 point
 
  uint8_t subbit;
  subbit = ((int)(temp*10))%10;// 
  addr[next++] = subbit + NUM_OFFSET; 
  addr[next++] = 36;//036 Degrees Celsius
  SpeakGroup(addr, next);
}

void SpeakGroup(uint8_t addr[], uint8_t size)//
{
  
  for(uint8_t i=0; i < size; i ++)
  {
    while(mp3.getStatus()!=STATUS_STOP)delay(50);
	mp3.playWithFileName(folderName,addr[i]);
  }
  while(mp3.getStatus()!=STATUS_STOP)delay(50);
}


/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

void displayTemperature(float temp)
{
  if((temp<-40)||(temp>125))return;
  lcd.setCursor(0, 1);
  if(temp < 0)
  {
    temp = abs(temp);
    lcd.print("-");
  }
  if((temp < 100)&&(temp>=10))lcd.print(" ");
  else if(temp < 10)lcd.print("  ");
  lcd.print(temp,1);
  lcd.print(" 'C");
}

/**********Timer 1 interrupt routine*********/
void TimingISR()
{
  Update = ON;
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/

