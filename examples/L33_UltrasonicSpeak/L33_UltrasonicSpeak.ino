/***********************************************************************************************************
* Easy Starter Kit Lesson 33: Play song with its file name
* 
* In this lesson you will learn how to use the ultrasonic distance measurement module to detect objects in 
* front of you. The sensor will detect anything from 3cm away upto 500cm away. Once the distance is measured
* you will use the MP3 player to read the distance out.
* 
* Board Configuration:
*       Ensure the SD card is inserted in the back of the MP3 player
*       Connect the speaker to the white connector on the MP3 player
*       Connect the MP3 player to the yellow connector on the Arduino labeled D4 and D2
*       Connect the LCD Screen to the other free yellow I2C port
*       Connect your ultrasonic sensor to the yellow connector labeled D7 and D8 
*       
* Before we get started add the sound recordings from the google drive to your SD card.
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* 
* The files must be on the SD card the exact way they are on the drive.
*     // Folder names must be 01 02
*     // Inside the folder each audio clip must be labeled 000 001 002 003 ...
*     
* The following functions are available for this lesson:
* 
* ultrasonic.readDistance();                            // returns the measured distance as a float in cm
* 
* lcd.init();                                           // I2C 1602 LCD screen initialization
* lcd.backlight();                                      // turn on the back light of the screen
* lcd.noBacklight();                                    // turn off the back light of the screen
* lcd.setCursor(uint8_t col, uint8_t row);              // set the cursor position
* lcd.print(Sring str);                                 // display string on the screen
* lcd.print(double n, int digits);                      // display a decimal number on the screen
* 
* mp3.getStatus();                                      // returns the status of MP3 player
*                                                       // options include: PlayerSTOP, PLAY, PAUSE, FORWARD, REWIND
* mp3.setVolume(int8_t vol);                            // vol is 0~0x1e, 30 adjustable level
* mp3.playWithFileName(int8_t directory, int8_t file);  // play a song according to the folder name and prefix of its file name
*                                                       // folder name must be 01 02 03...09 10...99
*                                                       // prefix of file name must be 001...009 010...255
***********************************************************************************************************/
#include "EasyStarterKitUltrasonic.h"     // include the ultrasonic sensor library
#include <EasyStarterKit_LCD1602.h>       // include the LCD screen library
#include <EasyStarterKitMP3.h>            // include the MP3 library

LiquidCrystal_I2C lcd(0x20);      // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
                                  // if 0x20 does not work, please modify it to be 0x38

#define TRIGGER_PIN  7            // Trig pin of ultrasonic sensor module connects to D7 of Arduino
#define ECHO_PIN     8            // Echo to D8
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN); // setup the ultrasonic object

#define MP3_RX 4                  // RX (receive) of Serial MP3 module connects to D4 of Arduino
#define MP3_TX 2                  // TX (transmit)connects to D2
MP3 mp3(MP3_RX, MP3_TX);          // Setup MP3 object
int8_t volume = 0x1e;             // 0~0x1e (30 adjustable level)
int8_t folderName = 2;            // folder name must be 01 02 03 04 ...
int8_t fileName = 1;              // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2              // offset between number and file name, here the file named 0 is 002 and 1 is 003
#define TEN_OFFSET 20             // only for whole numbers divisible by ten,  
                                  // here the file named 20 is 22, 30 is 23, 40 is 24
                                  
void setup() {              // start setup(), setup() runs once when the program starts
  delay(500);               // 500ms wait for the MP3 module to initialize
  lcd.init();               // initialize the lcd 
  lcd.backlight();          // turn on the back light of LCD
  mp3.setVolume(volume);    // Set the volume based on the above declared variable
  delay(50);                // wait for 50ms
}

void loop(){                            // loop() runs over and over
  float cmMsec;                         // declare variable named cmMsec of type float
  lcd.clear();                          // clear anything that may be on the LCD Screen
  cmMsec = ultrasonic.readDistance();   // get the distance measurement, if out of range, it returns -1;
  
  mp3.playWithFileName(folderName,43);  // play clip 043 "Ultrasonic distance detecting"
  
  lcd.setCursor(0, 0);                  // set cursor to 1st column, 1st row
  lcd.print(cmMsec,1);                  // print the measurement detected
  lcd.print(" cm");                     // print " cm" on the end
  speakDistance(cmMsec);                // speak the distance aloud
  delay(3000);                          // wait for 3 seconds
}

// This is the function responsible for compiling audio files to play using SpeakGroup()
void speakDistance(float distance){
	if(distance >= 1000)return;                           // we do not have audio to read these values, however
	else if(distance <= -1000)return;                     // we shouldnt reach them since they are outside the bounds
	
	uint8_t addr[10] = {0};                               // we use this array to construct the sentences
  uint8_t next = 0;                                     // counter to keep track of next index
	addr[next++] = 44;                                    // add clip 044, says "The nearest obstacle distance is"
	if(distance < 0){                                     // if the obstacle is out of range, return
	  return;
	}
	int t = distance;                                     // cast the distance as an int (truncate the decimals)
	uint8_t flag_hundred;                                 // flag that is true if the number is greater than 100
  if(t >= 100){                                         // if the number is greater than 100
  	  flag_hundred = 1;                                 // set our flag to true
  	  addr[next++] = t / 100 + NUM_OFFSET;              // digit before hundred
	    addr[next++] = 30;                                // add clip 030, says "hundred"
	    t %= 100;                                         // modulus by 100, this sets t only to the remainder
  }
  else flag_hundred = 0;                                // if our number isnt >100 we set the flag to 0
  if(t != 0){                                           // if our distance is greater than 0
  	if(flag_hundred)addr[next++] = 38;                  // add clip 038, says "and" only if our number was >100
  	if(t < 20){                                         // if the number is less than 20
  	  addr[next++] = t + NUM_OFFSET;                    // add the clip saying that number
  	}
    else{                                               // if the number is greater than 20
  	  addr[next++] = t / 10 + TEN_OFFSET;               // say the number in the tenths place
	    t %= 10;                                          // modulus to get the remainder 
	    if(t != 0)addr[next++] = t + NUM_OFFSET;          // say the remaining number
  	}
  }
  addr[next++] = 1;                                     // add clip 001, says "point"
	
	uint8_t subbit;                                       // declare a variable named subbit of type unsigned 8 bit int
	subbit = ((int)(distance*10))%10;                     // multiply float by 10 then modulus by 10 to get the last digit
	addr[next++] = subbit + NUM_OFFSET;                   // add this digit to the array, used to say decimal
	addr[next++] = 45;                                    // finally add clip 045 to say "Centimeters"

	SpeakGroup(addr, next);                               // send this array and the amount of items (stored in next) to the function to speak it
}

// This function plays out constructed sentences audio files.
void SpeakGroup(uint8_t addr[], uint8_t size){
  for(uint8_t i=0; i < size; i ++){                // for each element in the array
    while(mp3.getStatus()!=STATUS_STOP)delay(50);  // wait 50ms between each clip
    mp3.playWithFileName(folderName,addr[i]);      // play the audio file from the folder
  }
  while(mp3.getStatus()!=STATUS_STOP)delay(50);    // wait 50ms after it finishes the sentence
}
