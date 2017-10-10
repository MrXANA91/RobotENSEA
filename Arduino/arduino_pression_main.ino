#include <SPI.h>

// numero des broches
const int chipSelectPin = 7;
/*
 * MOSI : 11
 * MISO : 12
 * SCK : 13
 */

// constantes de commande
const int RESET = 0x1E;
const int CONVERTD1_256 = 0x40;
const int CONVERTD1_512 = 0x42;
const int CONVERTD1_1024 = 0x44;
const int CONVERTD1_2048 = 0x46;
const int CONVERTD1_4096 = 0x48;
const int CONVERTD2_256 = 0x50;
const int CONVERTD2_512 = 0x52;
const int CONVERTD2_1024 = 0x54;
const int CONVERTD2_2048 = 0x56;
const int CONVERTD2_4096 = 0x58;
const int ADC_READ = 0x00;
const int PROM_READ = 0xA0;
const int NONE = 0x11;

// constantes PROM
unsigned int C1;
unsigned int C2;
unsigned int C3;
unsigned int C4;
unsigned int C5;
unsigned int C6;

// déclaration de fonctions
int readProm(int);
int readAndWriteSPI(int);
unsigned long readADC(void);

// test
int flag,boucle;
String inputString = "";
boolean stringComplete = false;  

void setup() {
  // Initialisation de la console PC
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  flag = 1;
  boucle = 0;
  
  // Initialisation des broches SPI et CS
  pinMode(chipSelectPin, OUTPUT);
  SPI.begin();

  // Commande de reinitialisation
  readAndWriteSPI(RESET);

  // Attente d'au moins 2.8 ms
  delay(10);

  // Lecture de la PROM et stockage dans les constantes PROM
  C1 = readProm(1);
  C2 = readProm(2);
  C3 = readProm(3);
  C4 = readProm(4);
  C5 = readProm(5);
  C6 = readProm(6);

  // Attente de fin de setup
  delay(100);
}

void loop() {
  unsigned long D0,D1,D2,DT,TEMP,P = 0;
  double OFF,SENS = 0;
  char buf[50] = "";
  if (flag){
    // just once
    flag = 0;

    // ** Read digital pressure and temperature data **
    // get the uncompensated pressure
    readAndWriteSPI(CONVERTD1_1024);
    delay(10);
    D1 = readADC();
    
    // get the uncompensated temperature
    readAndWriteSPI(CONVERTD2_1024);
    delay(10);
    D2 = readADC();

    // ** Calculate temperature **
    DT = D2 - C5*( pow(2,8) );
    TEMP = 2000 + DT*C6/( pow(2,23) );

    // ** Calculate temperature compensated pressure **
    OFF = C2*( pow(2,17) ) + (C4*DT)/( pow(2,6) );
    SENS = C1*( pow(2,16) ) + (C3*DT)/( pow(2,7) );

    P = (D1*SENS/( pow(2,21) ) - OFF)/( pow(2,15) );

    delay(100);
    
    // ** affichage de la pression et de la temperature **
    Serial.println(C1);
    Serial.println(C2);
    Serial.println(C3);
    Serial.println(C4);
    Serial.println(C5);
    Serial.println(C6);
    Serial.print("OFF = ");
    Serial.println(OFF);
    Serial.print("SENS = ");
    Serial.println(SENS);
    delay(20);
    ltoa(D1, buf, 10); // conversion LONG vers STRING
    Serial.print("D2 = ");
    Serial.println(D2);
    delay(20);
    ltoa(TEMP, buf, 10); // conversion LONG vers STRING
    Serial.print("Temperature : ");
    Serial.println(TEMP);
    delay(20);
    ltoa(D2, buf, 10); // conversion LONG vers STRING
    Serial.print("D1 = ");
    Serial.println(D1);
    ltoa(P, buf, 10); // conversion LONG vers STRING
    Serial.print("Pression : ");
    Serial.println(P);
    delay(20);
    Serial.println("");
    delay(20);
  }
}

// @brief : fonction pour la lecture du PROM
int readProm(int adress) {
  int res,res2;

  digitalWrite(chipSelectPin, LOW);
  delay(1);
  SPI.beginTransaction(SPISettings(10000,MSBFIRST,SPI_MODE0));

  SPI.transfer(PROM_READ + (adress << 1));
  res = SPI.transfer(NONE);
  res2 = SPI.transfer(NONE);

  SPI.endTransaction();
  delay(1);
  digitalWrite(chipSelectPin, HIGH);

  return ( (res<<8) + res2 );
}

// @brief : fonction pour l'echange de donnees sur l'interface SPI
int readAndWriteSPI(int value) {
  int res;
  
  digitalWrite(chipSelectPin, LOW);
  delay(1);
  SPI.beginTransaction(SPISettings(10000,MSBFIRST,SPI_MODE0));

  res = SPI.transfer(value);

  SPI.endTransaction();
  delay(1);
  digitalWrite(chipSelectPin, HIGH);

  return res;
}

// @brief : lecture des donnees apres conversion
// ATTENTION : laissez bien 20ms apres une conversion avant d'utiliser cette commande
unsigned long readADC() {
  unsigned long res,res2,res3;
  
  digitalWrite(chipSelectPin, LOW);
  delay(1);
  SPI.beginTransaction(SPISettings(10000,MSBFIRST,SPI_MODE0));

  SPI.transfer(ADC_READ);
  res = SPI.transfer(NONE);
  res2 = SPI.transfer(NONE);
  res3 = SPI.transfer(NONE);

  SPI.endTransaction();
  delay(1);
  digitalWrite(chipSelectPin, HIGH);

  return ( (res*pow(2,16)) + (res2*pow(2,8)) + res3 );
 
}

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
