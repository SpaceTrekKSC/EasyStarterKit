/***********************************************************************************************************
* Easy Starter Kit Lesson 34: Speak temperature
* 
* In this lesson you will learn how to read the temperature and play that through the speakers of the MP3
* player, you will also use the touch sensor to act as a play button to start reading the temperature displayed
* on your LCD screen aloud aloud. 
* 
* Board Configuration:
*       Ensure the SD card is inserted in the back of the MP3 player
*       Connect the speaker to the white connector on the MP3 player
*       Connect the MP3 player to the yellow connector on the Arduino labeled D4 and D2
*       Connect the LCD Screen to the other free yellow I2C port
*       Connect your touch sensor to the red SPI port
*       Connect your NTC sensor to its adapter
*       Connect the adapter to the blue connector on the Arduino labeled A1
* 
* Before we get started add the sound recordings from the google drive to your SD card.
* https://drive.google.com/drive/folders/1yWyeuK4em8zeGK2CnpOeYN-mcKpOM52l?usp=sharing
* 
* The files must be on the SD card the exact way they are on the drive.
*     // Folder names must be 01 02
*     // Inside the folder each audio clip must be labeled 000 001 002 003 ... 
*     
* The following functions are available:
* 
* timer1.initialize(unsigned long microseconds);        // set a timer for given microseconds
* timer1.attachInterrupt (TimingISR);                   // sets a timing interrupt routine
* 
* lcd.init();                                           // I2C 1602 LCD screen initialization
* lcd.backlight();                                      // turn on the back light of the screen
* lcd.noBacklight();                                    // turn off the back light of the screen
* lcd.setCursor(uint8_t col, uint8_t row);              // set the cursor position
* lcd.print(Sring str);                                 // display string on the screen
* lcd.print(double n, int digits);                      // display a decimal number on the screen
* 
* temper.getTemperature();                              // returns the temperature value as a float
* 
* displayTemperature(float temperature);                // display temperature on LCD screen
* 
* touch.get();                                          // returns the pin number of the touched area, 
*                                                       // if nothing is touched it returns -1

* mp3.getStatus();                                      // returns the status of MP3 player
*                                                       // options include: PlayerSTOP, PLAY, PAUSE, FORWARD, REWIND
* mp3.setVolume(int8_t vol);                            // vol is 0~0x1e, 30 adjustable level
* mp3.playWithFileName(int8_t directory, int8_t file);  // play a song according to the folder name and prefix of its file name
*                                                       // folder name must be 01 02 03...09 10...99
*                                                       // prefix of file name must be 001...009 010...255
***********************************************************************************************************/
#include <TimerOne.h>                     // include the library for the timer

#include <EasyStarterKitMP3.h>            // include the MP3 library
#include <EasyStarterKitTemperature.h>    // include the temperature sensor library
#include <EasyStarterKitTouchSensor.h>    // include the touch sensor library
#include <EasyStarterKit_LCD1602.h>       // include the LCD screen library

LiquidCrystal_I2C lcd(0x20);      // set the LCD address to 0x20 for the 16 chars and 2 line display (default setting)
                                  // if 0x20 does not work, please modify it to be 0x38
						
#define ON 1                      // Define variable ON to 1
#define OFF 0                     // Define variable OFF to 0
unsigned char Update;             // Update flag, 1 = re-acquire time and display, 0 = no action

#define TOUCH_OUT1 10             // the pin that out1-TCH1 area corresponds to, D10
#define TOUCH_OUT2 13             // the pin that out2-TCH2 area corresponds to, SCK (D13)
#define TOUCH_OUT3 11             // the pin that out3-TCH3 area corresponds to, MOSI (D11)
#define TOUCH_OUT4 12             // the pin that out4-TCH4 area corresponds to, MISO (D12)
TouchSensor touch(TOUCH_OUT1,TOUCH_OUT2, TOUCH_OUT3,TOUCH_OUT4);    // setup touch object

#define NTC_PIN A1                // SIG pin of NTC module corresponds to A1 
Temperature temper(NTC_PIN);      // Setup temperature object

#define MP3_RX 4                  // RX (receive) of Serial MP3 module connects to D4 of Arduino
#define MP3_TX 2                  // TX (transmit)connects to D2
MP3 mp3(MP3_RX, MP3_TX);          // Setup MP3 object
int8_t volume = 0x1e;             // 0~0x1e (30 adjustable level)
int8_t folderName = 2;            // folder name must be 01 02 03 04 ...
int8_t fileName = 1;              // prefix of file name must be 001xxx 002xxx 003xxx 004xxx ...099xxx
#define NUM_OFFSET 2              // offset between number and file name, here the file named 0 is 002 and 1 is 003
#define TEN_OFFSET 20             // offset between number and file name, here the file named 10 is 012

void setup() {                          // start setup(), setup() runs once when the program starts
  delay(500);                           // 500ms wait for the MP3 module to initialize
  lcd.init();                           // initialize the lcd 
  lcd.backlight();                      // turn on the back light of LCD
  Timer1.initialize(500000);            // timing for 500ms
  Timer1.attachInterrupt(TimingISR);    // declare the interrupt routine:TimingISR 
  mp3.setVolume(volume);                // set volume based on our above declared variable
  delay(50);                            // wait for 50ms
}

void loop() {                           // loop repeats over and over
  int button;                           // declare a variable named button of type int 
  uint8_t flag_speak = 0;               // 0 = not speak, 1 = to 
  button = touch.get();                 // store the value of touched area in "button"
  if(button == TOUCH_OUT1){             // if touched area corresponds to TCH1 area
    delay(10);                          // delay for 10ms
    if(touch.get() == TOUCH_OUT1){      // check it again to ensure a press
      flag_speak = 1;                   // set the flag to speak to true
    }
	  while(touch.get() == TOUCH_OUT1);   // Wait for the button to be released
  }
  float celsius;                        // declare variable named celsius of type float
  if(Update == ON){                     // update the display if the interupt timer says to
    Update = OFF;                       // reset the Update flag to OFF
    celsius = temper.getTemperature();  // get temperature and store it in celsius
    displayTemperature(celsius);        // display updated temperature
  }
  if(flag_speak){                       // if speak temp flag was changed
    SpeakTemp(celsius);                 // speak the current temperature
    flag_speak = 0;                     // set speak temp flag to false
    
  } 
  delay(100);                           // wait for 100ms
}

// This function reads the current temperature aloud
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

// This function takes the current temperature and then displays it to the LCD screen
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
  Update = ON;      // set our update flag to ON
}
/*********************************************************************************************************
The end of file
*********************************************************************************************************/
