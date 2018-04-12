/*
 * Driver pour le sonar à ultrasons
 * HC_SR04.cpp
 */

#include "Arduino.h"
#include "HC_SR04.h"

// @brief : constructor
HC_SR04::HC_SR04(int trig, int echo){
	// Initialisation du ECHO
	pinMode(echo,INPUT);
	_echo = echo;

	// Initialisation du TRIG
	pinMode(trig,OUTPUT);
	_trig = trig;
}

// @brief : retourne le pin TRIG
int HC_SR04::getTrig(void){
	return _trig;
}

// @brief : retourne le pin ECHO
int HC_SR04::getEcho(void){
	return _echo;
}

// @brief : retourne la valeur de la distance jusqu'à l'objet le plus proche
// 0 si il y a un probleme
unsigned long HC_SR04::getTime(void){
	unsigned long time_var;
	digitalWrite(_trig,HIGH);
    delayMicroseconds(15);
    digitalWrite(_trig,LOW);
	
	time_var = micros();
    while(digitalRead(_echo) == LOW){
		if((micros()-time_var)>50000){
			return 0;
		}
	}
    time_var = micros();
    while(digitalRead(_echo) == HIGH){
		if((micros()-time_var)>50000){
			return 0;
		}
	}
    time_var = micros() - time_var;
	
	return time_var;
}

// @brief : outil de diagnostique automatisé
// 0 -> OK ; -1 -> not responding ; 1 -> get stuck ; 2 -> value not in range.
// A REVOIR (ne diagnostique pas correctement le composant)
int HC_SR04::diagnose(void){
	unsigned long time_var;
	digitalWrite(_trig,HIGH);
	delayMicroseconds(15);
	digitalWrite(_trig,LOW);
	
	time_var = micros();
	while(digitalRead(_echo) == LOW){
		if((micros()-time_var)>50000){
			return -1;
		}
	}
    time_var = micros();
    while(digitalRead(_echo) == HIGH){
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
