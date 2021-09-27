/*********************************************************************************************************
* Easy Starter Kit Lesson 11: Display number
* In this lesson you will learn how to use the 4 digit 7 segment display. 
* 
* Use a 4 wire connector to connect the display to the yellow port that has a D$ and D2 in its pin label
*
* The following functions are available:
*
* disp.init();                                        //initialize the display
* disp.display(int8_t DispData []);                   //display the numbers in the array
*                                                     //for example, if the array is {1,2,3,4}, then show 1234
*
* disp.display(uint8_t BitAddr, int8_t DispData);     //bit by bit to display numbers, characters, BitAddr range is 0 ~ 3,
*                                                     //parameter DispData range is 0 ~ 17 represents 0 ~ 9, A, b, c, d, E, F, -, space
* 
* disp.display(int Decimal);                          //display range: -999 ~ 9999
* disp.clearDisplay();                                //clear the display
* disp.set(uint8_t brightness);                       //set the brightness, 0 ~ 7, the greater the value, the higher the brightness, 
*                                                     //the next display takes effect
*
* disp.point(0);                                      //turn off the display's colon and the next display takes effect
* disp.point(1);                                      //turn on the display's colon and the next display takes effect
*
**********************************************************************************************************/

#include <EasyStarterKitTM1637.h>

#define DIO 2                         //DIO of the TM1637 IC connect to D2 of Arduino
#define CLK 4                         //CLK of the TM1637 IC connect to D4 of Arduino
TM1637 disp(CLK,DIO);                 //setup the TM1637 display object

void setup(){                         //setup() runs once when the program is first started
  disp.init();                        //initialize the display
}

void loop(){                          //loop() runs over and over
  int8_t ListDisp[4] = {3,4,5,6};     //declare and initialize an array of numbers
  disp.display(ListDisp);             //display the array
  delay(1000);                        //wait for 1 second
  
  disp.display(-100);                 //display a negative integer
  delay(1000);                        //wait 1 second

  disp.display(256);                  //display an integer
  delay(1000);                        //wait 1 second
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
