/************************************************* ************************************************** ******
* Easy Starter Kit Lesson 30: IR control MP3
* NOTE!!! First of all you should download the voice resources from our google drive:
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* Then unzip it and find the 01 and 02 folder and put them on your micro SD card (card should not be larger than 32GB)
* Sometimes if you copy everything at once folder 02 will be written to the card first and it will cause the file
* index number to not be as expected.  Copy the 01 folder first and then copy the 02 folder to the SD card
* 
* Ensure the Arduino board is set to the 5 volt setting.  The switch is next to the USB port and should be set
* to the side that is closes to the USB port.  The bottom of the Arduino board has the labels.
*
* This lesson is similar to lesson 29 except you will use the IR remote as the control.
* 
* Use a 3 wire connector to connect the active buzzer to the blue port with D6 in its pin label
* Use a 4 wire connector to connect the mp3 player to the yellow port with D4 and D2 in its pin label
* Use a 3 wire connector to connect the IR receiver to the blue port with D3 in its pin label
*
* The following functions are available:
*
* buzzer.on();                      //turn on the buzzer
* buzzer.off();                     //turn off the buzzer
*
* IR.enableIRIn();                  //Start the receiver
* IR.decode();                      //if no result, it return 0, otherwise it return 1; .
* IR.resume();                      //so that it can receive the next value
* IR.isReleased();                  //if the button is not released yet, it return 1; otherwise it return 0;
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
* mp3.stopInject();                                           //when you inject a song, this operation can stop it and come back to the song before you inject
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

#include <EasyStarterKitMP3.h>          //include the mp3 player library
#include <EasyStarterKitIRremote.h>     //include the IR remote library
#include <EasyStarterKitBuzzer.h>       //include the buzzer library

#define BUZZER_PIN 6                    //the SIG pin of the Buzzer module is connected with D6 of OPEN-SMART UNO R3
Buzzer buzzer(BUZZER_PIN);              //setup buzzer object

#define MP3_RX 4                        //mp3 RX pin
#define MP3_TX 2                        //mp3 TX pin
MP3 mp3(MP3_RX, MP3_TX);                //setup mp3 object

#define RECV_PIN 3                      //IR receiver pin
IRrecv IR(RECV_PIN);                    //setup IR object

int8_t flag_play = 1;                   //a flag for to play or not

void setup(){                           //setup() runs once when the program first starts
  IR.enableIRIn();                      //enable the IR receiver
}

void loop(){                            //loop() runs over and over again
  if (IR.decode()){                     //If decode is valid
    if(IR.isReleased()){                //wait for the button to be released
	    buzzer.on();                      //every time you press the key, the buzzer will beep
	    switch(IR.keycode){               //switch/case on IR.keycode
	      case KEY_PLUS:                  //the plus key is volume up
	        mp3.volumeUp(); 
	        break;
		    case KEY_MINUS:                 //the minus key is volume down
		      mp3.volumeDown(); 
		      break;
		    case KEY_PLAY:                  //the play key cycles the play flag so it can act as play and pause
		      flag_play = !flag_play;       //swap the play flag
			    if(flag_play)mp3.play();      //if play flag is 1 play the song
					else mp3.pause();             //else pause the song
					break;
		    case KEY_PREV:                  //the previous key plays the previous song
		      mp3.previousSong();
		      break;
		    case KEY_NEXT:                  //the next key plays the next song
		      mp3.nextSong();
		      break;
		    case KEY_ONE:                   //the one key plays the file with file system index 0x01
		      mp3.playWithIndex(0x01);
		      break;
		    case KEY_TWO:
		      mp3.playWithIndex(0x02);      //the two key plays the file with file system index 0x02
		      break;
		    default:
		      break;
	    }	 
	    delay(100);                       //buzzer beep for 100ms
	    buzzer.off();                     //turn off the buzzer
    }
    IR.resume();                        //Receive the next value
  }
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
