/*
 * L00_Hello_Arduino
 * 
 * Sends "Hello!" to the serial terminal every 1 second
 */

//setup() runs once when the program is first started
void setup() {
  Serial.begin(9600);           //sets up serial comunication at 9600 baud
  delay(2500);                  //waits for 2.5 seconds (delay() takes milliseconds)
}

//loop() runs over and over again forever
void loop() {
  Serial.println("Hello!");     //sends "Hello!" and a new line character to the serial terminal
  delay(1000);                  //waits for 1 second
}
