/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink

  Modified by: Andrew Gafford - 10 June 2021
  Added #include, #define, variables, and user function sections
  for teaching the structure of a program
*/

#include <Arduino.h>              //include the main Arduino library

#define BLINK_TIME  1000          //a constant value defined before compilation happens

int blinkTime = 1000;             //a global variable, could be changed during program execution

// the setup function runs once when you press reset or power on the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  blinkFunction();      //call user defined blinkFunction();
}

void blinkFunction(){
  digitalWrite(LED_BUILTIN, HIGH);        // turn the LED on (HIGH is the voltage level)
  delay(BLINK_TIME);                      // wait for a second
  digitalWrite(LED_BUILTIN, LOW);         // turn the LED off by making the voltage LOW
  delay(blinkTime);                       // wait for a second
}
