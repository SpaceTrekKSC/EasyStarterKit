/************************************************* ************************************************** ******
* Easy Starter Kit Lesson 27: Play song with its file name
* NOTE!!! First of all you should download the voice resources from our google drive:
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* Then unzip it and find the 01 and 02 folder and put them on your micro SD card (card should not be larger than 32GB)
* Sometimes if you copy everything at once folder 02 will be written to the card first and it will cause the file
* index number to not be as expected.  Copy the 01 folder first and then copy the 02 folder to the SD card
* 
* Ensure the Arduino board is set to the 5 volt setting.  The switch is next to the USB port and should be set
* to the side that is closes to the USB port.  The bottom of the Arduino board has the labels.
* 
* This lesson is similar to lesson 26 except it uses the folder and file name instead of the file system index
* 
* Use a 4 wire connector to connect the MP3 player to the yellow port with D4 and D2 in its pin label
*
* The following functions are available:
*
* /--------basic operations---------------/
* mp3.play();
* mp3.pause();
* mp3.nextSong();
* mp3.previousSong();
* mp3.volumeUp();
* mp3.volumeDown();
* mp3.forward();                                              //fast forward
* mp3.rewind();                                               //fast rewind
* mp3.stopPlay();  
* mp3.stopInject();                                           //when you inject a song, this operation can stop it and come back to the song befor you inject
* mp3.singleCycle();                                          //it can be set to cycle play the currently playing song 
* mp3.allCycle();                                             //to cycle play all the songs in the SD card
* /--------------------------------/
* 
* mp3.playWithIndex(int8_t index);                            //play the song according to the physical index of song on the SD card
* 
* mp3.injectWithIndex(int8_t index);                          //inject a song according to the physical index of song on the SD card when it is playing song.
* 
* mp3.setVolume(int8_t vol);                                  //vol is 0~0x1e, 30 adjustable level
* 
* mp3.playWithFileName(int8_t directory, int8_t file);        //play a song according to the folder name and prefix of its file name
*                                                             //folder name must be 01 02 03...09 10...99
*                                                             //prefix of file name must be 001...009 010...099
* 
* mp3.playWithVolume(int8_t index, int8_t volume);            //play the song according to the physical index of song in the TF card and the volume set
* 
* mp3.cyclePlay(int16_t index);                               //single cycle play a song according to the physical index of song in the TF
* 
* mp3.playCombine(int16_t folderAndIndex[], int8_t number);   //play combination of the songs with its folder name and physical index
*                                                             //folderAndIndex: high 8bit is folder name(01 02 ...09 10 11...99) , low 8bit is index of the song
*                                                             //number is how many songs you want to play combination
* 
****************************************************************************************************/

#include <EasyStarterKitMP3.h>                  //include mp3 player library

#define MP3_RX 4                                //mp3 RX pin
#define MP3_TX 2                                //mp3 TX pin
MP3 mp3(MP3_RX, MP3_TX);                        //setup mp3 object

int8_t volume = 0x17;                           //0~0x1e (30 adjustable level)
int8_t folderName = 0x01;                       //folder name must be 01 02 03 04 ...
int8_t fileName = 0x01;                         //prefix of file name must be 001xxx 002xxx 003xxx ... 255xxx

void setup(){                                   //setup() runs once when the program is first started
  mp3.setVolume(volume);                        //set the volume to 0x17 (23 in decimal)
  delay(50);                                    //you should wait for >=50ms between two commands
  mp3.playWithFileName(folderName,fileName);    //play the file in folder 01 with file name that starts with 001
  delay(50);
}

void loop(){                                    //loop() is required by Arduino so it is included even though it is not used
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
