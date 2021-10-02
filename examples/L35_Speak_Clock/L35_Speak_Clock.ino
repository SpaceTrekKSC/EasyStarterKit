/***********************************************************************************************************
* Easy Starter Kit Lesson 35: Speak Clock
* 
* In this lesson you will learn how to use the MP3 player to read the time aloud. You will
* use the RTC module to get the current time and date, the MP3 player to play the audio, 
* the touch sensor for controls, and the LCD screen to display data.
* 
* Board Configuration:
*       Ensure the SD card is inserted in the back of the MP3 player
*       Connect the speaker to the white connector on the MP3 player
*       Connect the MP3 player to the yellow connector on the Arduino labeled D4 and D2
*       Connect the RTC Clock to one of the yellow I2C ports
*       Connect the LCD Screen to the other free yellow I2C port
* 
* Before we get started add the sound recordings from the google drive to your SD card.
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* 
* The files must be on the SD card the exact way they are on the drive.
*     // Folder names must be 01 02
*     // Inside the folder each audio clip must be labeled 000 001 002 003 ...
*     
* The following functions are available:
* 
* touch.get();                                          // returns the pin number of the touched area, 
*                                                       // if nothing is touched it returns -1
* timer1.initialize(unsigned long microseconds);        // set a timer for given microseconds
* timer1.attachInterrupt (TimingISR);                   // sets a timing interrupt routine
* 
* lcd.init();                                           // I2C 1602 LCD screen initialization
* lcd.backlight();                                      // turn on the back light of the screen
* lcd.noBacklight();                                    // turn off the back light of the screen
* lcd.setCursor(uint8_t col, uint8_t row);              // set the cursor position
* lcd.print(Sring str);                                 // display string on the screen
* lcd.print(double n, int digits);                      // display a decimal number on the screen
* 
* /--------------- Basic Operations ---------------/
* mp3.play();                                           // this will play the audio, usually used as a resume
* mp3.pause();                                          // this will pause the current audio clip
* mp3.nextSong();                                       // this will play the next audio clip on file
* mp3.previousSong();                                   // this will play the previous audio clip on file
* mp3.volumeUp();                                       // increase volume
* mp3.volumeDown();                                     // decrease volume
* mp3.forward();                                        // fast forward
* mp3.rewind();                                         // fast rewind
* mp3.stopPlay();  
* mp3.stopInject();                                     
* mp3.singleCycle();                                    // repeat the same audio on loop
* mp3.allCycle();                                       // this will play all audio on file in order
* /-------------------------------------------------/
* mp3.getStatus();                                      // returns the status of MP3 player
*                                                       // options include: PlayerSTOP, PLAY, PAUSE, FORWARD, REWIND
* mp3.playWithIndex(int8_t index);                      // play the song according to the physical index of song in the SD card
* 
* mp3.injectWithIndex(int8_t index);                    // inject a song according to the physical index of song in the SD card when it is playing song.
* 
* mp3.setVolume(int8_t vol);                            // vol is 0~0x1e, 30 adjustable level
* mp3.playWithFileName(int8_t directory, int8_t file);  // play a song according to the folder name and prefix of its file name
*                                                       // folder name must be 01 02 03...09 10...99
*                                                       // prefix of file name must be 001...009 010...255
*                                                            
* mp3.playWithVolume(int8_t index, int8_t volume);      // play the song according to the physical index of song in the SD card and the volume set
* 
* mp3.cyclePlay(int16_t index);                         // play a song on repeat by its index on the SD card
* 
* mp3.playCombine(int16_t folderAndIndex[], int8_t number);   //play combination of the songs with its folder name and physical index
*                                                       // folderAndIndex: high 8bit is folder name(01 02 ...09 10 11...99) , low 8bit is index of the song
*                                                       // number is how many songs you want to play combination
*
***********************************************************************************************************/
#include <TimerOne.h>                     // include the library for the timer

#include <EasyStarterKitMP3.h>            // include the MP3 library
#include <EasyStarterKitTouchSensor.h>    // include the touch sensor library
#include <EasyStarterKitDS1307.h>         // include the clock library
#include <EasyStarterKit_LCD1602.h>       // include the LCD screen library

LiquidCrystal_I2C lcd(0x20);      // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
                                  // if 0x20 does not work, please modify it to be 0x38
DS1307 clock;                     // DS1307 should connect to SDA and SCL via I2C port

#define ON 1                      // Define variable ON to 1
#define OFF 0                     // Define variable OFF to 0

int8_t Time[] = {0,0,0,0};        // Global array to hold the Time data
unsigned char ClockPoint = 1;     // Clock point flag, 1 = display clock point, 0 = not display
unsigned char Update;             // Update flag, 1 = re-acquire time and display, 0 = no action
uint8_t halfsecond = 0;           // declare variable named halfsecond of type int and set it to 0

#define TOUCH_OUT1 10             // the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13             // the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11             // the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12             // the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);    // setup touch object


#define MP3_RX 4                  // RX (receive) of Serial MP3 module connects to D4 of Arduino
#define MP3_TX 2                  // TX (transmit)connects to D2
MP3 mp3(MP3_RX, MP3_TX);          // Setup MP3 object
int8_t volume = 0x1e;             // 0~0x1e (30 adjustable level)
int8_t folderName = 2;            // folder name must be 01 02 03 04 ...
int8_t fileName = 1;              // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2              // offset between number and file name, here the file named 0 is 002 and 1 is 003
#define TEN_OFFSET 20             // only for whole numbers divisible by ten,  
                                  // here the file named 20 is 22, 30 is 23, 40 is 24
                                  
void setup(){                         // start setup(), setup() runs once when the program starts
  lcd.init();                         // initialize the lcd 
  lcd.backlight();                    // turn on the back light of LCD
  clock.begin();                      // Start the clock
  Timer1.initialize(500000);          // timing for 500ms
  Timer1.attachInterrupt(TimingISR);  // declare the interrupt serve routine:TimingISR 
  mp3.setVolume(volume);              // set the volume based on our declared value
}

void loop() {                           // loop repeats over and over
  int button;                           // declare a variable named button of type int 
  uint8_t flag_speak = 0;               // 0 = not speak, 1 = to speak
  button = touch.get();                 // store the value of touched area in "button"
  if(button == TOUCH_OUT2){             // if touch area two is pressed
    delay(10);                          // delay for 10ms
    if(touch.get() == TOUCH_OUT2){      // check it again, to ensure a press
      flag_speak = 1;                   // set the flag to read clock
    }
	  while(touch.get() == TOUCH_OUT2);   // Wait for the button to be released
  }

  if(Update == ON){                     // update the display if interrupt timer says to
    TimeUpdate();                       // get the updated time
    Update == OFF;                      // set Update flag to off
  }
  if(flag_speak){                       // if speak time flag was changed
    SpeakTime(Time);                    // speak the current time
    flag_speak_time = 0;                // set the speak time flag to false
  }
}

// This function speaks the current time
void SpeakTime(int8_t time[]){
  uint8_t addr[10] = {0};                                   // we use this array to construct the sentences
  uint8_t next = 0;                                         // counter to keep track of the next index
  addr[next++] = 31;                                        // 031 plays a bell before speaking
  addr[next++] = 32;                                        // 032 says "the time is"
  if(Time[0] < 2){                                          // if the hour is less than 20
    addr[next++] = Time[0]*10 + Time[1]+NUM_OFFSET;         // add the sound clip to the array with NUM_OFFSET
  }
  else{                                                     // if it is larger than 19
    addr[next++] = 22;//022 twenty                          // add the clip to say "twenty"
    if(Time[1] != 0) addr[next++] = Time[1] + NUM_OFFSET;   // then add the clip for the second digit
  }
  if((Time[2] == 0) && (Time[3] == 0)) addr[next++] = 33;   // add clip 033 to say "O' clock" if there is no minutes
  else{                                                   
    if(Time[2] < 2){                                        // if there are minutes and its less than 20
      addr[next++] = Time[2]*10 + Time[3]+NUM_OFFSET;       // add corresponding clip to the array
    }
    else{
      addr[next++] = Time[2] + TEN_OFFSET;                  // otherwise use the Ten offset to speak the first digit
      if(Time[3] != 0)addr[next++] = Time[3] + NUM_OFFSET;  // then add the second digit normally
    }
  }
  SpeakGroup(addr, next);                                   // send the array we constructed to the function to read it
}

// This function plays out constructed sentences audio files.
void SpeakGroup(uint8_t addr[], uint8_t size){
  for(uint8_t i=0; i < size; i ++){                // for each element in the array
    while(mp3.getStatus()!=STATUS_STOP)delay(50);  // wait 50ms between each clip
    mp3.playWithFileName(folderName,addr[i]);      // play the audio file from the folder
  }
  while(mp3.getStatus()!=STATUS_STOP)delay(50);    // wait 50ms after it finishes the sentence
}

/********** Timer1 Interrupt Routine **********/
void TimingISR(){
  halfsecond++;                           // increase our half second counter
  if(halfsecond == 2){                    // if it has been a full second
    Update = ON;                          // set our update flag to ON
	  halfsecond = 0;                       // reset the half second counter
  }
}

void TimeUpdate(void){                    // this function updates the time on the lcd
  clock.getTime();                        // get the current time
  Time[0] = clock.hour / 10;              // store the hour's first digit
  Time[1] = clock.hour % 10;              // store the hour's second digit
  Time[2] = clock.minute / 10;            // store minute's first digit
  Time[3] = clock.minute % 10;            // store minute's second digit
  
  lcd.setCursor(0, 0);                    // start in first column, first row
  lcd.print("Time: ");                    // print "Time: "
  if(clock.hour < 10)lcd.print(" ");      // if there is no first digit in hour, print space
  lcd.print(clock.hour, DEC);             // print the hour
  lcd.print(":");                         // add colon between hour and minutes
  if(clock.minute < 10)lcd.print("0");    // if minutes has no first digit, print a 0
  lcd.print(clock.minute, DEC);           // print the minutes
  lcd.print(":");                         // print colon between minutes and seconds
  if(clock.second < 10)lcd.print("0");    // if there is no first digit, print a 0
  lcd.print(clock.second, DEC);           // print the seconds
  
  lcd.setCursor(0, 1);                    // move cursor to first column second row
  lcd.print("Date: ");                    // print "Date: "
  lcd.print(clock.year+2000, DEC);        // add the year to the print
  lcd.print(".");                         // add a period between year and month
  if(clock.month < 10)lcd.print("0");     // if the month is a single digit add a 0
  lcd.print(clock.month, DEC);            // add the month to the print
  lcd.print(".");                         // add a period between month and day
  if(clock.dayOfMonth < 10)lcd.print("0");// if the day of month is single digit print a 0
  lcd.print(clock.dayOfMonth, DEC);       // print the day
  Update = OFF;                           // reset our Update flag to OFF
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
