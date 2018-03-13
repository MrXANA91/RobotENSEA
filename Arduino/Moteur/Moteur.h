/*
*Librairiepour le moteur
*Moteur.h
*/

#ifndef Moteur
#define Moteur

#include "Arduino.h"

class Moteur
{
	public:
		Moteur(int _ML_p, int _ML_m, int _MR_p, int _MR_m, int _ENL, int _ENR);
		void avance();
		void recul();
		void droite();
		void gauche();
		void arret();
		
	private:
		int ML_p;
		int ML_m;
		int MR_p;
		int MR_m;
		int ENL;
		int ENR;
}
#endif Moteur