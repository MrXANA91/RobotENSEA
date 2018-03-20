/*
 * Driver pour le capteur de pression/temperature MS5803_02BA
 * MS5803_02BA.h
 */
#ifndef MS5803_02BA_h
#define MS5803_02BA_h

#include "Arduino.h"
#include <SPI.h>

// constantes d'erreur
#define MS5803_02BA_OK 0
#define MS5803_02BA_OSR 1

// constantes de commandes
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
const int NONE = 0x00;

class MS5803_02BA
{
	public:
		// Constructor :
		MS5803_02BA(int _chipSelectPin);
		
		// Fonctions/méthodes
		void resetSensor(void);
		int getValues(unsigned long*,unsigned long*,int);
		int diagnose(void);
		
	private:
		// Pin CS
		int chipSelectPin;
		// Variable de la PROM
		unsigned int C1,C2,C3,C4,C5,C6;
		// Pression (D1) et Température (D2) non compensée
		unsigned long D1,D2;
		
		// Fonctions/méthodes
		unsigned int readProm(int);
		int readAndWriteSPI(int);
		unsigned long readADC(void);
		void temperatureCompensatedPressure(unsigned long*, unsigned long*, unsigned long, unsigned long);
};

#endif
