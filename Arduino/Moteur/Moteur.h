/*
*Librairiepour le moteur
*Moteur.h
*/

#ifndef Moteur_H
#define Moteur_H

#include "Arduino.h"

class Moteur
{
	public:
		// Constructor
		Moteur(int ML_p, int ML_m, int MR_p, int MR_m, int ENL, int ENR);
		
		// Fonctions/méthodes
		void avance();
		void recul();
		void droite();
		void gauche();
		void arret();
		float getStatus();
		
	private:
		int _ML_p;
		int _ML_m;
		int _MR_p;
		int _MR_m;
		int _ENL;
		int _ENR;
		
		float status;
};

#endif