/***********************************************************************************************************
* Easy Starter Kit Lesson 32: Speak Pi
* 
* In this lesson you will learn how to use the MP3 player to read any number given and use the files
* on your SD card to read that number aloud. By default this program will be setup to read the value
* of Pi (3.14159265).
* 
* Board Configuration:
*       Ensure the SD card is inserted in the back of the MP3 player
*       Connect the speaker to the white connector on the MP3 player
*       Connect the MP3 player to the yellow connector on the Arduino labeled D4 and D2
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
*
***********************************************************************************************************/
#include <EasyStarterKitMP3.h>    // include the MP3 library

#define MP3_RX 4                  // RX (receive) of Serial MP3 module connects to D4 of Arduino
#define MP3_TX 2                  // TX (transmit)connects to D2
MP3 mp3(MP3_RX, MP3_TX);          // Setup MP3 object

int8_t volume = 0x1e;             // 0~0x1e (30 adjustable level)
int8_t folderName = 2;            // folder name must be 01 02 03 04 ...
int8_t fileName = 1;              // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2              // offset between number and file name, here the file named 0 is 002 and 1 is 003

const char pi[] = "3.14159265";   // declare a char array named Pi and set its contents to the value of pi

//setup() runs once when the program is first started
void setup(){
  //this program does not do anything in the setup() function, but
  //Arduino requires the setup() function to be in the program
}

void loop() {                           // loop() runs over and over
  uint8_t num = sizeof(pi);             // declare variable num of type unsigned 8 bit int
  mp3.playWithFileName(folderName,39);  // play clip 039 which says "pi is..."
  for(uint8_t i = 0; i < num; i++){     // for every character in our array "pi"
    speaknum(pi[i]);                    // send the character to speaknum() to be read aloud
  }
  while(1);                             // this while statement stops the loop from repeating
}

void speaknum(char c) {
  if ('0' <= c && c <= '9') {           // if the input char is between 0 - 9
    fileName = c - 0x30 + NUM_OFFSET;   // set fileName to the clip of audio that reads that number
  } 
  else if(c == '.'){                    // if the char is a period 
    fileName = 1;                       // set fileName to clip 001, which says "point"
  }
  else if (c != '.') {                  // if the char is not a number or a period
    return;                             // return, this is an error
  }
  while(mp3.getStatus()!=STATUS_STOP)delay(50); // wait until the MP3 player is stopped, check every 50ms until it is stopped
  mp3.playWithFileName(folderName,fileName);    // play the audio clip selected from above
  delay(100);                                   // wait 100ms
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
