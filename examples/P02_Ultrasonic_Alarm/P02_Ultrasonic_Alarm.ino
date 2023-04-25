/*********************************************************************************************************
* Easy Starter Kit Project 02: Ultrasonic Distance Alarm
* 
* For this project you will build an alarm system that uses the ultrasonic distance sensor
* to detect when something is too close.  If placed by a window or door it could be used to
* detect if an intruder has entered and sound an alarm. When the alarm is triggered, by an 
* object getting close to the ultrasonic sensor, flash the RGB LED red and blue and play an 
* alternating tone from the passive buzzer.  Use the push button to deactivate the alarm.
*
* You will use what you learned in lesson L02, L08, L09, and L22.
*
* Use a 3 wire connector to connect the push button to the blue port that has A2 on its pin labels
* Use a 3 wire connector to connect the RGB LED to the blue port that has a D3 on its pin label
* Use a 3 wire connector to connect the passive buzzer to the blue port with a D6 on its pin label
* .............(The passive buzzer is the taller one without the sticker)...........................
* Use a 4 wire connector to connect the ultrasonic sensor to the yellow port with D7 and D8 on its pin label
*
*
* The following functions are available:
*
* button.get();                                 //return HIGH (true) when the button is pressed and LOW (false) when not pressed
*
* buz.playTone(note, millisecond);              //The first parameter note is the frequency of note, unit is HZ; 
*                                               //millisecond is the duration of this tone, unit is ms.
*
* buz.off();                                    //turns off the buzzer
*
* rgb.begin();                                  //this initializes the RGB LED object
* rgb.setPixelColor(r, g, b, n=0);              //r, g, b should be 0~255, n is the position of the RGB led but can be omitted for a single LEG.
* rgb.show();                                   //after you have set the color of all the RGB LEDs, you should run this to display.
*
* ultrasonic.readDistance();                    //Get the measured distance, and return a float type value in centimeters
*
****************************************************************************************************/

//All libraries have been included for you
#include <EasyStarterKitSwitch.h>               //include the Switch library to use the push button
#include <EasyStarterKitRGB.h>                  //include the RGB library
#include <EasyStarterKitPassiveBuzzer.h>        //include the passive buzzer library
#include <EasyStarterKitUltrasonic.h>           //include the ultrasonic sensor library

//All objects have been setup for you
#define BUTTON_PIN A2                           //SIG pin of push button connect to A2 of Arduino
Switch button(BUTTON_PIN);                      //setup the button as a switch object

#define RGB_PIN     3                           //DIN pin of the module connect to D3 of Arduino
#define NUMpixcels  1                           //number of RGB LED pixels 
RGB rgb(NUMpixcels, RGB_PIN);                   //setup the RGB object

#define PassiveBuzzerPin 6                      //the SIG pin of the buzzer is connected with D6 of Arduino
#define LOW_TONE  330                           //a low pitch tone (L3)
#define HIGH_TONE 1318                          //a high pitch tone (H3)
PassiveBuzzer buz(PassiveBuzzerPin);            //setup the passive buzzer object

#define TRIGGER_PIN  7                          //Trig pin of ultrasonic sensor module connects to D7 of Arduino
#define ECHO_PIN     8                          //Echo pin of ultrasonic sensor module connects to D8 of Arduino
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);   //setup object for ultrasonic sensor

//fixed value settings - Adjust these to change how your alarm works
#define ALARM_DISTANCE      40.0                //set our alarm distance to 40cm (values between 10.0 to 100 should work well)
#define ALARM_CYCLE_TIME    300                 //set the alarm cycle time to 300ms (0.3s)

//global variables
float distanceCM = 0.0;                         //a variable to store the measured distance in
bool alarmActive = false;                       //a variable to store if the alarm should be active or not
bool alarmCycle = false;                        //a variable to store what cycle the alarm is on (red/low or blue/high)

//setup function runs once at the begining of the program
void setup() {
  rgb.begin();                                  //this initializes the RGB LED object
}//end of setup()

//loop function runs over and over in a loop
void loop() {
  if(alarmActive){                                  //checks if the alarm is active
    alarmCycle = !alarmCycle;                       //cycles alarmCycle between true and false
    if(alarmCycle){                                 //set alarm LED blue and buzzer to high tone

      //add code to set the rgb led to blue and show it
      //add code to play the HIGH_TONE for ALARM_CYCLE_TIME on the buzzer
      //see lesson L08 and L09

    }
    else{                                           //set alarm LED red and buzzer to low tone

      //add code to set the rgb led to red and show it
      //add code to play the LOW_TONE for ALARM_CYCLE_TIME on the buzzer
      //see lesson L08 and L09

    }
  }
  else{                                             //if the alarm is not active turn things off

    //set the rgb color to (0,0,0) to turn it off
    //and turn off the buzzer

  }

  distanceCM = ultrasonic.readDistance();           //read the ultrasonic sensor (if it is out of range, it returns -1)
  if(distanceCM < 0) distanceCM = 999.0;            //change out of range to a large value so it does not trip the alarm
  if(distanceCM < ALARM_DISTANCE){                  //if the distance is less than ALARM_DISTANCE
    alarmActive = true;                             //turn the alarm on
  }

  if(button.get()){                                 //if the button is pressed
    alarmActive = false;                            //turn off the alarm
  }
}//end of loop()
