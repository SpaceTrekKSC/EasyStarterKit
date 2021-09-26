/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 35: Speak clock
NOTE!!! First of all you should download the voice resources from our google drive:
https://drive.google.com/drive/folders/13cnW83AlHeHzdwrgioymGj7KpKZ1wekA?usp=sharing

Then unzip it and find the 01 and 02 folder and put them into your TF card (should not larger than 32GB). 

* You can learn how to speak the time you get from the RTC module
   according to the value and the filename of digit /beep tone.
*
* The following functions are available:

* touch.get(); / / return is the touch area corresponding Arduino pin number, if not then return -1

* timer1.initialize(unsigned long microseconds); // set the timed length, the unit is subtle
* timer1.attachInterrupt (TimingISR); // set interrupt routine function name, is the timing interrupt entry

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
* lcd.print(double n, int digits);//display a double float number

/--------basic operations---------------/
mp3.play();
mp3.pause();
mp3.nextSong();
mp3.previousSong();
mp3.volumeUp();
mp3.volumeDown();
mp3.forward();    //fast forward
mp3.rewind();     //fast rewind
mp3.stopPlay();  
mp3.stopInject(); //when you inject a song, this operation can stop it and come back to the song befor you inject
mp3.singleCycle();//it can be set to cycle play the currently playing song 
mp3.allCycle();   //to cycle play all the songs in the TF card
/--------------------------------/
mp3.getStatus();//get the status of the MP3 module, could be STOP, PLAY, PAUSE, FORWARD, REWIND
mp3.playWithIndex(int8_t index);//play the song according to the physical index of song in the TF card

mp3.injectWithIndex(int8_t index);//inject a song according to the physical index of song in the TF card when it is playing song.

mp3.setVolume(int8_t vol);//vol is 0~0x1e, 30 adjustable level

mp3.playWithFileName(int8_t directory, int8_t file);//play a song according to the folder name and prefix of its file name
                                                            //foler name must be 01 02 03...09 10...99
                                                            //prefix of file name must be 001...009 010...099

mp3.playWithVolume(int8_t index, int8_t volume);//play the song according to the physical index of song in the TF card and the volume set

mp3.cyclePlay(int16_t index);//single cycle play a song according to the physical index of song in the TF

mp3.playCombine(int16_t folderAndIndex[], int8_t number);//play combination of the songs with its folder name and physical index
      //folderAndIndex: high 8bit is folder name(01 02 ...09 10 11...99) , low 8bit is index of the song
      //number is how many songs you want to play combination

************************************************** **************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>
#include <TimerOne.h>

#include "EasyStarterKitMP3.h"
#include "EasyStarterKitTouchSensor.h"
#include "EasyStarterKitDS1307.h"
#include "EasyStarterKit_LCD1602.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38
DS1307 clock;//define a object of DS1307 class

#define ON 1
#define OFF 0

int8_t Time[] = {0,0,0,0};
unsigned char ClockPoint = 1;
unsigned char Update;//Update flag, 1 = re-acquire time and display, 0 = no action
uint8_t halfsecond = 0;

#define TOUCH_OUT1 10//out1-TCH1 area not use now
#define TOUCH_OUT2 13//out2-TCH2 area  corresponds to start speak time
#define TOUCH_OUT3 11//out3-TCH3 area not use now
#define TOUCH_OUT4 12//out4-TCH4 area not use now
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);


#define MP3_RX 4//RX of Serial MP3 module connect to D4 of Arduino
#define MP3_TX 2//TX to D2
MP3 mp3(MP3_RX, MP3_TX);
int8_t volume = 0x1a;//0~0x1e (30 adjustable level)
int8_t folderName = 2;//folder name must be 01 02 03 04 ...
int8_t fileName = 1; // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2//lonly for 0~19, offset between number and file name, such as file name of 0 is 002, 1 is 003
#define TEN_OFFSET 20//only for whole ten, offset between whole ten digit and file name, such as file name of 20 is 22, 30 is 23, 40 is 24

void setup()
{
  lcd.init();	 // initialize the lcd 
  lcd.backlight();//turn on the backlight
  clock.begin();
  Timer1.initialize(500000);//timing for 500ms
  Timer1.attachInterrupt(TimingISR);//declare the interrupt serve routine:TimingISR 
  mp3.setVolume(volume);
}

void loop()
{
  
  int button;
  uint8_t flag_speak = 0;//0 = not speak, 1 = to speak
  button = touch.get();
  if(button == TOUCH_OUT2) //if touch the TCH1 area
  {
    delay(10);//delay for 10ms
    if(touch.get() == TOUCH_OUT2)//check it again
    {
      flag_speak = 1;
    }
	while(touch.get() == TOUCH_OUT2);//Wait for the button to be released
  }
  if(Update == ON)//Update the display time, is to flash the clock porint
  {
    TimeUpdate();
    Update == OFF;
  }
  if(flag_speak)
  {
    SpeakTime(Time);
	  flag_speak = 0;
  }
}

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


void SpeakGroup(uint8_t addr[], uint8_t size)//
{
  
  for(uint8_t i=0; i < size; i ++)
  {
    while(mp3.getStatus()!=STATUS_STOP)delay(50);
	mp3.playWithFileName(folderName,addr[i]);
  }
  while(mp3.getStatus()!=STATUS_STOP)delay(50);
}

/**********Timer 1 interrupt routine*********/
void TimingISR()
{
  halfsecond++;
  if(halfsecond == 2)
  {
    Update = ON;
	halfsecond = 0;
  }
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
