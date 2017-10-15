/*
  Based on 'Blink' and 'SerialEvent'
  Test connection between Arduino and Raspberry(Node Red)

  Controling the frequency of the flashed of the LED with Node Red
*/

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

float freq = 0;              // Frequency of the Led flashes

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  
  // initialize serial:
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}

// the loop function runs over and over again forever
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    freq = inputString.toFloat();

    // clear the string:
    inputString = "";
    stringComplete = false;
  }
  if (freq!=0) {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay((1/freq)*1000);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay((1/freq)*1000);                       // wait for a second
  } else {
    digitalWrite(LED_BUILTIN, LOW);
    
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
