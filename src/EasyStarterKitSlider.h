#ifndef __EasyStarterKit_SLIDER_H__
#define __EasyStarterKit_SLIDER_H__

#include <Arduino.h>

class Slider{

private:

    uint8_t _pin;

public:

    Slider(uint8_t pin)
    {
      _pin = pin;
      pinMode(_pin, INPUT);
    }

   inline int getPosition()
   {
	 int sensor_value = analogRead(_pin);
	 int position;
	 position = map(sensor_value, 0, 1023, 0, 100);
	
	 return position;
   }
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/