/*
 * Driver pour le capteur de pression/temperature MS5803_02BA
 * MS5803_02BA.cpp
 */

#include "Arduino.h"
#include "MS5803_02BA.h"

// PUBLIC :
// @brief : Constructor.
MS5803_02BA::MS5803_02BA(int _chipSelectPin)
{
	// Initialisation du CS
	pinMode(_chipSelectPin, OUTPUT);
	chipSelectPin = _chipSelectPin;
  
	// Initialize SPI
	SPI.begin();
}

// @brief : getValues (utilisation de pointeurs)
int MS5803_02BA::getValues(unsigned long* pression, unsigned long* temperature, int OSR){
	if (OSR!=256 && OSR!=512 && OSR!=1024 && OSR!=2048 && OSR!=4096){
		return MS5803_02BA_OSR;
	}
	if (OSR==256){
	    // ** Read digital pressure and temperature data **
	    // get the uncompensated pressure
	    readAndWriteSPI(CONVERTD1_256);
	    delay(1);
	    D1 = readADC();
	    
	    // get the uncompensated temperature
	    readAndWriteSPI(CONVERTD2_256);
	    delay(1);
	    D2 = readADC();

	    temperatureCompensatedPressure(pression, temperature, D1, D2);
	}
	if (OSR==512){
	    // ** Read digital pressure and temperature data **
	    // get the uncompensated pressure
	    readAndWriteSPI(CONVERTD1_512);
	    delay(2);
	    D1 = readADC();
	    
	    // get the uncompensated temperature
	    readAndWriteSPI(CONVERTD2_512);
	    delay(2);
	    D2 = readADC();

	    temperatureCompensatedPressure(pression, temperature, D1, D2);
	}
	if (OSR==1024){
	    // ** Read digital pressure and temperature data **
	    // get the uncompensated pressure
	    readAndWriteSPI(CONVERTD1_1024);
	    delay(3);
	    D1 = readADC();
	    
	    // get the uncompensated temperature
	    readAndWriteSPI(CONVERTD2_1024);
	    delay(3);
	    D2 = readADC();

	    temperatureCompensatedPressure(pression, temperature, D1, D2);
	}
	if (OSR==2048){
	    // ** Read digital pressure and temperature data **
	    // get the uncompensated pressure
	    readAndWriteSPI(CONVERTD1_2048);
	    delay(5);
	    D1 = readADC();
	    
	    // get the uncompensated temperature
	    readAndWriteSPI(CONVERTD2_2048);
	    delay(5);
	    D2 = readADC();

	    temperatureCompensatedPressure(pression, temperature, D1, D2);
	}
	if (OSR==4096){
	    // ** Read digital pressure and temperature data **
	    // get the uncompensated pressure
	    readAndWriteSPI(CONVERTD1_4096);
	    delay(10);
	    D1 = readADC();
	    
	    // get the uncompensated temperature
	    readAndWriteSPI(CONVERTD2_4096);
	    delay(10);
	    D2 = readADC();

	    temperatureCompensatedPressure(pression, temperature, D1, D2);
	}
	return MS5803_02BA_OK;
}

// @brief : fonction de reset du capteur
void MS5803_02BA::resetSensor(){
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

// PRIVATE :
// @brief : fonction pour la lecture de la PROM
unsigned int MS5803_02BA::readProm(int adress) {
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
int MS5803_02BA::readAndWriteSPI(int value) {
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
// ATTENTION : laissez bien 10ms apres une conversion avant d'utiliser cette commande
unsigned long MS5803_02BA::readADC() {
  unsigned long res,res2,res3;
  
  digitalWrite(chipSelectPin, LOW);                             // On met le CSB à l'état BAS
  delay(1);                                                     // (temporisation de sécurité)
  SPI.beginTransaction(SPISettings(10000,MSBFIRST,SPI_MODE0));  // On initialise le module SPI avec la fréquence de l'horloge, le sens de lecture des données 
                                                                // et le mode de l'horloge
                                                                // (à noter "MSBFIRST", bit de poids fort en premier)
  SPI.transfer(ADC_READ);                                       // On envoie la commande de lecture du convertisseur
  res = SPI.transfer(NONE);                                     // On récupère les 8 premiers bits
  res2 = SPI.transfer(NONE);                                    // On récupère les 8 bits suivants
  res3 = SPI.transfer(NONE);                                    // On récupère les 8 derniers bits

  SPI.endTransaction();                                         // On arrête l'échange SPI
  delay(1);                                                     // (temporisation de sécurité)
  digitalWrite(chipSelectPin, HIGH);                            // On repasse le CSB à l'état HAUT

  return ( (res*pow(2,16)) + (res2*pow(2,8)) + res3 );          // On renvoie la valeur sur 24 bits, en prenant en compte le MSBFIRST
}

// @brief : fonction de calcul de la température et de la pression
void MS5803_02BA::temperatureCompensatedPressure(unsigned long* pression, unsigned long* temperature, unsigned long D1, unsigned long D2){
  unsigned long DT,T2 = 0;
  double OFF,SENS,OFF2,SENS2 = 0;                                     // D1 est la valeur brute de la pression
                                                                      // D2 est la valeur brute de la température
  // ** Calculate temperature **
  DT = D2 - C5*( pow(2,8) );                                          // DT = D2 - Tref
  (*temperature) = 2000 + DT*C6/( pow(2,23) );                        // TEMP = 20°C + DT*TEMPSENS

  // ** Calculate temperature compensated values **
  OFF = C2*( pow(2,17) ) + (C4*DT)/( pow(2,6) );                      // OFF = OFFt1 + TCO*DT
  SENS = C1*( pow(2,16) ) + (C3*DT)/( pow(2,7) );                     // SENS = SENSt1 + TCS*DT

  // ** Calculate second order temperature compensated values **
  if ((*temperature) < 2000){                                         // 
    T2 = pow(DT,2)/pow(2,31);                                         // 
    OFF2 = 61 * pow(((*temperature)-2000),2)/pow(2,4);                // 
    SENS2 = 2 * pow(((*temperature)-2000),2);                         // 
    if ((*temperature) < -1500){                                      // 
      OFF2 = OFF2 + 20 * pow(((*temperature) + 1500),2);              // 
      SENS2 = SENS2 + 12 * pow(((*temperature) + 1500),2);            // 
    }
  }
  (*temperature) = (*temperature) - T2;                               // 
  OFF = OFF - OFF2;                                                   // 
  SENS = SENS - SENS2;                                                // 

  // ** Calculate temperature compensated pressure **

  (*pression) = (D1*SENS/( pow(2,21) ) - OFF)/( pow(2,15) );          // P = D1*SENS - OFF
}

// @brief : outil de diagnostique automatisé.
// 0 -> OK ; -1 -> const null ; 1 -> const not in range .
int MS5803_02BA::diagnose(void){
	// réinitialisation du capteur
	resetSensor();
	// analyse des constantes
	if ( (C1==0) || (C2==0) || (C3==0) || (C4==0) || (C5==0) || (C6==0) ){
		return -1;
	}
	if ( (C1>51372) || (C1<41372) || (C2>48981) || (C2<38981) || (C3>34059) || (C3<24059) || (C4>32842) || (C4<22842) || (C5>36553) || (C5<26553) || (C6>33165) || (C6<23165) ){
		return 1;
	}
	return 0;
}