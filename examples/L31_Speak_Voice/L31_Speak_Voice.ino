/***********************************************************************************************************
* Easy Starter Kit Lesson 31: Speak Voice
* 
* In this lesson you will learn how the audio files are structured and get a better understanding for the
* indexing of the audio sources, this will be useful in your upcoming lessons.
* 
* Board Configuration:
*       Ensure the SD card is inserted in the back of the MP3 player
*       Connect the speaker to the white connector on the MP3 player
*       Connect the MP3 player to the yellow connector on the Arduino labeled D4 and D2
*       Connect the 4 Digit Display into one of the yellow I2C ports on the arduino
*       Connect the touch sensor to the red SPI connector on the Arduino
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
***********************************************************************************************************/
#include <EasyStarterKitMP3.h>            // include the MP3 library
#include <EasyStarterKitTouchSensor.h>    // include the touch sensor library
#include "EasyStarterKitTM1637.h"         // include the 4 Digit Display library

#define CLK A5                    //CLK of the TM1637 IC connect to A5 of Arduino
#define DIO A4                    //DIO of the TM1637 IC connect to A4 of Arduino
TM1637 disp(CLK,DIO);             // setup the 4 digit display object

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
int8_t maxNum = 40;               // we just supply 49 number of voice on our google drive
int8_t minNum = 1;                // minimum number should be 1 but not 0

void setup(){                     // start setup(), setup() runs once when the program starts
  mp3.setVolume(volume);          // set the MP3 players volume using the variable we declared above
  disp.init();                    // initialize the 4 digit display
  delay(50);                      // wait 50ms
}

void loop(){                      // loop() runs over and over 
  int button;                     // declare variable named button of type int
  uint8_t flag_speak = 0;         // 0 = do not speak, 1 = speak
  button = touch.get();           // store the value of touched area in "button"
  if(button != -1){               // if any area is touched
    delay(10);                    // delay for 10ms
    if(button == TOUCH_OUT1){     // if the touched area corresponds to button 1
      if(fileName < maxNum){      // if the filename index is less than our max
        flag_speak = 1;           // set our speak flag to 1 (true)
        fileName++;               // increase filename index
      }
    }
    else if(button == TOUCH_OUT2){  // if the touched area corresponds to button 2
      if(fileName > minNum){        // if the file index we are displaying is greater than min
        flag_speak = 1;             // set our speak flag to 1 (true)
        fileName--;                 // lower our filename idex
      }
    }
    else if(button == TOUCH_OUT3){  // if the touched area corresponds to button 3
      flag_speak = 1;               // play the audio corresponding to the index on display
    }
    else if(button == TOUCH_OUT4){  // if the touched area corresponds to button 4
    }                               // do nothing
	  while(button == touch.get());   // wait for the button to be released
  }
  if(button!=-1)disp.display(fileName);  // display the filename (index) on the display

  if(flag_speak){                                 // if our flag to speak is set
    mp3.playWithFileName(folderName,fileName);    // play the audio from that index on the folder declared
	  flag_speak = 0;                               // reset our flag back to 0 (false)
  }
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
