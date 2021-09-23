#ifndef __EasyStarterKit_BUZZER_H__
#define __EasyStarterKit_BUZZER_H__

#include <Arduino.h>


class Buzzer{

private:

    uint8_t _pin;

public:

    Buzzer(uint8_t pin)
    {
      _pin = pin;
      pinMode(_pin, OUTPUT);
      off();
    }
    
    inline void on()                        
    {
      digitalWrite(_pin, HIGH);
    }
    inline void off()                        
    {
      digitalWrite(_pin, LOW);
    }
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/