/*
 * Driver pour le sonar à ultrasons
 * HC_SR04.h
 */

#ifndef HC_SR04_H
#define HC_SR04_H

#include "Arduino.h"

class HC_SR04
{
	public:
		// Constructor
		HC_SR04(int _trig, int _echo);
		
		// Fonctions/méthodes
		int getTrig();
		int getEcho();
		unsigned long getTime();
		int diagnose(void);
		
	private:
		int trig,echo;
		
};

#endif