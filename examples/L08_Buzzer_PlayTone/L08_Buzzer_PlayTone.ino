/************************************************* *************************************************
* Easy Starter Kit Lesson 8:  Passive buzzer to play tone of Chinese Double Tiger song
* In this lesson you will learn how to use the passive buzzer.  Unlike the active buzzer used in the
* previous examples, the passive buzzer is able to produce different tones depending on its input.
* The active buzzer generates its own oscillating signal when it is activated.  The passive buzzer
* needs to have an oscillating signal sent to its input, it is more like a speaker than the active
* buzzer.  The passive buzzer is a blue board with a black circular component.  It is the one that 
* did not come with the sticker on it.
* 
* Use a 3 wire connector to connect the passive buzzer to the blue port with a D6 on its pin label
*
* The following functions are available:
*
* buz.playTone(long note, long millisecond);    //The first parameter note is the frequency of note, unit is HZ; 
*                                               //millisecond is the duration of this tone, unit in ms.
*
* buz.off();                                    //turns off the buzzer*                                               
*
************************************************** *******************************************************/
	 
#include <EasyStarterKitPassiveBuzzer.h>      //include the passive buzzer library

#define PassiveBuzzerPin 6                    //the SIG pin of the module is connected with D6 of Arduino / OPEN-SMART UNO
                                              //There is no white waterproof sticker on the passive buzzer
PassiveBuzzer buz(PassiveBuzzerPin);          //setup the passive buzzer object

//------------------------------------
//This is a macro definition of bass, mid-range, and treble frequencies
#define NOTE_L1  262
#define NOTE_L2  294
#define NOTE_L3  330
#define NOTE_L4  349
#define NOTE_L5  392
#define NOTE_L6  440
#define NOTE_L7  494
#define NOTE_M1  523
#define NOTE_M2  587
#define NOTE_M3  659
#define NOTE_M4  698
#define NOTE_M5  784
#define NOTE_M6  880
#define NOTE_M7  988
#define NOTE_H1  1046
#define NOTE_H2  1175
#define NOTE_H3  1318
#define NOTE_H4  1397
#define NOTE_H5  1568
#define NOTE_H6  1760
#define NOTE_H7  1976
//-------------------------------------------
//musical notation of the song Two Tigers 
const static int doubleTiger[] = {          //create a constant static array containing the notes to play
NOTE_M1,NOTE_M2,NOTE_M3,NOTE_M1, 
NOTE_M1,NOTE_M2,NOTE_M3,NOTE_M1, 
NOTE_M3,NOTE_M4,NOTE_M5, 
NOTE_M3,NOTE_M4,NOTE_M5, 
NOTE_M5,NOTE_M6,NOTE_M5,NOTE_M4,
NOTE_M3,NOTE_M1, 
NOTE_M5,NOTE_M6,NOTE_M5,NOTE_M4,
NOTE_M3,NOTE_M1, 
NOTE_M2,NOTE_L5,
NOTE_M1,0,
NOTE_M2,NOTE_L5,
NOTE_M1,0
};
//-------------------------------------------


////note duration of Two Tigers, 2 is half a beat, 1 is a beat.
const static int tempo[] = {              //create a constant static array of the beats needed for each note
2,2,2,2, 
2,2,2,2, 
2,2,1, 
2,2,1,  
2,2,2,2,
1,1,
2,2,2,2,
1,1,
1,1,
1,1,
1,1,
1,1
};

void setup(void){   //setup() runs once.  By calling sing() here we make it so it will only play once
  sing();           //call the user defined function sing() to play the song
}

//loop repeats over and over.  If you put the sing() command in loop the song will play forever
void loop(){
  //this program does not do anything in the loop() function, but
  //Arduino requires the loop() function to be in the program
}

void sing(){      // iterate over the notes of the melody:
  int size = sizeof(doubleTiger) / sizeof(int);           //declare a variable named size of type int and initialize it to 
                                                          // the size of the doubleTiger array divided by the size of an int
                                                          //so that we have the number of values in the array
                                                          
  for (int thisNote = 0; thisNote < size; thisNote++){    //create a for loop to iterate through each of the elements in the array
    int noteDuration = 500 / tempo[thisNote];             //create a variable for the beat of the tone to be played using a beat of 0.5 seconds

    buz.playTone(doubleTiger[thisNote], noteDuration);    //play the current tone for the current beat time

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 0.30;
    delay(pauseBetweenNotes);
  }
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
