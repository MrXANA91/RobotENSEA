#include "Moteur.h"
#include "Arduino.h"

Moteur::Moteur(int _ML_p, int _ML_m, int _MR_p, int _MR_m, int _ENL, int _ENR)
{
	ML_p=_ML_p;
	ML_m=_ML_m;
	MR_p=_MR_p;
	MR_m=_MR_m;
	ENL=_ENL;
	ENR=_ENR;
	analogWrite(ENL,43);
	analogWrite(ENR,43);
}
	
void Moteur::avance(){
	digitalWrite(ML_p,1);
	digitalWrite(ML_m,0);
	digitalWrite(MR_p,1);
	digitalWrite(MR_m,0);
}
void Moteur::droite(){
	digitalWrite(ML_p,1);
	digitalWrite(ML_m,0);
	digitalWrite(MR_p,0);
	digitalWrite(MR_m,1);
}
void Moteur::gauche(){
	digitalWrite(ML_p,0);
	digitalWrite(ML_m,1);
	digitalWrite(MR_p,1);
	digitalWrite(MR_m,0);
}
void Moteur::recul(){
	digitalWrite(ML_p,0);
	digitalWrite(ML_m,1);
	digitalWrite(MR_p,0);
	digitalWrite(MR_m,1);
}
void Moteur::arret(){
	digitalWrite(ML_p,0);
	digitalWrite(ML_m,0);
	digitalWrite(MR_p,0);
	digitalWrite(MR_m,0);
}