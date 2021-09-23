/************************************************* ************************************************** ******
* OPEN-SMART Easy Starter Kit Lesson 33: Play song with its file name
NOTE!!! First of all you should download the voice resources from our google drive:
https://drive.google.com/drive/folders/13cnW83AlHeHzdwrgioymGj7KpKZ1wekA?usp=sharing

Then unzip it and find the 01 and 02 folder and put them into your TF card (should not larger than 32GB). 

* You can learn to use  the ultrasonic distance measurement module to measure the distance 
* of the nearest obstacle in front, the maximum measurement is 500 cm, at least 3 cm,
* and then use MP3 module to speak out it.

* The following functions are available for this lesson:

* ultrasonic.readDistance();//Get the measured distance, and return a float type value in centimeters

* lcd.init();     // I2C 1602 LCD screen initialization
* lcd.backlight();//turn on the backlight
* lcd.noBacklight();//turn off the backlight
* lcd.setCursor(uint8_t col, uint8_t row);// set the cursor position,col is column
* lcd.print(Sring str);//display string
* lcd.print(double n, int digits);//display a double float number

mp3.getStatus();//get the status of the MP3 module, could be STOP, PLAY, PAUSE, FORWARD, REWIND
mp3.setVolume(int8_t vol);//vol is 0~0x1e, 30 adjustable level
mp3.playWithFileName(int8_t directory, int8_t file);//play a song according to the folder name and prefix of its file name
                                                            //foler name must be 01 02 03...09 10...99
                                                            //prefix of file name must be 001...009 010...255

************************************************** **************************************************/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "EasyStarterKitUltrasonic.h"
#include "EasyStarterKit_LCD1602.h"
#include "EasyStarterKitMP3.h"

LiquidCrystal_I2C lcd(0x20);  // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							// if 0x20 does not work, please modify it to be 0x38

//module--- Arduino
//VCC   --- 5V
//GND   --- GND
#define TRIGGER_PIN  7//Trig pin of ultrasonic sensor module connect to D7 of Arduino
#define ECHO_PIN     8//Echo to D8
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

#define MP3_RX 4//RX of Serial MP3 module connect to D4 of Arduino
#define MP3_TX 2//TX to D2
MP3 mp3(MP3_RX, MP3_TX);
int8_t volume = 0x1a;//0~0x1e (30 adjustable level)
int8_t folderName = 2;//folder name must be 01 02 03 04 ...
int8_t fileName = 1; // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...
#define NUM_OFFSET 2//offset between number and file name, such as file name of 0 is 002, 1 is 003
#define TEN_OFFSET 20//only for whole ten, offset between whole ten digit and file name, such as file name of 20 is 22, 30 is 23, 40 is 24
void setup()
{
  delay(500);//Requires 500ms to wait for the MP3 module to initialize
  lcd.init();	 // initialize the lcd 
  lcd.backlight();//turn on the backlight
  mp3.setVolume(volume);//Set the volume
  delay(50);
}

void loop()
{
  float cmMsec;  
  lcd.clear();
  cmMsec = ultrasonic.readDistance();//if out of range, it return -1;
  
  mp3.playWithFileName(folderName,43); //043 Ultrasonic distance detecting
  
  lcd.setCursor(0, 0);
  lcd.print(cmMsec,1);
  lcd.print(" cm");
  speakDistance(cmMsec);
  delay(3000);//wait for 3 seconds
}

void speakDistance(float distance)
{
	if(distance >= 1000)return;
	else if(distance <= -1000)return;
	
	uint8_t addr[10] = {0};
	uint8_t next = 0;
	addr[next++] = 44;//044 The nearest obstacle distance is
	if(distance < 0)
	{
	  return;
	}
	int t = distance;//Get the integer part
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
	subbit = ((int)(distance*10))%10;//One after the decimal point
	addr[next++] = subbit + NUM_OFFSET; 
	addr[next++] = 45; //045 Centimeters

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
