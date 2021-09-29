/********************************************************************************************************
* Easy Starter Kit Lesson 14: Temperature measurement and display
* 
* In this lesson you will learn how to read the temperature from an NTC thermistor and display it on the
* 4 digit display.  Since the display doesn't have a decimal you will only display the integer part of
* the temperature measurement.  The NTC thermistor is a silver tube with a black wire. The NTC adapter is
* a blue board with a white two pin connector and a blue three pin connector as well as the symbol of a 
* thermometer on it.  Connect the white connector of the thermistor to the white connector on the adapter
* board.
* 
* Connect the thermistor to the adapter board and use a 3 wire connector to connect the adapter board to
* the blue port with an A1 in its pin label
* 
* Use a 4 pin connector to connect the display to the yellow port with D4 and D2 in its pin label
*
* The following functions are available:
* 
* temper.getTemperature();                    // get the temperature value, the return of the float is the decimal
*
* displayTemperature(int8_t temperature);     // display temperature, at the end of the value of the word added C is the degree of Celsius
*
* disp.init();                                // initialize the display
* disp.display(int8_t DispData []);           // display the numbers in the array
*                                             // For example, the array is {1,2,3,4}, then show 1234
*********************************************************************************************************/

#include <EasyStarterKitTM1637.h>       //include the display library
#include <EasyStarterKitTemperature.h>  //include the temperature library

#define NTC_PIN A1                      //SIG pin of NTC module connect to A1 of IO Shield, that is pin A1 of Arduino
Temperature temper(NTC_PIN);            //setup temper object

#define CLK 4                           //CLK of the TM1637 IC connect to D4 of Arduino
#define DIO 2                           //DIO of the TM1637 IC connect to D2 of Arduino
TM1637 disp(CLK,DIO);                   //setup display object

void setup(){                           //setup() runs once when the program is first started
  disp.init();                          //The initialization of the display
  delay(1000);                          //wait for 1 second
}

void loop(){                            //loop() runs over and over
  float celsius;                        //declare a variable names Celsius of type float
  celsius = temper.getTemperature();    //getTemperature() returns a floating point value into the variable Celsius
  displayTemperature((int8_t)celsius);  //calls the user defined function displayTemperature() passing Celsius cast as an 8 bit integer
  delay(1000);                          //delay 1000ms
}


/************************************************************************************/
/* Function: Display temperature on 4-digit digital display                         */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees Celsius   */
/* Return Value: void                                                               */
/*                                                                                  */
/* If you recall from lesson 11 there are a few ways to use the disp.display()      */
/* command. Here we want to add the letter 'c' to the end of what is displayed so   */
/* we need to use the method that passes an array of numbers and make sure that the */
/* last element in the array has the number 12 in it to display the letter c.       */
/************************************************************************************/

void displayTemperature(int8_t temperature){            //a user defined function to display the temperature on the 4 digit display
  int8_t temp[4];                                       //declare an array to store our characters to display
  if(temperature < 0){                                  //if the temperature is negative
    temp[0] = INDEX_NEGATIVE_SIGN;                      //then set the first element in the array to the negative sign
    temperature = abs(temperature);                     //and change temperature to the absolute value to make it positive
  }
  else if(temperature < 100)temp[0] = INDEX_BLANK;      //else if temperature is less than 100 set the first element to blank
  else temp[0] = temperature/100;                       //else set the first element to the hundreds place of temperature
  temperature %= 100;                                   //the modulus of temperature and 100 will give just the tens and ones places
  temp[1] = temperature / 10;                           //set the second element in the array to the tens place of 100
  temp[2] = temperature % 10;                           //set the third element in the array to the ones place of temperature
  temp[3] = 12;	                                        //set the last element of the array to the index of 'C' for Celsius degree symbol
  disp.display(temp);                                   //display the array temp
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
