/***********************************************************************************************************
* Easy Starter Kit Lesson 36: Speak Time and Temperature
* 
* In this lesson you will learn how to use the MP3 player to read the time and temperature aloud. You will
* use the RTC module to get the current time and date, the temperature sensor to record temperature, the 
* buzzer for added responsiveness, the touch sensor for controls, the rotary knob to adjust volume, and the
* LCD screen to display data.
* 
* Before we get started add the sound recordings from the google drive to your SD card.
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* 
* The files must be on the SD card the exact way they are on the drive.
*     // Folder names must be 01 02
*     // Inside the folder each audio clip must be labeled 000 001 002 003 ...
*     
* timer1.initialize(unsigned long microseconds);        // set a timer for given microseconds
* timer1.attachInterrupt (TimingISR);                   // sets a timing interrupt routine
* 
* temper.getTemperature();                              // returns the temperature value as a float
* 
* lcd.init();                                           // I2C 1602 LCD screen initialization
* lcd.backlight();                                      // turn on the back light of the screen
* lcd.noBacklight();                                    // turn off the back light of the screen
* lcd.setCursor(uint8_t col, uint8_t row);              // set the cursor position
* lcd.print(Sring str);                                 // display string on the screen
* lcd.print(double n, int digits);                      // display a decimal number on the screen
* 
* knob.getAngle();                                      // returns the degrees the knob is turned (0-280)
* 
* touch.get();                                          // returns the pin number of the touched area, 
*                                                       // if nothing is touched it returns -1
*                                                     
* mp3.getStatus();                                      // returns the status of MP3 player
*                                                       // options include: PlayerSTOP, PLAY, PAUSE, FORWARD, REWIND
* mp3.setVolume(int8_t vol);                            // vol is 0~0x1e, 30 adjustable level
* mp3.playWithFileName(int8_t directory, int8_t file);  // play a song according to the folder name and prefix of its file name
*                                                       // folder name must be 01 02 03...09 10...99
*                                                       // prefix of file name must be 001...009 010...255
* clock.begin();                                        // this starts the clock
* clock.getTime(void);                                  // reads the clock and time, will be saved in the clock class variable
*                                                       // The clock class variable contains the following:
*                                                             // uint8_t second;	
*                                                             // uint8_t minute;	
*                                                             // uint8_t hour; 	
*                                                             // uint8_t dayOfWeek;	
*                                                             // uint8_t dayOfMonth;	
*                                                             // uint8_t month;	
*                                                             // uint16_t year;															
***********************************************************************************************************/
#include <TimerOne.h>                     // include the library for the timer

#include <EasyStarterKitMP3.h>            // include the MP3 library
#include <EasyStarterKitTouchSensor.h>    // include the touch sensor library
#include <EasyStarterKitDS1307.h>         // include the clock library
#include <EasyStarterKitBuzzer.h>         // include the buzzer library
#include <EasyStarterKitTemperature.h>    // include the temperature sensor library
#include <EasyStarterKitKnob.h>           // include the rotary knob library
#include <EasyStarterKit_LCD1602.h>       // include the LCD screen library

LiquidCrystal_I2C lcd(0x20);      // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
							                    // if 0x20 does not work, please modify it to be 0x38

#define KNOB_PIN A0               // SIG pin of Rotary Angle Sensor module corresponds to A0
Knob knob(KNOB_PIN);              // Setup the knob object

#define BUZZER_PIN 6              // SIG pin of Buzzer module corresponds to D6
Buzzer buzzer(BUZZER_PIN);        // Setup the buzzer object

#define NTC_PIN A1                // SIG pin of NTC module corresponds to A1 
Temperature temper(NTC_PIN);      // Setup temperature object

DS1307 clock;                     // DS1307 should connect to SDA and SCL via I2C port

int8_t Time[] = {0,0,0,0};        // Global array to hold the Time data

unsigned char ClockPoint = 1;     // Clock point flag, 1 = display clock point, 0 = not display

#define ON 1
#define OFF 0
unsigned char Update;             // Update flag, 1 = re-acquire time and display, 0 = no action

#define TOUCH_OUT1 10             // the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13             // the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11             // the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12             // the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);    // setup touch object

#define MP3_RX 4                  // RX (receive) of Serial MP3 module connects to D4 of Arduino
#define MP3_TX 2                  // TX (transmit)connects to D2
MP3 mp3(MP3_RX, MP3_TX);          // Setup MP3 object
int8_t volume = 0x1e;             // 0~0x1e (30 adjustable level)
int8_t oldVolume;
int8_t newVolume;                 // 0~0x1e (30 adjustable level)

int8_t folderName = 2;            // folder name must be 01 02 03 04 ...
int8_t fileName = 1;              // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2              // offset between number and file name, here the file named 0 is 002 and 1 is 003
#define TEN_OFFSET 20             // only for whole numbers divisible by ten,  
                                  // here the file named 20 is 22, 30 is 23, 40 is 24

void setup() {      // start setup(), setup() runs once when the program starts
  delay(500);       // 500ms wait for the MP3 module to initialize
  clock.begin();    // Start the clock
  lcd.init();	      // initialize the lcd 
  lcd.backlight();  // turn on the back light of LCD

  Timer1.initialize(500000);            // timing for 500ms
  Timer1.attachInterrupt(TimingISR);    // declare the interrupt routine:TimingISR 
  updateVolume();                       // set volume based on knob rotation
}

void loop() {                           // loop repeats over and over
  int button;                           // declare a variable named button of type int 
  uint8_t flag_speak_time = 0;          // 0 = not speak, 1 = to speak
  uint8_t flag_speak_temp = 0;          // 0 = not speak, 1 = to speak
  button = touch.get();                 // store the value of touched area in "button"
  if(button != -1){                     // if something was touched
    delay(10);                          // delay for 10ms
    buzzer.on();                        // sound the buzzer
    if(button == TOUCH_OUT1){           // if touched area corresponds to TCH1 area
      flag_speak_temp = 1;              // set the flag to read the temperature
    }
    else if(button == TOUCH_OUT2){      // if touched area corresponds to TCH2 area
      flag_speak_time = 1;              // set the flag to read the time
    }
  	delay(50);                          // delay for 50ms
  	buzzer.off();                       // turn the buzzer off
  	while(button == touch.get());       // Wait for the button to be released
  }
  float celsius;
  if(Update == ON){                     // update the display if interrupt timer says to
    TimeUpdate();                       // get the updated time
	  celsius = temper.getTemperature();  // get temperature
    displayTemperature(celsius);        // display updated temperature
    Update == OFF;                      // set Update flag to off
  }
  updateVolume();                       // set volume based on current angle of knob
  if(flag_speak_time){                  // if speak time flag was changed
    SpeakTime(Time);                    // speak the current time
	  flag_speak_time = 0;                // set the speak time flag to false
  }
  if(flag_speak_temp){                  // if speak temp flag was changed
    SpeakTemp(celsius);                 // speak the current temperature
	  flag_speak_temp = 0;                // set speak temp flag to false
  } 
}

/****************** USER DEFINED FUCTIONS ******************/

// This function updates the volume based on current angle of the knob
void updateVolume(){
  int angle;                                      // declare a variable named angle of type int
  angle = knob.getAngle();                        // returns the angle of the knob
  newVolume = map(angle, 0, FULL_ANGLE, 0, 30);   // chooses volume based on that measurement
  if(newVolume != oldVolume){                     // if volumes aren't the same
  	mp3.setVolume(newVolume);                     // set volume to the new value
  	oldVolume = newVolume;                        // update the oldVolume so current volume is used in next comparison
  	delay(50);                                    // wait for 50ms for these changes to apply
  }
}

// This function speaks the current time
void SpeakTime(int8_t time[]){
  uint8_t addr[10] = {0};                                 // we use this array to construct the sentences
  uint8_t next = 0;
  addr[next++] = 31;                                      // 031 plays a bell before speaking
  addr[next++] = 32;                                      // 032 says "the time is"
  if(Time[0] < 2){                                        // if the hour is less than 20
    addr[next++] = Time[0]*10 + Time[1]+NUM_OFFSET;       // add the sound clip to the array with NUM_OFFSET
  }
  else{                                                   // if it is larger than 19
    addr[next++] = 22;//022 twenty                        // add the clip to say "twenty"
    if(Time[1] != 0) addr[next++] = Time[1] + NUM_OFFSET; // then add the clip for the second digit
  }
  if((Time[2] == 0) && (Time[3] == 0)) addr[next++] = 33; // add clip 033 to say "O' clock" if there is no minutes
  else{                                                   
    if(Time[2] < 2){                                      // if there are minutes and its less than 20
  	  addr[next++] = Time[2]*10 + Time[3]+NUM_OFFSET;     // add corresponding clip to the array
  	}
  	else{
  	  addr[next++] = Time[2] + TEN_OFFSET;                  // otherwise use the Ten offset to speak the first digit
  	  if(Time[3] != 0)addr[next++] = Time[3] + NUM_OFFSET;  // then add the second digit normally
  	}
  }
  SpeakGroup(addr, next);                                   // send the array we constructed to the function to read it
}

// This function reads the current temperature
void SpeakTemp(float temp){
  if(temp >= 1000)return;                 // we only have audio files to support up to 999
  else if(temp <= -1000)return;           // however, if the reading was this temperature this is surely an issue
  
  uint8_t addr[10] = {0};                 // we use this array to construct the sentences
  uint8_t next = 0;
  addr[next++] = 31;                      // 031 plays a bell before speaking
  addr[next++] = 35;                      // 035 says "the temperature is"
  if(temp < 0){                           // if the temperature is negative
    temp = abs(temp);                     // take the absolute value
	  addr[next++] = 40;                    // 040 says "minus"
  }
  int t = temp;                           // cast the temperature as an int (truncate the decimals)
  uint8_t flag_hundred;
  if(t >= 100){                           // if temperature is greater than 100
    flag_hundred = 1;                     // set the flag to true
  	addr[next++] = t / 100 + NUM_OFFSET;  // add audio for digit before hundred (1, 2, 3...)
	  addr[next++] = 30;                    // add clip 030 to array it says "hundred"
	  t %= 100;                             // get the remainder of the temperature
  }
  else flag_hundred = 0;                  // if temp is lower than 100 set flag to false
  if(t != 0){                             // if temp is not 0
   	if(flag_hundred)addr[next++] = 38;    // if temp is above hundred say "and" with clip 038
  	if(t < 20){                           // if temp remaining is less than 20
  	  addr[next++] = t + NUM_OFFSET;      // add to array as normal
  	}
    else{
  	  addr[next++] = t / 10 + TEN_OFFSET;         // otherwise, use the ten offset for first digit
	    t %= 10;                                    // get the remainder
	    if(t != 0)addr[next++] = t + NUM_OFFSET;    // add clip for the remainder
  	}
  }
 
  addr[next++] = 1;                       // add clip 001 which says "point"
 
  uint8_t subbit;                         // declare a variable named subbit of type unsigned 8 bit int
  subbit = ((int)(temp*10))%10;           // multiply float by 10, then modulus by ten to get the last digit
  addr[next++] = subbit + NUM_OFFSET;     // add this to array for the degree in decimal
  addr[next++] = 36;                      // add clip 036 to say "degrees Celsius"
  SpeakGroup(addr, next);                 // send this array and the amount of items (stored in next) to the function to speak it
}

// This function plays out constructed sentences audio files.
void SpeakGroup(uint8_t addr[], uint8_t size){
  for(uint8_t i=0; i < size; i ++){                // for each element in the array
    while(mp3.getStatus()!=STATUS_STOP)delay(50);  // wait 50ms between each clip
	  mp3.playWithFileName(folderName,addr[i]);      // play the audio file from the folder
  }
  while(mp3.getStatus()!=STATUS_STOP)delay(50);    // wait 50ms after it finishes the sentence
}

void displayTemperature(float temp){                // this function displays temp on the lcd display
  if((temp<-40)||(temp>125))return;                 // if temperature is unreasonable exit the function
  lcd.setCursor(0, 1);                              // set cursor to the first column, second row
  lcd.print("Temp: ");                              // print "Temp: " to the screen
  if(temp < 0){                                     // if the temperature is negative
    temp = abs(temp);                               // take absolute value
    lcd.print("-");                                 // add the "-" to the screen 
  }
  if((temp < 100)&&(temp>=10))lcd.print(" ");       // if the temp is two digits, print a space for hundredths place
  else if(temp < 10)lcd.print("  ");                // if the temp is one digit, print two spaces
  lcd.print(temp,1);                                // print the temperature
  lcd.print(" 'C");                                 // add the Celsius symbol to the screen
} 

/********** Timer1 Interrupt Routine **********/
void TimingISR(){
  Update = ON;                // Set update flag to ON
  ClockPoint = !ClockPoint;   // Set checkpoint to opposite of whatever it is now
}

void TimeUpdate(void){                    // this function updates the time on the lcd
  clock.getTime();                        // get the current time
  Time[0] = clock.hour / 10;              // store the hour's first digit
  Time[1] = clock.hour % 10;              // store the hour's second digit
  Time[2] = clock.minute / 10;            // store minute's first digit
  Time[3] = clock.minute % 10;            // store minute's second digit
  
  lcd.setCursor(0, 0);                    // start in first column, first row
  lcd.print("Time: ");                    // print "Time: "
  if(clock.hour < 10)lcd.print(" ");      // if there is no first digit in hour, print space
  lcd.print(clock.hour, DEC);             // print the hour
  lcd.print(":");                         // add colon between hour and minutes
  if(clock.minute < 10)lcd.print("0");    // if minutes has no first digit, print a 0
  lcd.print(clock.minute, DEC);           // print the minutes
  lcd.print(":");                         // print colon between minutes and seconds
  if(clock.second < 10)lcd.print("0");    // if there is no first digit, print a 0
  lcd.print(clock.second, DEC);           // print the seconds
  
  Update = OFF;                           // set update flag to OFF
}

/*********************************************************************************************************
The end of file
*********************************************************************************************************/
