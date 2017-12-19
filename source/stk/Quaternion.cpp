
#include "stk/Quaternion.h"
#include "stk/Math.h"
#include <math.h>

using namespace stk;

stk::Quaternion::Quaternion():s(0)
{}

stk::Quaternion::Quaternion(double s, Vector3 v) : s(s),v(v)
{}

bool stk::Quaternion::operator== (const Quaternion &Q) const
{
	return (s==Q.s) && (v==Q.v);
}

bool stk::Quaternion::operator!= (const Quaternion &Q) const
{
	return !(*this == Q);
}

Quaternion stk::Quaternion::operator* (const Quaternion&Q) const
{
	return Quaternion(s*Q.s - v*Q.v, s*Q.v + Q.s*v + (v^Q.v));
}

Quaternion stk::Quaternion::operator* (double e) const
{
	return Quaternion(s*e, v*e);
}

Quaternion stk::Quaternion::operator/ (double e) const
{
	return Quaternion(s/e,v/e);
}

Quaternion& stk::Quaternion::operator*= (double e)
{
	s*=e; v*=e; return *this;
}

Quaternion& stk::Quaternion::operator/= (double e)
{
	s/=e; v/=e; return *this;
}

Quaternion stk::Quaternion::operator+ (const Quaternion&Q) const
{
	return Quaternion(s+Q.s, v+Q.v);
}

Quaternion stk::Quaternion::operator- (const Quaternion& Q) const
{
	return Quaternion(s-Q.s, v-Q.v);
}

Quaternion& stk::Quaternion::operator+= (const Quaternion&Q)
{
	s+=Q.s; v+=Q.v; return *this;
}

Quaternion& stk::Quaternion::operator-= (const Quaternion&Q)
{
	s-=Q.s; v-=Q.v; return *this;
}

//divise le vecteur par sa norme...	
void stk::Quaternion::normalize()
{
	if(getNorm()>0)
		(*this) /= getNorm();
}

double stk::Quaternion::getNorm() const
{
	return sqrt(s*s+v.x*v.x+v.y*v.y+v.z*v.z);
}

// CLASS QUATERNIONS
void stk::EulerAnglesToQuaternion(double Roulis, double Tangage, double Lacet, Quaternion& q)
{
	q.s = cos(Lacet/2) * cos(Tangage/2) * cos(Roulis/2)                     
	   + sin(Lacet/2) * sin(Tangage/2) * sin(Roulis/2);

	q.v.x = cos(Lacet/2) * cos(Tangage/2) * sin(Roulis/2)
	   - sin(Lacet/2) * sin(Tangage/2) * cos(Roulis/2);

	q.v.y = cos(Lacet/2) * sin(Tangage/2) * cos(Roulis/2)
	   + sin(Lacet/2) * cos(Tangage/2) * sin(Roulis/2);

	q.v.z = sin(Lacet/2) * cos(Tangage/2) * cos(Roulis/2)
	   - cos(Lacet/2) * sin(Tangage/2) * sin(Roulis/2);
}

void stk::VectorRotationToQuaternion(stk::Vector3 v, double rot, Quaternion& q)
{
	double s2 = sin(rot/2);

	q.s = cos(rot/2);

	q.v.x = v.x * s2;
	q.v.y = v.y * s2;
	q.v.z = v.z * s2;
}


void stk::QuaternionToEulerAngles(const Quaternion& q, double& Roulis, double& Tangage, double& Lacet)
{
	Roulis  = atan2( 2 * (q.s * q.v.x + q.v.y * q.v.z), (square(q.s) - square(q.v.x) - square(q.v.y) + square(q.v.z)));
	Tangage = asin (-2 * (q.v.x * q.v.z - q.s * q.v.y));                                
	Lacet   = atan2( 2 * (q.v.x * q.v.y + q.s * q.v.z), (square(q.s) + square(q.v.x) - square(q.v.y) - square(q.v.z)));      
}

Vector3 stk::ApplyRotation(const Quaternion& q, Vector3 Vin)
{
	Vector3 Vout = (2 * square(q.s) - 1) * Vin
					+ 2 * (q.v * Vin) * q.v
					+ 2 * q.s * (q.v^Vin);

	return Vout;
}

void stk::NormaliseCoordQuaternion(double& S, double& Vx, double& Vy, double& Vz)
{
	Quaternion Q(S, Vector3(Vx, Vy, Vz));
	Q.normalize();
	S = Q.s;
	Vx = Q.v.x;
	Vy = Q.v.y;
	Vz = Q.v.z;
}

