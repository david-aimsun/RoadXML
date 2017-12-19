
#include "stk/Torser.h"

using namespace stk;

stk::Torser::Torser(const Vector3& PtAppli, const Vector3& force, const Vector3&  moment) : 
	applicationPoint(PtAppli),
	force(force),
	moment(moment)
{}

stk::Torser::Torser(const Torser &T) :
	applicationPoint(T.applicationPoint),
	force(T.force),
	moment(T.moment)
{
}

//BUG passer le resultat en reference
Torser stk::Torser::TorseurEquivalent(const Vector3& NouveauPoint) const
{
	//moment(A) = moment(B) + AB^force
	Torser tmp;
	tmp.moment = moment + ((applicationPoint - NouveauPoint)^force);
	tmp.force = force;
	tmp.applicationPoint = NouveauPoint;
	return tmp;
}

Torser stk::Torser::operator+(const Torser&  A) const
{
	Torser tmp = A.TorseurEquivalent(applicationPoint);
	return Torser(applicationPoint, force + tmp.force, moment + tmp.moment);
}


Torser stk::Torser::operator-(const Torser&  A) const
{
	Torser tmp = A.TorseurEquivalent(applicationPoint);
	return Torser(applicationPoint, force - tmp.force, moment - tmp.moment);
}


Torser& stk::Torser::operator+=(const Torser&  A)
{
	Torser tmp = A.TorseurEquivalent(applicationPoint);
	force += tmp.force;
	moment += tmp.moment; 
	return *this ;
}

Torser& stk::Torser::operator-=(const Torser&  A)
{
	Torser tmp = A.TorseurEquivalent(applicationPoint);
	force -= tmp.force;
	moment -= tmp.moment; 
	return *this ;
}

Torser& stk::Torser::operator=(const Torser& A)
{
	applicationPoint = A.applicationPoint;
	force = A.force;
	moment = A.moment;
	return *this;
}

bool stk::Torser::operator==(const Torser&  A) const
{
	Torser tmp = A.TorseurEquivalent(applicationPoint);
	return ((applicationPoint == tmp.applicationPoint) && (force == tmp.force) && (moment == tmp.moment));
}

void stk::ApplyTrans(const Transform& M, Torser& T)
{
	T.applicationPoint = M.apply(T.applicationPoint);
	T.force = M.applyRotation(T.force);
	T.moment = M.applyRotation(T.moment);
}
