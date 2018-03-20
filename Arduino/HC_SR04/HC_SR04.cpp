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
unsigned long HC_SR04::getTime(void){
	unsigned long time_var;
	digitalWrite(trig,HIGH);
    delayMicroseconds(15);
    digitalWrite(trig,LOW);

    while(digitalRead(echo) == LOW);
    time_var = micros();
    while(digitalRead(echo) == HIGH);
    time_var = micros() - time_var;
	
	return time_var;
}