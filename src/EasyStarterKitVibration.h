#ifndef __EasyStarterKit_VIBRATION_H__
#define __EasyStarterKit_VIBRATION_H__

#include <Arduino.h>


class Vibration{

private:

    uint8_t _pin;

public:

    Vibration(uint8_t pin)
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