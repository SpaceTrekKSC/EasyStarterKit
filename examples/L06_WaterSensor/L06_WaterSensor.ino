/**************************************************************************************************
* Easy Starter Kit Lesson 6:  Water Sensor
* In this lesson you will learn how to use the water sensor.  The water sensor will detect anything
* that conducts electricity touching the silver lines on it.  You can test it by dipping it in water
* or you can touch it with your fingers and is will change resistance.  The below example uses a
* function to determine if the resistance is low enough to be considered exposed to water.
* 
* As a fun experiment, when you are done with this lesson see if you can modify it based on lesson 5
* such that it outputs the analogRead() value to the serial terminal so that you can see in real time
* what the voltage being read by the Arduino is.  Then experiment with different things contacting
* the sensor to see how they effect it.
* 
* Use a 3 wire connector to connect the active buzzer to the blue port that has a D6 on its pin
* labels.
* 
* Use a 3 wire connector to connect the water sensor to the blue port that has an A0 on its pin
* labels.
* 
* The following functions are available: 
*
* buzzer.on();              //turn on the buzzer
* buzzer.off();             //turn off the buzzer
* 
* isExposedToWater()        //use defined function to determine if the sensor is exposed to water
*
*
***************************************************************************************************/

#include <EasyStarterKitBuzzer.h>

#define BUZZER_PIN 6         //SIG pin of Buzzer module connect to D6 of Arduino
Buzzer buzzer(BUZZER_PIN);    

#define WATER_SENSOR A0//SIG pin of Water Sensor module connect to A0 of IO Shield, that is pin A0 of Arduino

//setup() runs once when the program is first started
void setup(){
  //this program does not do anything in the setup() function, but
  //Arduino requires the setup() function to be in the program
}

void loop(){                  //loop repeats over and over
	if(isExposedToWater()){     // if isExposedToWater returns true
		buzzer.on();              //turn the buzzer on
		delay(100);               //and wait for 100 ms so the user has a chance to hear the buzzer
	}
	else buzzer.off();          //otherwise turn the buzzer off
}


/************************************************************************/
/*Function: Determine whether the sensor is exposed to water            */
/*Parameter: -void - you do not pass any argument parameters            */
/*Return: -boolean, if it is exposed to the water,it will return true   */
boolean isExposedToWater(){
  if(analogRead(WATER_SENSOR) < 180)      //if the voltage on the analog pin assigned to WATER_SENSOR returns a value less than 180
    return true;                          //return true
  else return false;                      //otherwise return false
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
