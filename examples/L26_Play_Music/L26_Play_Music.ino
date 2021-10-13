/*********************************************************************************************************
* Easy Starter Kit Lesson 26: Play a song
* NOTE!!! First of all you should download the voice resources from our google drive:
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* Then unzip it and find the 01 and 02 folder and put them on your micro SD card (card should not be larger than 32GB)
* Sometimes if you copy everything at once folder 02 will be written to the card first and it will cause the file
* index number to not be as expected.  Copy the 01 folder first and then copy the 02 folder to the SD card
* 
* Ensure the Arduino board is set to the 5 volt setting.  The switch is next to the USB port and should be set
* to the side that is closes to the USB port.  The bottom of the Arduino board has the labels.
* 
* In this lesson you will play a music file from the SD card using the file index.  The MP3 player communicates with
* the Arduino board using a serial connection at 9600 baud. The library for it handles setting up the software defined
* serial connection. Once connected the Arduino can send commands to the MP3 player to make it do things like play or
* stop or next track.  The MP3 player has all the necessary hardware to decode the mp3 files and generate the amplified
* audio signal needed to drive the speaker or headphones.  The Arduino is simply the control interface for the MP3 player.
* 
* The MP3 player is on a red board. It has an SD card slot on one side and a headphone jack and 2 pin connector for a
* speaker on the other side. Connect the speak to the white 2 pin connector on the MP3 player.  Ensure the audio files
* are on the SD card and insert the SD card into the MP3 player.
* 
* Use a 4 wire connector to connect the MP3 player to the yellow port with D4 and D2 in its pin label
* 
*
* The following functions are available:
*
/--------basic operations---------------/
mp3.play();
mp3.pause();
mp3.nextSong();
mp3.previousSong();
mp3.volumeUp();
mp3.volumeDown();
mp3.forward();                                              //fast forward
mp3.rewind();                                               //fast rewind
mp3.stopPlay();  
mp3.stopInject();                                           //when you inject a song, this operation can stop it and come back to the song befor you inject
mp3.singleCycle();                                          //it can be set to cycle play the currently playing song 
mp3.allCycle();                                             //to cycle play all the songs in the SD card
/--------------------------------/

mp3.playWithIndex(int8_t index);                            //play the song according to the physical index of song on the SD card

mp3.injectWithIndex(int8_t index);                          //inject a song according to the physical index of song on the SD card when it is playing song.

mp3.setVolume(int8_t vol);                                  //vol is 0~0x1e, 30 adjustable level

mp3.playWithFileName(int8_t directory, int8_t file);        //play a song according to the folder name and prefix of its file name
                                                            //folder name must be 01 02 03...09 10...99
                                                            //prefix of file name must be 001...009 010...099

mp3.playWithVolume(int8_t index, int8_t volume);            //play the song according to the physical index of song in the TF card and the volume set

mp3.cyclePlay(int16_t index);                               //single cycle play a song according to the physical index of song in the TF

mp3.playCombine(int16_t folderAndIndex[], int8_t number);   //play combination of the songs with its folder name and physical index
                                                            //folderAndIndex: high 8bit is folder name(01 02 ...09 10 11...99) , low 8bit is index of the song
                                                            //number is how many songs you want to play combination

****************************************************************************************************/

#include <EasyStarterKitMP3.h>            //include the mp3 player library

#define MP3_RX 4                          //the MP3 RX pin
#define MP3_TX 2                          //the MP3 TX pin
MP3 mp3(MP3_RX, MP3_TX);                  //setup mp3 object

int8_t index  = 0x01;                     //the first song in the TF card
int8_t volume = 0x17;                     //0~0x1e (30 adjustable level)

void setup(){                             //setup() runs once when the program if first started
  mp3.playWithVolume(index,volume);       //play a song by index with volume
  delay(50);                              //you should wait for >=50ms between two commands
}

void loop(){                              //loop() is required by Arduino so it is included even though it is not used
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
