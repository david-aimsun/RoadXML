
#pragma once

#include "stk/STK.h"

#include <iostream>

namespace stk {

class STK_API PID
{
public:
	PID();
	PID(double kp, double ki, double kd, bool ResetIntegrale);
	
	double integre(double ValeurActuelle, double ValeurConsigne, double DeltaT);

	double integre(double ValeurActuelle, double VitesseActuelle, double ValeurConsigne, double VitesseConsigne, double DeltaT);

	bool isInitialised();

	std::ostream& describeParams(std::ostream& out);
private:

	bool isInit;	//permet de voir si on a utilisé le constructeur complet

	bool isResetIntegrale;	//est ce qu on reinitialise le cumul d'erreur quand l'erreur change de signe

	double kp;	//coeff proportionnel
	double ki;	//coeff intégrateur
	double kd;	//coeff dérivateur

	double error;
	double errorCumulee;
	double errorDerivee;
};

}
