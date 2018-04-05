/*
 * Driver pour la sonde de monoxyde de carbone MQ-7
 * MQ7.h
 */
#ifndef MQ7_h
#define MQ7_h

#include "Arduino.h"

class MQ7
{
	public:
		// Constructor
		MQ7(int _dig_IN,int _an_IN);
		
		int getValue();
		int isThisSafe();
		int diagnose(void);
	
	private:
		int dig_IN,an_IN;
};

#endif