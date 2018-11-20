
#include "stk/Math.h"
#include "stk/Macro.h"
#include "stk/Transform.h"
#include <float.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace stk;

long stk::lround(double x)
{
	return ((x)>=0?(long)((x)+0.5):(long)((x)-0.5));
}

Vector3 stk::barycenter(double M1, const Vector3& Pt1, double M2, const Vector3& Pt2)
{
	if (M1+M2 == 0)
		return (Pt1+Pt2)/2;
	else
		return (Pt1 * M1 + Pt2 * M2) / (M1 + M2);
}

Vector2 stk::barycenter(double M1, const Vector2& Pt1, double M2, const Vector2& Pt2)
{
	if (M1+M2 == 0)
		return (Pt1+Pt2)/2;
	else
		return (Pt1 * M1 + Pt2 * M2) / (M1 + M2);
}

double stk::barycenter(double m1, double x1, double m2, double x2)
{
	if (m1+m2 == 0)
		return (x1+x2)/2;
	else
		return (x1 * m1 + x2 * m2) / (m1 + m2);
}


Mark stk::intlin(const Mark &P1, const Mark &P2, double x1, double x2, double x)
{
	Mark ret;
	ret.x = intlin(P1.x,P2.x,x1,x2,x);
	ret.y = intlin(P1.y,P2.y,x1,x2,x);
	ret.z = intlin(P1.z,P2.z,x1,x2,x);
	ret.roll = intlinAngle(P1.roll,P2.roll,x1,x2,x);
	ret.pitch = intlinAngle(P1.pitch,P2.pitch,x1,x2,x);
	ret.heading = intlinAngle(P1.heading,P2.heading,x1,x2,x);
	return ret;
}

double stk::square(double value)
{
	return value*value;
}


/*double stk::norme(double x, double y, double z)
{
	double carre_norme = x*x + y*y + z*z;

	return sqrt(carre_norme);
}*/

double stk::modulo(double Angle, double Max)
{
	//Ressort Angle entre 0 et 2*pi
    
	//if (AngleTourelle > PI)
	//	AngleTourelle = AngleTourelle - (2*PI) * int((PI + AngleTourelle)/(2*PI));
	//if (AngleTourelle < -PI)
	//	AngleTourelle = AngleTourelle + (2*PI) * int((PI - AngleTourelle)/(2*PI));

	//if ((Angle >  1.0e300) || (Angle < -1.0e300))
	//	return 0;

	//if (Angle>0)
	//	return Angle - Max * int(Angle/Max);
	//else
	//	return Angle + Max * int(-Angle/Max + 1);

	//while (Angle < 0.0)
	//	Angle += PI*2;
		
	//while (Angle > PI*2)
	//	Angle -= PI*2;

	double Rest = fmod(Angle,Max);
	return (Rest>=0) ? Rest : Rest+Max;
}

double stk::safeACos(double x)
{
	if (x>=1.0)
		return 0;
	if (x<=-1.0)
		return PI;
	return acos(x);
}


#define SWAP(TYPE,A,B) {TYPE C=A;A=B;B=C;}

double stk::intlinAngle(double y1, double y2, double x1, double x2, double x)
{
	if (y1>y2)
	{
	//probleme avec swap<double> et VC8
		//swap<double>(y1,y2);
		//swap<double>(x1,x2);
		SWAP(double,y1,y2);
		SWAP(double,x1,x2);
	}

	if ((y2-y1)>PI)
		y1 += 2*PI;

	return intlin(y1, y2, x1, x2, x);
}

double stk::limitModulo(double min, double x, double max)
{
	double ecart = max-min;
	double tmp = x-min;
	if (x>=0)
		tmp = tmp-ecart*int(tmp/ecart);
	else
		tmp = tmp+ecart*int(1-tmp/ecart);
	return tmp+min;
}

//Calcule l'intersection du segment Pt1Pt2 et du plan (Normale,D)
bool stk::intersectionSegmentPlan(const Vector3& Pt1, const Vector3& Pt2, const Vector3& Normale, double D, Vector3 *Sortie)
{
	double Denominateur = Normale * (Pt2 - Pt1);
	if (fabs(Denominateur) < 1.0e-9)
		return false;
	
	double mu = - (D + (Normale * Pt1)) / Denominateur;
	if ((mu < 0) || (mu > 1))
		return false;
	
	*Sortie = Pt1 + mu *(Pt2 - Pt1);
	return true;
}

//Calcule l'intersection du segment Pt1Pt2 et du plan (Normale,D)
bool stk::intersectionDroitePlan(const Vector3& Pt1, const Vector3& Pt2, const Vector3& Normale, double D, Vector3 *Sortie)
{
	double Denominateur = Normale * (Pt2 - Pt1);
	if (fabs(Denominateur) < 1.0e-9)
		return false;
	
	double mu = - (D + (Normale * Pt1)) / Denominateur;
	
	*Sortie = Pt1 + mu *(Pt2 - Pt1);
	return true;
}

bool stk::intersectLinePlane(const Vector3& P0, // first point of the segment
						   const Vector3& P1, // second point of the segment
						   const Vector3& V0, // point on the plane
						   const Vector3& normal, // normal to the plane
						   Vector3& result,
						   bool LimitToSegment)
{
	const Vector3 u = (P1-P0);
	
	// Check first if the line is parallel to the plane
	double scalar = normal*u;
	if(scalar == 0) 
		return false; // No intersection
	
	double x=((normal*(V0-P0))/(scalar));
	
	if(LimitToSegment && (x<0 || x>1))	//on ne daube pas result s'il est en dehors du segment
		return false;

	result = P0 + x*u;
	return true;
}



double stk::distancePointPlan(const Vector3& P, const Vector3& V0, const Vector3& Normal)
{
	return fabs(distancePointPlanSigne(P, V0, Normal));
}

double stk::distancePointPlanSigne(const Vector3& P, const Vector3& V0, const Vector3& Normal)
{
	double D = - Normal.x*V0.x - Normal.y*V0.y - Normal.z*V0.z;
	return Normal.x*P.x + Normal.y*P.y + Normal.z*P.z + D/Normal.getNorm();
}

double stk::distancePointLine(const Vector3& P, const Vector3& V0, const Vector3& Normal)
{
//BUG code à verifier
	return (Normal^(P-V0)).getNorm()/Normal.getNorm();
}

Vector3 stk::apply(const Mark& ref, const Vector3& p)
{
	Transform M;
	M.localToGlobal(ref);
	return M.apply(p);
}

Mark stk::convertFromAtUp(const Vector3& From, const Vector3& At, const Vector3& Up)
{
	Vector3 X = At-From;
	X /= X.getNorm();
	Vector3 Y = Up^X;
	Y /= Y.getNorm();
	Vector3 Z = X^Y;
	return toMark(From, X, Y, Z);
}

Mark stk::toMark(const Vector3& O, const Vector3& X, const Vector3& Y, const Vector3& Z)
{
	Transform M;
	M(0,0)=X.x; M(1,0)=X.y; M(2,0)=X.z; M(3,0)=0;
	M(0,1)=Y.x; M(1,1)=Y.y; M(2,1)=Y.z; M(3,1)=0;
	M(0,2)=Z.x; M(1,2)=Z.y; M(2,2)=Z.z; M(3,2)=0;
	M(0,3)=O.x; M(1,3)=O.y; M(2,3)=O.z; M(3,3)=1;
	return M.toMark();
}

void stk::decompose(const Mark& Rep, Vector3& X, Vector3& Y, Vector3& Z, Vector3& O)
{
	Transform M;
	M.localToGlobal(Rep);
	X = Vector3(M(0,0), M(1,0), M(2,0));
	Y = Vector3(M(0,1), M(1,1), M(2,1));
	Z = Vector3(M(0,2), M(1,2), M(2,2));
	O = Vector3(M(0,3), M(1,3), M(2,3));
}

//retourne "B est compris entre A et C"
bool stk::contains(double A, double B, double C)
{
	return ((B<C) && (B>=A)) || ((B>=C) && (B<A));
}

bool stk::contains2(double A, double B, double C)
{
	return ((B<=C) && (B>=A)) || ((B>=C) && (B<=A));
}

//-------------------------------------------------------------------------------------------------
bool stk::overlaps(double a1, double a2, double b1, double b2, double&r1, double&r2)
{
	if(contains(a1, b1, a2))
	{
		r1 = b1;
		if(contains(a1, b2, a2))
			r2 = b2;
		else
			r2 = a2;
		return true;
	}
	if(contains(b1, a1, b2))
	{
		r1 = a1;
		if(contains(b1, a2, b2))
			r2 = a2;
		else
			r2 = b2;
		return true;
	}
	return false;
}

//-------------------------------------------------------------------------------------------------
bool stk::overlaps(double a1, double a2, double b1, double b2)
{
	if(b1 > a2	&& b2 > a2)
		return false;
	if(b1 < a1 && b2 < a2)
		return false;
	return true;
}

double stk::angleOnCircle(const stk::Vector2& center, double radius, const stk::Vector2&  begin,const stk::Vector2& point)
{
	// center to first point vector
	stk::Vector2 vBegin(begin);
	vBegin -= center;
	// center to founded point vector
	stk::Vector2 vFound(point);
	vFound -= center;
	double alpha = acos((vBegin*vFound)/(radius*radius));
	if((vBegin^vFound) < 0.)
		alpha = -alpha;
	return alpha;
}

bool stk::intersectSegment(const Vector2& A1, const Vector2& A2, const Vector2& B1, const Vector2& B2, Vector2& Intersect)
{
	stk::Vector2 DP = B1 - A1;
	stk::Vector2 QA = A2 - A1;
	stk::Vector2 QB = B2 - B1;
	double d  =  QB^QA;
	double la = (QB^DP) / d;
	double lb = (QA^DP) / d;

	if( 0 <= la && la <= 1 &&
		0 <= lb && lb <= 1 )
	{	// Intersection exists
	   Intersect = A1 + la * QA;
	   return true;
	}

	return false;
}

bool stk::intersectSegment(const Vector2& A1, const Vector2& A2, const Vector2& B1, const Vector2& B2, Vector2& Intersect, double& parameterA, double& parameterB)
{
	stk::Vector2 DP = B1 - A1;
	stk::Vector2 QA = A2 - A1;
	stk::Vector2 QB = B2 - B1;
	double d  =  QB^QA;
	double la = (QB^DP) / d;
	double lb = (QA^DP) / d;

	if( 0 <= la && la <= 1 &&
		0 <= lb && lb <= 1 )
	{	// Intersection exists
	   Intersect = A1 + la * QA;
	   parameterA = la;
	   parameterB = lb;
	   return true;
	}

	return false;
}


/*------------------------------------------------------------*/
// Retrun true if the float value is zero at with EPSILON precision
#define iszero(x) (fabs(x) < EPSILON)
bool stk::intersectLine(const Vector2& A1, const Vector2& A2, const Vector2& B1, const Vector2& B2, Vector2& Intersect)
{
	double a = (B1.x - A1.x) * (A2.y - A1.y) -
	  (B1.y - A1.y) * (A2.x - A1.x);
	double b = (B2.x - A1.x) * (A2.y - A1.y) -
	  (B2.y - A1.y) * (A2.x - A1.x);

	double c = (A1.x - B1.x) * (B2.y - B1.y) -
	  (A1.y - B1.y) * (B2.x - B1.x);

	// compute the cross product of the 2 segment
	double det = a - b;
	if(iszero(det)) 
		return false; // The segments are colinear.  Determining colinear intersection parameters would be tedious and not instructive.

	double r = -c/det;
	Intersect.x = A1.x + r * (A2.x - A1.x);
	Intersect.y = A1.y + r * (A2.y - A1.y);
	return true;
}

int stk::intersectCircleLine(const stk::Vector2& center, double radius, const stk::Vector2& begin, const stk::Vector2& end, stk::Vector2 points[2], double parameters[2], bool limitToSegment)
{
	unsigned int solutionNb = 0;
	stk::Vector2 segment(end);
	segment -= begin;
	double a = segment.x * segment.x + segment.y * segment.y;
	double b = 2. * (segment.x * (begin.x - center.x) + segment.y * (begin.y - center.y));
	double c = begin.x*begin.x + begin.y*begin.y + center.x*center.x + center.y*center.y -2. * (begin.x * center.x + begin.y*center.y) -radius*radius;
	double delta = b*b - 4.* a*c;
	if(delta < 0)
		return 0;
	delta = sqrt(delta);
	if(fabs(delta) < EPSILON)
	{
		double u = -b / (2.*a);
		if(!limitToSegment || (u > 0 && u < 1))
		{
			points[solutionNb] = segment;
			points[solutionNb] *= u;
			points[solutionNb] += begin;
			parameters[solutionNb] = u;
			++solutionNb;
		}
	}
	else
	{
		// generic case : 2 solutions
		double u = (-b + delta)/(2.*a);

		if(!limitToSegment || (u > 0 && u < 1))
		{
			points[solutionNb] = segment;
			points[solutionNb] *= u;
			points[solutionNb] += begin;
			parameters[solutionNb] = u;
			++solutionNb;
		}	 
		u = (-b - delta)/(2.*a);
		if(!limitToSegment || (u > 0 && u < 1))
		{
			points[solutionNb] = segment;
			points[solutionNb] *= u;
			points[solutionNb] += begin;
			parameters[solutionNb] = u;
			++solutionNb;
		}
	}
	return solutionNb;
}

int stk::intersectCircleSegment(const stk::Vector2& center, double radius, const stk::Vector2& begin, const stk::Vector2& end, stk::Vector2 points[2], double parameters[2])
{
	return intersectCircleLine(center, radius, begin, end, points, parameters, true);
}

int stk::intersectCircle(const stk::Vector2& center1, double radius1,
								 const stk::Vector2& center2, double radius2,
								 stk::Vector2 point[2])
{
	// test same circle
	if(((fabs(center1.x - center2.x) < EPSILON) && (fabs(center1.y - center2.y) < EPSILON))
		&&
		(fabs(radius1 - radius2) < EPSILON))
	{
		return 0;
	}
	// aX² + bX + c = 0 equation
	double n = (radius2*radius2 - radius1*radius1 - center2.x*center2.x + center1.x*center1.x - center2.y*center2.y + center1.y*center1.y) / (2.*(center1.y - center2.y));
	double p = (center1.x - center2.x)/(center1.y - center2.y);
	double a = p*p + 1;
	double b = 2.*(center1.y*p - n*p - center1.x);
	double c = center1.x*center1.x + center1.y*center1.y + n*n - radius1*radius1 - 2.*center1.y*n;

	double delta = b*b - 4*a*c;
	if(delta < 0.)
		return 0;
	delta = sqrt(delta);
	if( fabs(delta) < EPSILON)
	{
		double x =(-b)/(2.*a);
		double y = n - x*p;
		point[0].x = x;
		point[0].y = y;
		return 1;
	}

	double x =(-b+delta)/(2.*a);
	double y = n - x*p;
	point[0].x = x;
	point[0].y = y;
	x =(-b-delta)/(2.*a);
	y = n - x*p;
	point[1].x = x;
	point[1].y = y;
	return 2;
}

Vector3 stk::normalePlan(const Vector3& Pt1, const Vector3& Pt2, const Vector3& Pt3, double* D)
{
	stk::Vector3 Normale = (Pt2 - Pt1)^(Pt3 - Pt1);
	if (D!=NULL)
		*D = -(Normale * Pt1);
	return Normale;
}


//Quaternions et MatricesTransfo

void stk::quaternionToMatriceTransfo(const Quaternion& q, const stk::Vector3& d, stk::Transform& mt)
{
	stk::Matrix M(4,4);

	M(0,0) = 1 - 2*q.v.y*q.v.y - 2*q.v.z*q.v.z;	
	M(1,0) = 2*q.v.x*q.v.y + 2*q.s*q.v.z;		
	M(2,0) = 2*q.v.x*q.v.z - 2*q.s*q.v.y;
	
	M(0,1) = 2*q.v.x*q.v.y - 2*q.s*q.v.z;		
	M(1,1) = 1 - 2*q.v.x*q.v.x - 2*q.v.z*q.v.z;	
	M(2,1) = 2*q.v.y*q.v.z + 2*q.s*q.v.x;
	
	M(0,2) = 2*q.v.x*q.v.z + 2*q.s*q.v.y;
	M(1,2) = 2*q.v.y*q.v.z - 2*q.s*q.v.x;
	M(2,2) = 1 - 2*q.v.x*q.v.x - 2*q.v.y*q.v.y;	
	
	M(0,3) = d.x; 
	M(1,3) = d.y; 
	M(2,3) = d.z;
	M(3,3) = 1;
	
	mt = stk::Transform(M);	
}



void stk::matriceTransfoToQuaternion(const stk::Transform& m, Quaternion& q)
{
	double tr, s;

	tr = m(0,0) + m(1,1) + m(2,2);

	if(tr >= 0)
	{
		s = sqrt(tr + 1);
		q.s = 0.5 * s;
		s = 0.5 / s;
		q.v.x = (m(2,1) - m(1,2)) * s;
		q.v.y = (m(0,2) - m(2,0)) * s;
		q.v.z = (m(1,0) - m(0,1)) * s;
	}
	else
	{
		int i = 0;
		if(m(1,1) > m(0,0))
			i = 1;
		if(m(2,2) > m(i,i))
			i = 2;

		switch (i)
		{
		case 0:
			s = sqrt((m(0,0) - (m(1,1) + m(2,2))) + 1);
			q.v.x = 0.5 * s;
			s = 0.5 / s;
			q.v.y = (m(0,1) + m(1,0)) * s;
			q.v.z = (m(2,0) + m(0,2)) * s;
			q.s = (m(2,1) - m(1,2)) * s;
			break;

		case 1:
			s = sqrt((m(1,1) - (m(2,2) + m(0,0))) + 1);
			q.v.y = 0.5 * s;
			s = 0.5 / s;
			q.v.z = (m(1,2) + m(2,1)) * s;
			q.v.x = (m(0,1) + m(1,0)) * s;
			q.s = (m(0,2) - m(2,0)) * s;
			break;

		case 2:
			s = sqrt((m(2,2) - (m(0,0) + m(1,1))) + 1);
			q.v.z = 0.5 * s;
			s = 0.5 / s;
			q.v.x = (m(2,0) + m(0,2)) * s;
			q.v.y = (m(1,2) + m(2,1)) * s;
			q.s = (m(1,0) - m(0,1)) * s;
			break;
		}
	}
}


bool stk::isEqual( const double value1, const double value2, const double epsilon )
{
	return (fabs(value1-value2)<epsilon)?true:false;
}

///-------------------------------------------------------------------------------------------------
 void stk::rotate(Vector2&v, double angle)
{
	double c = cos(angle);
	double s = sin(angle);
	double x2 = v.x*c - v.y*s;
	double y2 = v.x*s + v.y*c;
	v.x = x2;
	v.y = y2;
}

bool stk::isOnRightOfLane (const Vector2& point, const Vector2& A, const Vector2& B)
{
	double t= ((B.x-A.x)*(point.y-A.y)) - ((point.x-A.x)*(B.y-A.y)); //det
	return( t>0.0? false: true);
}

//-------------------------------------------------------------------------------------------------
void stk::projectOnCircle(const Vector2&point, const Vector2&center, double radius, Vector2&result)
{
	result = point-center;
	result.normalize();
	result *= radius;
	result += center;
}

//-------------------------------------------------------------------------------------------------
bool stk::projectOnSegment(const Vector2&point, const Vector2&A1, const Vector2&A2, Vector2&result, double& dist)
{
 Vector2 segment = A2-A1;

	// left of segment ending
 Vector2 tmp = point-A2;
	if(tmp*segment > 0)
		return false;
	
	tmp = point-A1;
	// right of segment begining
	if(tmp*segment < 0)
		return false;

	result = segment;
	result.normalize();
	dist = tmp*result;
	result *= dist;
	result += A1;

	return true;
}

////x est compris entre 0.0 et 1.0
double stk::Signale(stk::t_Signale type, double x)
{
	if (x<0)
		return 0;
	if (x>1)
		return 1;

	switch (type)
	{
	case TTrapeze:
		return x;
	case TEnS:
		//return (-2 * x*x*x + 3 * x*x);
	//this new formula has the following properties:
	//f(0)=0 f(1)=1 f'(0)=0 f'(1)=1 f"(0)=0 f"(1)=1
	//it can be used in cases where it is important to minimize the "jerk"
		return -(sin(2*M_PI*x)/(2*M_PI)) + x;
	case TExponentiel:
		return expm1(x) / expm1(1.0);
	}

	return 0;
}

double stk::Signale(stk::t_Signale type, double y1, double y2, double x1, double x2, double x)
{
	if ((x2 - x1) == 0)
		return y2;
	return y1 + (y2 - y1)*Signale(type, (x - x1) / (x2 - x1));
}


////-----------------------------------------------------------------------------------------------------------------
////	Naive FFT Implementation : to be refined
////-----------------------------------------------------------------------------------------------------------------
void stk::fastFourierTransform(const TComplexSequence &sequence, TComplexSequence &result, bool inverse)
{
	size_t sequenceSize = sequence.size();
	double dSequenceSize = static_cast<double>(sequenceSize);
	result.resize(sequenceSize);
	for (size_t i = 0; i < sequenceSize; ++i)
	{
		result[i].first = 0;
		result[i].second = 0;
		for (size_t j = 0; j < sequenceSize; ++j)
		{
			double angle = -(inverse ? -2. : 2.) * M_PI * static_cast<double>(i)* static_cast<double>(j) / dSequenceSize;
			double cosAngle = cos(angle);
			double sinAngle = sin(angle);
			result[i].first += sequence[j].first * cosAngle - sequence[j].second * sinAngle;
			result[i].second += sequence[j].first * sinAngle + sequence[j].second * cosAngle;
		}
		if (inverse)
		{
			result[i].first /= dSequenceSize;
			result[i].second /= dSequenceSize;
		}
	}
}

////-----------------------------------------------------------------------------------------------------------------
////	FFT code taken from https://www.programming-techniques.com/2013/05/calculation-of-discrete-fourier.html
////-----------------------------------------------------------------------------------------------------------------
//void stk::fastFourierTransform(const TComplexSequence &sequence, TComplexSequence &result, bool inverse)
//{
//	size_t sequenceSize = sequence.size();
//	size_t n1 = 1;
//	size_t n2 = n1;
//	size_t log2Size = log2(sequence.size());
//
//	result = sequence;		//	Copy the sequence
//	for (size_t k = 0; k < log2Size; ++k)
//	{
//		n2 = n1;
//		n1 <<= 1;
//
//		double angle = (inverse ? (M_PI * 2.0) : (-M_PI * 2.0)) / static_cast<double>(n1);
//		double wtmp = sin(0.5 * angle);
//		std::pair<double, double> wp(-2.0*wtmp*wtmp, sin(angle));
//		std::pair<double, double> w(1.0, 0.0);
//
//		for (size_t m = 0; m < n2; ++m)
//		{
//			for (size_t i = m; i < sequenceSize; i += n1)
//			{
//				size_t j = i + n2;
//				std::pair<double, double> temp(
//					(w.first * result[j].first) - (w.second * result[j].second),
//					(w.first * result[j].second) + (w.second * result[j].first));
//
//				//	result[j] = result[i] - temp;
//				result[j].first = result[i].first - temp.first;
//				result[j].second = result[i].second - temp.second;
//
//				//	result[i] += temp;
//				result[i].first += temp.first;
//				result[i].second += temp.second;
//			}
//			//	w += w*wp;
//			wtmp = w.first;
//			w.first += (w.first * wp.first) - (w.second * wp.second);
//			w.second += (w.second * wp.first) + (wtmp * wp.second);
//		}
//	}
//	if (inverse)
//	{
//		double scale = 1.0 / static_cast<double>(sequenceSize);
//		for (auto &c : result)
//		{
//			c.first *= scale;
//			c.second *= scale;
//		}
//	}
//}
