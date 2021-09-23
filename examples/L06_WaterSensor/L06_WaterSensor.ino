/************************************************* *************************************************
* OPEN-SMART Easy Starter Kit Lesson 6:  Water Sensor
* You can learn how to detect whether there is water. If it finds the water, the buzzer will sound.
*
*
************************************************** ************************************************** *****/

#include <Wire.h>
#include <SoftwareSerial.h>
#include "EasyStarterKitBuzzer.h"

#define BUZZER_PIN 6         //SIG pin of Buzzer module connect to D6 of Arduino
Buzzer buzzer(BUZZER_PIN);    

#define WATER_SENSOR A0//SIG pin of Water Sensor module connect to A0 of IO Shield, that is pin A0 of Arduino

void setup()
{
	
}
void loop()
{
	if(isExposedToWater())
	{
		buzzer.on();
		delay(100);
	}
	else buzzer.off();

}


/************************************************************************/
/*Function: Determine whether the sensor is exposed to the water		*/
/*Parameter:-void           											*/
/*Return:	-boolean,if it is exposed to the water,it will return ture. */
boolean isExposedToWater()
{
	if(analogRead(WATER_SENSOR) < 180)
		return true;
	else return false;
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/