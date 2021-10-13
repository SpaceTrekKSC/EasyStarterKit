/*********************************************************************************************************
* Easy Starter Kit Lesson 25: Infrared remote control and display
* In this lesson you will learn how to receive the button pressed on the IR remote and
* display it to the 4 digit display.  The buzzer will also sound while you are holding
* a button on the IR remote.
* 
* Use a 3 wire connector to connect the active buzzer to the blue port with D6 in its pi label
* Use a 3 wire connector to connect the infrared receiver to the blue port with D3 in its pin label
* Use a 4 wire connector to connect the 4 digit display to the yellow port with D4 and D2 in its pin label
*
* The following functions are available:
*
* buzzer.on();                  //turn on the buzzer
* buzzer.off();                 //turn off the buzzer
*
* IR.enableIRIn();              //Start the receiver
* IR.decode();                  //if no result, it return 0, otherwise it return 1; .
* IR.resume();                  //so that it can receive the next value
* IR.isReleased();              //if the button is not released yet, it return 1; otherwise it return 0;
*
* disp.init ();                 //initialization
* disp.display (int Decimal);   //display range: -999 ~ 9999
****************************************************************************************************/

#include <EasyStarterKitTM1637.h>                 //include the display library
#include <EasyStarterKitIRremote.h>               //include the IR remote library
#include <EasyStarterKitBuzzer.h>                 //include the buzzer library

#define BUZZER_PIN 6                              //the SIG pin of the Buzzer module is connected with D6 of OPEN-SMART UNO R3
Buzzer buzzer(BUZZER_PIN);                        //setup the buzzer object

#define RECV_PIN 3                                //the SIG pin of Infrared Receiver is connected with D3 of OPEN-SMART UNO R3
IRrecv IR(RECV_PIN);                              //setup the IR object

#define CLK 4                                     //CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                                     //DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);                             //setup the display object

uint8_t num = 0;                                  //a global variable to store the number pressed in.  It can store 0 to 255

void setup(){                                     //setup() runs once when the program is first started
  disp.init();                                    //The initialization of the display
  IR.enableIRIn();                                //Start the receiver
}

void loop(){                                      //loop() runs over and over 
  if (IR.decode()){                               //If a valid key code is detected
    if(IR.isReleased()){                          //If it is not the repeat code for long press
  	  buzzer.on();                                //every time you press the key, the buzzer will beep
  	  switch(IR.keycode){                         //use a switch case to set or modify the variable num
        case KEY_ZERO:  num = 0;break;
        case KEY_ONE:   num = 1;break;
        case KEY_TWO:   num = 2;break;
        case KEY_THREE: num = 3;break;
        case KEY_FOUR:  num = 4;break;
        case KEY_FIVE:  num = 5;break;
        case KEY_SIX:   num = 6;break;
        case KEY_SEVEN: num = 7;break;
        case KEY_EIGHT: num = 8;break;
        case KEY_NINE:  num = 9;break;
        case KEY_PLUS:  num = num+1;break;
        case KEY_MINUS: num = num-1;break;
        default: break;
  	  }
  	  disp.display(num);                          //display the number
  	  delay(100);                                 //buzzer beep for 100ms
  	  buzzer.off();                               //turn off the buzzer
    }
    IR.resume();                                  //Receive the next value
  }
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
