#include "Moteur.h"
#include "Arduino.h"

// @brief : constructor
Moteur::Moteur(int ML_p, int ML_m, int MR_p, int MR_m, int ENL, int ENR)
{
	// variable de status
	status = 0.0
	
	// initialisation des différents sorties vers les moteurs
	_ML_p = ML_p;
	pinMode(_ML_p, OUTPUT);
	_ML_m = ML_m;
	pinMode(_ML_m, OUTPUT);
	_MR_p = MR_p;
	pinMode(_MR_p, OUTPUT);
	_MR_m = MR_m;
	pinMode(_MR_m, OUTPUT);
	
	
	// Initialisation de la PWM de l'Enable Left
	_ENL = ENL;
	pinMode(_ENL, OUTPUT);
	analogWrite(_ENL,43);
	
	// Initialisation de la PWM de l'Enable Right
	_ENR = ENR;
	pinMode(_ENR, OUTPUT);
	analogWrite(_ENR,43);
}

void Moteur::avance(){
	digitalWrite(_ML_p,1);
	digitalWrite(_ML_m,0);
	digitalWrite(_MR_p,1);
	digitalWrite(_MR_m,0);
	status = 1;
}
void Moteur::droite(){
	digitalWrite(_ML_p,1);
	digitalWrite(_ML_m,0);
	digitalWrite(_MR_p,0);
	digitalWrite(_MR_m,1);
	status = 0.5;
}
void Moteur::gauche(){
	digitalWrite(_ML_p,0);
	digitalWrite(_ML_m,1);
	digitalWrite(_MR_p,1);
	digitalWrite(_MR_m,0);
	status = -0.5;
}
void Moteur::recul(){
	digitalWrite(_ML_p,0);
	digitalWrite(_ML_m,1);
	digitalWrite(_MR_p,0);
	digitalWrite(_MR_m,1);
	status = -1.0;
}
void Moteur::arret(){
	digitalWrite(_ML_p,0);
	digitalWrite(_ML_m,0);
	digitalWrite(_MR_p,0);
	digitalWrite(_MR_m,0);
	status = 0.0;
}

float Moteur::getStatus(void){
	return status;
}