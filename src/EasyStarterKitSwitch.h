#ifndef __EasyStarterKit_SWITCH_H__
#define __EasyStarterKit_SWITCH_H__

#include <Arduino.h>

class Switch{

private:
    uint8_t _pin;

public:
    Switch(uint8_t pin){
      _pin = pin;
      pinMode(_pin, INPUT);
	  digitalWrite(_pin, LOW);
    }
    
    inline uint8_t get()    {
      uint8_t level = digitalRead(_pin);
	  delay(10);
	  if(level == digitalRead(_pin)){
        return level;
	  }
	  else{
        return 0;
	  }
    }
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/