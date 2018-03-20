/*
 * Driver pour la sonde de monoxyde de carbone MQ-7
 * MQ7.cpp
 */

#include "Arduino.h"
#include "MQ7.h"

// PUBLIC :
// @brief : Constructor
MQ7::MQ7(int _dig_IN,int _an_IN){
	pinMode(_dig_IN,INPUT);
	pinMode(_an_IN,INPUT);
	dig_IN = _dig_IN;
	an_IN = _an_IN;
}

int MQ7::getValue(){
	return analogRead(an_IN);
}

int MQ7::isThereCO(){
	return digitalRead(dig_IN);
}

// @brief : outil de diagnostique automatisé
// 0 -> OK ; 1 -> value null .
int MQ7::diagnose(void){
	int ret=0;
	if (getValue() == 0){
		ret=1;
	}
	return ret;
}