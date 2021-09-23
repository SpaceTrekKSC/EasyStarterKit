/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 36: Speak Time and temperature
NOTE!!! First of all you should download the voice resources from our google drive:
https://drive.google.com/drive/folders/13cnW83AlHeHzdwrgioymGj7KpKZ1wekA?usp=sharing

Then unzip it and find the 01 and 02 folder and put them into your TF card (should not larger than 32GB). 

* This is integrated project. 
*  You can learn how to speak the time and temperature you get from the RTC module
   according to the value and the filename of digit /beep tone.
* Each time you touch the buzzer beep a time.
* The knob contorls the volume of the speak tone before you touch.

* The following functions are available for this lesson:

* timer1.initialize(unsigned long microseconds); // set the timed length, the unit is subtle
* timer1.attachInterrupt (TimingISR); // set interrupt routine function name, is the timing interrupt entry

* temper.getTemperature(); / / get the temperature value, the return of the float is the decimal

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
* lcd.print(double n, int digits);//display a double float number

* knob.getAngle();//read the angle of the knob, the range of angle is 0~280 degrees

* touch.get(); / / return is the touch area corresponding Arduino pin number, if not then return -1

mp3.getStatus();//get the status of the MP3 module, could be STOP, PLAY, PAUSE, FORWARD, REWIND
mp3.setVolume(int8_t vol);//vol is 0~0x1e, 30 adjustable level
mp3.playWithFileName(int8_t directory, int8_t file);//play a song according to the folder name and prefix of its file name
                                                            //foler name must be 01 02 03...09 10...99
                                                            //prefix of file name must be 001...009 010...255
* clock.begin(); // The clock is on and the function must be called first
* clock.getTime(void); / / read the clock and time, will be saved in the clock class variable
	That is, the seven variables save the date and time:
	uint8_t second;
	uint8_t minute;
	uint8_t hour; 
	uint8_t dayOfWeek;
	uint8_t dayOfMonth;
	uint8_t month;
	uint16_t year;															
************************************************** **************************************************/
#include <Wire.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>

#include "EasyStarterKitMP3.h"
#include "EasyStarterKitTouchSensor.h"
#include "EasyStarterKitDS1307.h"
#include "EasyStarterKitBuzzer.h"
#include "EasyStarterKitTemperature.h"
#include "EasyStarterKitKnob.h"
#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38

#define KNOB_PIN A0 //SIG pin of Rotary Angle Sensor module connect to A0 of IO Shield, that is pin A0 of Arduino
Knob knob(KNOB_PIN);

#define BUZZER_PIN 6      //SIG pin of Buzzer module connect to D6 of Arduino
Buzzer buzzer(BUZZER_PIN);

#define NTC_PIN A1     //SIG pin of NTC module connect to A1 of IO Shield, that is pin A1 of Arduino
Temperature temper(NTC_PIN);  

DS1307 clock;//DS1307 should connect to SDA and SCL on IO Shield

int8_t Time[] = {0,0,0,0};

unsigned char ClockPoint = 1;//Clock point flag, 1 = display clock point, 0 = not display

#define ON 1
#define OFF 0
unsigned char Update;//Update flag, 1 = re-acquire time and display, 0 = no action

#define TOUCH_OUT1 10//out1-TCH1 area corresponds to start speaker temperature
#define TOUCH_OUT2 13//out2-TCH2 area  corresponds to start speak time
#define TOUCH_OUT3 11//out3-TCH3 area not use now
#define TOUCH_OUT4 12//out4-TCH4 area not use now
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);

#define MP3_RX 4//RX of Serial MP3 module connect to D4 of Arduino
#define MP3_TX 2//TX to D2
MP3 mp3(MP3_RX, MP3_TX);
int8_t volume = 0x1e;//0~0x1e (30 adjustable level)
int8_t oldVolume;
int8_t newVolume;  //0~0x1e (30 adjustable level)

int8_t folderName = 2;//folder name must be 01 02 03 04 ...
int8_t fileName = 1; // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2//offset between number and file name, such as file name of 0 is 002, 1 is 003
#define TEN_OFFSET 20//only for whole ten, offset between whole ten digit and file name, such as file name of 20 is 22, 30 is 23, 40 is 24

void setup()
{
  delay(500);//Requires 500ms to wait for the MP3 module to initialize
  clock.begin();
  lcd.init();	 // initialize the lcd 
  lcd.backlight();//turn on the backlight

  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR 
  updateVolume();
}

void loop()
{
  
  int button;
  uint8_t flag_speak_time = 0;//0 = not speak, 1 = to speak
  uint8_t flag_speak_temp = 0;//0 = not speak, 1 = to speak
  button = touch.get();
  if(button != -1) //if touch the area
  {
    delay(10);//delay for 10ms
    buzzer.on();
    if(button == TOUCH_OUT1) //if touch the TCH1 area
    {
      flag_speak_temp = 1;
    }
    else if(button == TOUCH_OUT2) //if touch the TCH2 area
    {
      flag_speak_time = 1;
    }
	delay(50);
	buzzer.off();
	while(button == touch.get());//Wait for the button to be released
  }
  float celsius;
  if(Update == ON)//Update the display time, is to flash the clock porint
  {
    TimeUpdate();
	celsius = temper.getTemperature();//get temperature
    displayTemperature(celsius);//
    Update == OFF;
  }
  updateVolume();
  if(flag_speak_time)
  {
    SpeakTime(Time);
	flag_speak_time = 0;
  }
  
  if(flag_speak_temp)
  {
    
    SpeakTemp(celsius);
	flag_speak_temp = 0;
  }
    
}
/********************************/
//Update volume
void updateVolume()
{
  int angle;
  angle = knob.getAngle();
  newVolume = map(angle, 0, FULL_ANGLE, 0, 30); 
  if(newVolume != oldVolume) 
  {
  	mp3.setVolume(newVolume);
	oldVolume = newVolume;
	delay(50);
  }
}
/********************************/
//Speakt out time
void SpeakTime(int8_t time[])
{
  uint8_t addr[10] = {0};
  uint8_t next = 0;
  addr[next++] = 31;//031 before speak time
  addr[next++] = 32;//032 opensmart time
  if(Time[0] < 2)
  {
    addr[next++] = Time[0]*10 + Time[1]+NUM_OFFSET;
  }
  else
  {
    addr[next++] = 22;//022 twenty
    if(Time[1] != 0) addr[next++] = Time[1] + NUM_OFFSET;
  }
  if((Time[2] == 0) && (Time[3] == 0)) addr[next++] = 33;//033 e clock
  else{
    if(Time[2] < 2){
  	  addr[next++] = Time[2]*10 + Time[3]+NUM_OFFSET;
  	}
	else
	{
	  addr[next++] = Time[2] + TEN_OFFSET;
	  if(Time[3] != 0)addr[next++] = Time[3] + NUM_OFFSET;
	}
  }
  SpeakGroup(addr, next);
}

/********************************/
//Speak out temperature
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
  	  addr[next++] = t / 100 + NUM_OFFSET;//digit befor hundred
	  addr[next++] = 30;//030 hundred
	  t %= 100;
  	}
  else flag_hundred = 0;
  if(t != 0) 
  {
  	if(flag_hundred)addr[next++] = 38;//038 and
  	if(t < 20)
  	{
  	  addr[next++] = t + NUM_OFFSET;
  	}
    else
  	{
  	  addr[next++] = t / 10 + TEN_OFFSET;
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
  lcd.print("Temp: ");
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
  ClockPoint = !ClockPoint;
}

void TimeUpdate(void)
{
  clock.getTime();
  Time[0] = clock.hour / 10;
  Time[1] = clock.hour % 10;
  Time[2] = clock.minute / 10;
  Time[3] = clock.minute % 10;
  
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
  
  
  Update = OFF;
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/

