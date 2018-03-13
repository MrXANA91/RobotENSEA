/*
  Main file for the Arduino program.
  
*/
// Include libraries :
#include <MS5803_02BA.h>
#include <MQ7.h>

// ---------- Variables globales ----------
// - Numéro de broches :
const int cSPin = 7;
const int dig_IN = 2;
const int an_IN = A0;
// - Variables de mémoire
unsigned long TEMP,P = 0;		// température et pression
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

// - Périphériques :
MS5803_02BA pt_sensor(cSPin); // capteur de pression et de température
MQ7 co_sensor(dig_IN,an_IN); // sonde à monoxyde de carbone

void setup(){
	// initialize serial:
	Serial.begin(9600);
	// reserve 200 bytes for the inputString:
	inputString.reserve(200);
	
	// Initialisation du capteur de pression
	pt_sensor.resetSensor();
}

void loop(){
	// Variables temporaires
	char buf[50] = "";
	// Command handling
	if (stringComplete) {
		if (inputString.equals("ping\n")){
			// Test de la connexion
			Serial.println("pong");
		} else if (inputString.equals("pt_r_256")){
			// Lecture de la température et de la pression (OSR 256)
			pt_sensor.getValues(P,TEMP,256);
			ltoa(TEMP, buf, 10);
			Serial.println(TEMP)
			ltoa(P, buf, 10);
			Serial.println(P);
		} else if (inputString.equals("pt_r_512")){
			// Lecture de la température et de la pression (OSR 512)
			pt_sensor.getValues(P,TEMP,512);
			ltoa(TEMP, buf, 10);
			Serial.println(TEMP)
			ltoa(P, buf, 10);
			Serial.println(P);
		} else if (inputString.equals("pt_r_1024")){
			// Lecture de la température et de la pression (OSR 1024)
			pt_sensor.getValues(P,TEMP,1024);
			ltoa(TEMP, buf, 10);
			Serial.println(TEMP)
			ltoa(P, buf, 10);
			Serial.println(P);
		} else if (inputString.equals("pt_r_2048")){
			// Lecture de la température et de la pression (OSR 2048)
			pt_sensor.getValues(P,TEMP,2048);
			ltoa(TEMP, buf, 10);
			Serial.println(TEMP)
			ltoa(P, buf, 10);
			Serial.println(P);
		} else if (inputString.equals("pt_r_4096")){
			// Lecture de la température et de la pression (OSR 4096)
			pt_sensor.getValues(P,TEMP,4096);
			ltoa(TEMP, buf, 10);
			Serial.println(TEMP)
			ltoa(P, buf, 10);
			Serial.println(P);
		} else if (inputString.equals("pt_reset")){
			// Remise à zéro du capteur de pression
			pt_sensor.resetSensor();
		} else if (inputString.equals("cm_val")){
			// Lecture de la teneur en monoxyde de carbone
			Serial.println(MQ7.getValue());
		} else if (inputString.equals("cm_warn")){
			// Lecture avertissement (monoxyde de carbone)
			Serial.println(MQ7.isThereCO());
		} else if (inputString.equals("m_f")){
			// Motor : Move Forward
		
		} else if (inputString.equals("m_b")){
			// Motor : Move Backward
		
		} else if (inputString.equals("m_s")){
			// Motor : Stop
	
		} else if (inputString.equals("m_l")){
			// Motor : rotate left
		
		} else if (inputString.equals("m_r")){
			// Motor : rotate right
		
		} else if (inputString.equals("u_1")){
			// Lecture ultrason 1
		
		} else if (inputString.equals("u_2")){
			// Lecture ultrason 2
		
		} else if (inputString.equals("u_3")){
			// Lecture ultrason 3
	
		} else if (inputString.equals("u_4")){
			// Lecture ultrason 4
	
		} else {
		
		}
		Serial.println("OK");
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
