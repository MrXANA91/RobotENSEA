/*
  Main file for the Arduino program.
  
*/
// Include libraries :
#include <MS5803_02BA.h>

// Variables globales
const int cSPin = 7;
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

MS5803_02BA sensor(cSPin); // initialisation du capteur de pression

void setup(){
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  // Initialize SPI
  SPI.begin();
  
  // Reset sensor
  sensor.resetSensor();
}

void loop(){
  // Command handling
  if (stringComplete) {

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
