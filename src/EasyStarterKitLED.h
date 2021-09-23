#ifndef __EasyStarterKit_LED_H__
#define __EasyStarterKit_LED_H__

#include <Arduino.h>


class LED{

private:

    uint8_t _pin;

public:

    LED(uint8_t pin)
    {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      off();
    }
    inline void on(uint8_t brightness)
    {
      if((_pin == 3)||(_pin == 5)||(_pin == 6)||(_pin == 9)||(_pin == 10)||(_pin == 11))
	  	analogWrite(_pin, brightness);  //  analogWrite values from 0 to 255
    }
    inline void on()                        
    {
      digitalWrite(_pin, HIGH);
    }
    inline void off()                        
    {
      digitalWrite(_pin, LOW);
    }
	inline void breathe(uint8_t speed = 1)
	{
	  if(speed == 0) return;
	  uint8_t brightness = 0;
	  for(uint8_t i = 0; i < 50; i ++)
	  {
	    brightness = i;
	  	on(brightness);
		delay(30/speed);
	  }
	  for(uint8_t i = 50; i > 0; i --)
	  {
	    brightness = i;
	  	on(brightness);
		delay(30/speed);
	  }
	}
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/