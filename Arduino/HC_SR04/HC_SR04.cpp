/*
 * Driver pour le sonar à ultrasons
 * HC_SR04.cpp
 */

#include "Arduino.h"
#include "HC_SR04.h"

// PUBLIC :
// @brief : constructor
HC_SR04::HC_SR04(int _trig, int _echo){
	// Initialisation du ECHO
	pinMode(_echo,INPUT);
	echo = _echo;

	// Initialisation du TRIG
	pinMode(_trig,OUTPUT);
	trig = _trig;
}

// @brief : retourne le pin TRIG
int HC_SR04::getTrig(void){
	return trig;
}

// @brief : retourne le pin ECHO
int HC_SR04::getEcho(void){
	return echo;
}

// @brief : retourne la valeur de la distance jusqu'à l'objet le plus proche
// 0 si il y a un probleme
unsigned long HC_SR04::getTime(void){
	unsigned long time_var;
	digitalWrite(trig,HIGH);
    delayMicroseconds(15);
    digitalWrite(trig,LOW);
	
	time_var = micros();
    while(digitalRead(echo) == LOW){
		if((micros()-time_var)>50000){
			return 0;
		}
	}
    time_var = micros();
    while(digitalRead(echo) == HIGH){
		if((micros()-time_var)>50000){
			return 0;
		}
	}
    time_var = micros() - time_var;
	
	return time_var;
}

// @brief : outil de diagnostique automatisé
// 0 -> OK ; -1 -> not responding ; 1 -> get stuck ; 2 -> value not in range.
int HC_SR04::diagnose(void){
	unsigned long time_var;
	digitalWrite(trig,HIGH);
	delayMicroseconds(15);
	digitalWrite(trig,LOW);
	
	time_var = micros();
	while(digitalRead(echo) == LOW){
		if((micros()-time_var)>50000){
			return -1;
		}
	}
    time_var = micros();
    while(digitalRead(echo) == HIGH){
		if((micros()-time_var)>50000){
			return 1;
		}
	}
	time_var = micros() - time_var;
	if( (time_var>40000) || (time_var<200) ){
		return 2;
	}
	return 0;
}
