/************************************************* ************************************************** ******
* Easy Starter Kit Lesson 29: Touch control MP3
* NOTE!!! First of all you should download the voice resources from our google drive:
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* Then unzip it and find the 01 and 02 folder and put them on your micro SD card (card should not be larger than 32GB)
* Sometimes if you copy everything at once folder 02 will be written to the card first and it will cause the file
* index number to not be as expected.  Copy the 01 folder first and then copy the 02 folder to the SD card
* 
* Ensure the Arduino board is set to the 5 volt setting.  The switch is next to the USB port and should be set
* to the side that is closes to the USB port.  The bottom of the Arduino board has the labels.
*
* In this lesson you will use the touch sensor to provide user input to the mp3 player to add buttons for 
* play, pause, next song, and previous song.
* 
* Use a 3 wire connector to connect the active buzzer to the blue port with D6 in its pin label
* Use a 4 wire connector to connect the mp3 player to the yellow port with D4 and D2 in its pin label
* Use a 6 wire connector to connect the touch sensor the the red port with MOSI and MISO in its pin label
*
* The following functions are available:
*
* buzzer.on();                      //turn on the buzzer
* buzzer.off();                     //turn off the buzzer
*
* touch.get(); 						// return is the touch area corresponding Arduino pin number, if not then return -1
* touch.getLongPress(); 			// return long press the corresponding Arduino pin number, if not return -1
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

#include <EasyStarterKitTouchSensor.h>  //include touch sensor library
#include <EasyStarterKitMP3.h>          //include mp3 player library
#include <EasyStarterKitBuzzer.h>       //include buzzer library

#define BUZZER_PIN 6                    //the SIG pin of the Buzzer module is connected with D6 of OPEN-SMART UNO R3
Buzzer buzzer(BUZZER_PIN);              //setup the buzzer object

#define MP3_RX 4                        //mp3 RX pin
#define MP3_TX 2                        //mp3 TX pin
MP3 mp3(MP3_RX, MP3_TX);                //setup mp3 object

#define TOUCH_OUT1 10                   //the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13                   //the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11                   //the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12                   //the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);      //setup the touch object

int8_t volume = 0x17;                   //0~0x1e (30 adjustable level)
int8_t folderName = 0x01;               //folder name must be 01 02 03 04 ...
int8_t fileName = 0x01;                 //prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...

void setup(){                                 //setup() runs once 
  mp3.setVolume(volume);                      //set the volume
  delay(50);                                  //you should wait for >=50ms between two commands
  mp3.playWithFileName(folderName,fileName);  //play the song
  delay(50);
}

void loop(){
  int button;                       //local variable to store the button press
  button = touch.get();             //get the button press
  if(button != -1){                 //there is a touch
    delay(10);                      //wait 10 ms
  	if(button == touch.get()){      //Check again if there is the same touch
  	  buzzer.on();                  //every time you press the key, the buzzer will beep
  	  switch(button){               //switch/case on button
  	    case TOUCH_OUT1:            //touch TCH1 area is play
  	      mp3.play();
  	      break; 
    		case TOUCH_OUT2:            //touch TCH2 area is pause
    		  mp3.pause();
    		  break;                     
    		case TOUCH_OUT3:            //touch TCH3 area is next
    		  mp3.nextSong();
    		  break;                    
    		case TOUCH_OUT4:            //touch TCH4 area is previous
    		  mp3.previousSong();
    		  break;                    
    		default:
    		  break;
  	  }
  	}
	  while(button == touch.get());   //Wait for the button to be released
  }
  delay(50);                        //
  buzzer.off();                     //turn off the buzzer
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
