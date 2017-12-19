
#include "stk/Mecanics.h"

//Calcul l'acceleration ds le rep global d'un point M d'un solide en mouvement, fixe sur ce solide
stk::Vector3 stk::AbsoluteAccelerationAtPoint(const stk::Vector3& AccelerationAbsolueG,	//vecteur acceleration de G dans le repere global
								const stk::Vector3& Wterre,		//vecteur vitesse rotation instantannée	dans le repere global
								const stk::Vector3& dWterre,		//vecteur acceleration rotation  instantannée dans le repere global
								const stk::Vector3& GM)			//Vecteur GM dans le repere global
{
	return AccelerationAbsolueG + (dWterre^GM) + (Wterre^(Wterre^GM));
}
