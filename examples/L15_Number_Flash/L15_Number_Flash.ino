/*********************************************************************************************************
* Easy Starter Kit Lesson 15: Number flashing
*
* In this lesson you will learn how to use a switch/case to control what happens when a button is pressed
* and you will use a timer to trigger an interrupt to flash the display.  We will use this again in lesson
* 19 to set the date and time for the real time clock (RTC) module.
*
* The following functions are available:

* Timer1.initialize(unsigned long microseconds);  //set the timed length, the unit is microseconds
* Timer1.attachInterrupt(TimingISR);              //attaches a function to the Timer1 interrupt
* Timer1.stop();                                  //stops Timer1
* Timer1.start();                                 //Starts Timer1

* touch.get();                                    //returns the corresponding Arduino pin number, if not touched then returns -1
* touch.getLongPress();                           //returns long press for the corresponding Arduino pin number, if not touched then returns -1
*
* disp.init();                                    //initialize the display
* disp.display(int Decimal);                      //display range: -999 ~ 9999
* disp.clearDisplay();                            //clear the display

*********************************************************************************************************/

#include <TimerOne.h>                     //include the TimerOne library
#include <EasyStarterKitTM1637.h>         //include the display library
#include <EasyStarterKitTouchSensor.h>    //include the touch sensor library

#define CLK 4                             //CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                             //DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);                     //setup the display object

#define TOUCH_OUT1 10                     //the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13                     //the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11                     //the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12                     //the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);    //setup the touch object

uint8_t flag_disp = 0;                    //display flag, 0 = no number, 1 = display
uint8_t flag_ok = 0;                      //OK flag, 0 = the number is not set, 1 = the number is set
uint8_t flag_next = 0;                    //next flag, 0 = stay at this number (step), 1 = move to next number (step)
uint8_t flag_change = 0;                  //change flag, 0 = number can not be modified, 1 = number can be modified

int num = 2021;                           //the initial value of the number

void setup(){                             //setup() runs once when the program is first started
  disp.init();                            //initialize the display
  Timer1.initialize(500000);              //initialize the timer to 500 milliseconds, allowing the number to flash once per second
  Timer1.attachInterrupt(TimingISR);      //attach an interrupt to the TimingISR function
  Timer1.stop();                          //stops the timer so that the number is not flashing at first
  flag_disp = 1;                          //set the display flag to 1 so the number is displayed 
}

void loop(){                              //loop() runs over and over
  int button;                             //declare a local variable named button of type int
  button = touch.get();                   //get the button pressed and store it in the variable button, touch.get() returns -1 when no button is pressed
  if(button != -1){                       //if there is a touch (button is anything other than -1)
    delay(10);                            //wait 10 ms
  	if(button == touch.get()){            //then check again if there is a touch
  	  switch(button){                     //use switch/case command to take appropriate action for what button was pressed
  	    case TOUCH_OUT1:                  //in the case that TOUCH_OUT1 was pressed
  	      if(flag_change)num++;           //if you're allowed to change the number then touch TOUCH_OUT1 area corresponds to increasing the number
  	      break;                          //break away from the switch/case
          
        case TOUCH_OUT2:                  //in the case that TOUCH_OUT2 was pressed
          if(flag_change)num--;           //if you're allowed to change the number then touch TOUCH_OUT2 area corresponds to decreasing the number
          break;                          //break away from the switch/case
          
        case TOUCH_OUT3:                  //in the case that TOUCH_OUT3 was pressed
          flag_ok = 1;                    //set OK flag to 1 so the number gets set at the end of loop()
          flag_change = 0;                //since the number has been set it can not be changed
          break;                          //break away from the switch/case
  
        case TOUCH_OUT4:                  //in the case that TOUCH_OUT4 was pressed
          flag_next = 1;                  //set the next flag to 1 to move to the "next" number to set
          flag_change = 1;                //set the change flag to 1 so you can adjust the number
          break;                          //break away from the switch/case
          
        default:                          //it is good practice to always include a default case for the switch/case
          break;                          //even if it is just to break away from the switch/case
  	  }
  	}
    while(button == touch.get());         //Wait for the button to be released
  }
  
  if(flag_ok){                            //if the OK flag is set to 1 then the OK button was pressed
    Timer1.stop();                        //so stop the timer to prevent the number from flashing
    flag_disp = 1;                        //set flag_disp to 1 to ensure the number is displayed
    flag_ok = 0;                          //set the OK flag back to 0 so the program doesn't keep coming here
  }
  
  if(flag_next){                          //if the next flag is set to 1
    Timer1.start();                       //start the timer again so the number flashes again
                                          //in this lesson all we are doing for our "next" number is using the same one over
                                          //in lesson 19 we will actually move on to the next number to be set
    flag_next = 0;                        //set the next flag back to 0 so the program doesn't keep coming here
  }
  
  if(flag_disp) disp.display(num);        //if flag_disp is set to 1 then display the number
  else disp.clearDisplay();               //else clear the display
}


void TimingISR(){                         //the is the function that gets called when the interrupt we attached in setup is triggered
                                          //the timer we setup is the thing triggering the interrupt
  flag_disp = !flag_disp;                 //0 = false and 1 = true so the ! (not) operator here has the effect of cycling the flag between 0 and 1
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
