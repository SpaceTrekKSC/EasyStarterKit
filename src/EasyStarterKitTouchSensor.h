#ifndef _EasyStarterKit_TOUCHSENSOR_H__
#define _EasyStarterKit_TOUCHSENSOR_H__

#include <Arduino.h>


class TouchSensor{

private:

    uint8_t out[4];

public:

    TouchSensor(uint8_t out1, uint8_t out2, uint8_t out3, uint8_t out4)
    {
        out[0] = out1;
		out[1] = out2;
		out[2] = out3;
		out[3] = out4;
		for(uint8_t i=0; i < 4; i++){
          pinMode(out[i], INPUT);
          digitalWrite(out[i], LOW);
		}
        
    }
    
    int8_t get()                        
    {
      for(uint8_t i=0; i < 4; i++){
          if(digitalRead(out[i])){
		  	delay(10);
		  	if(digitalRead(out[i])) return out[i];
          }
		}
	  return -1;
    }
    int8_t getLongPress()
    {
      for(uint8_t i=0; i < 4; i++){
          if(digitalRead(out[i])){
		  	for(uint8_t j=0; j < 150; j++){
		  	delay(20);
		  	if(digitalRead(out[i]) == LOW) return - 1;
		  	}
			return out[i];
          }
		}
    }
};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/