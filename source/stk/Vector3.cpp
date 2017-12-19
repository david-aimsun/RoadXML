
#include "stk/Macro.h" // for conversion ops
//#include "stk/Vector2.h" // for conversion ops
#include "stk/Vector3.h"

#include <math.h>
#include <assert.h>

using namespace stk;

bool Vector3::setNorm(
	const double 		newMagnitude)
{
	bool		success = false;
	const double mag = getNorm();

	if (mag > 0)
	{
		*this *= newMagnitude / mag;
		success = true;
	}

	return success;
}

bool Vector3::isEqual(
	const Vector3& compareVec,
	const double 		zeroTolerance) const
{
	if (zeroTolerance == 0.0)
	{
		return (x == compareVec.x && y == compareVec.y && z == compareVec.z);
	}
	else
	{
		assert(zeroTolerance > 0.0);
		return (IS_CLOSE(x, compareVec.x, zeroTolerance) &&
			IS_CLOSE(y, compareVec.y, zeroTolerance) &&
			IS_CLOSE(z, compareVec.z, zeroTolerance));
	}
}


// [operator definitions] --------------------------------------------------------------//

// standard vector operators
Vector3 Vector3::operator/(const double	s) const
{
	return Vector3(*this) /= s;
}

Vector3 stk::operator*(const double s, const Vector3& v)
{
	return Vector3(v) *= s;
}

Vector3 Vector3::operator*  (const double s) const
{
	return Vector3(*this) *= s;
}

Vector3 Vector3::operator+  (const Vector3& b) const
{
	return Vector3(*this) += b;
}

Vector3 Vector3::operator-  (const Vector3& b) const
{
	return Vector3(*this) -= b;
}

Vector3 Vector3::operator/  (const Vector3& b) const
{
	return Vector3(*this) /= b;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

bool  Vector3::operator== (const Vector3& rhs) const
{
	return ((x == rhs.x) && (y == rhs.y) && (z == rhs.z));
}

bool  Vector3::operator!= (const Vector3& rhs) const
{
	return ((x != rhs.x) || (y != rhs.y) || (z != rhs.z));
}



// dot product, cross product, and term by term product (a.x * b.x, a.y * b.y, ...)
double	    Vector3::operator*  (const Vector3& b) const
{
	return (x*b.x + y*b.y + z*b.z);
}

Vector3 Vector3::operator^  (const Vector3& b) const
{
	return Vector3(
		y * b.z - z * b.y,
		z * b.x - x * b.z,
		x * b.y - y * b.x);
}

Vector3 Vector3::operator%  (const Vector3& b) const
{
	return Vector3(x*b.x, y*b.y, z*b.z);
}




// [implementation] -------------------------------------------------------------//

Vector3::Vector3()
: x(0.0), y(0.0), z(0.0)
{ }

Vector3::Vector3(const double xx, const double yy, const double zz)
: x(xx), y(yy), z(zz)
{ }

double Vector3::getNorm() const
{
	return sqrt(x*x + y*y + z*z);;
}

double Vector3::getNormSquared() const
{
	return x*x + y*y + z*z;
}

double Vector3::getDistance(const Vector3& rhs) const
{
	return sqrt((x - rhs.x)*(x - rhs.x) + (y - rhs.y)*(y - rhs.y) + (z - rhs.z)*(z - rhs.z));
}

Vector3& Vector3::operator=(const Vector3& v)
{
	x = v.x; y = v.y; z = v.z; return *this;
}

Vector3& Vector3::operator/=(const double scalar)
{
	const double invScalar = 1.0f / scalar;
	x *= invScalar; y *= invScalar; z *= invScalar;
	return *this;
}

Vector3& Vector3::operator*=(const double scalar)
{
	x *= scalar; y *= scalar; z *= scalar; return *this;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x; y += v.y; z += v.z; return *this;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x; y -= v.y; z -= v.z; return *this;
}


Vector3& Vector3::operator/=(const Vector3& v)
{
	x /= v.x; y /= v.y; z /= v.z; return *this;
}

bool Vector3::normalize(double zeroTolerance)
{
	bool normalizationSucceeded(false);
	double	magnitude(getNorm());

	if (magnitude > zeroTolerance)
	{
		*this *= 1 / magnitude;

		normalizationSucceeded = true;
	}

	return normalizationSucceeded;
}

//found in MesaGL
void stk::Vector3::rotate(const stk::Vector3& Kin)
{
	stk::Vector3 K = Kin;
	stk::Vector3 tmp(*this);
	double angle = K.getNorm();
	if (fabs(angle) < 1e-10)
		return;
	K = K / angle;

	double s, c;

	s = sin(angle);
	c = cos(angle);

	if (angle>0)
	{
		double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

		xx = K.x * K.x;
		yy = K.y * K.y;
		zz = K.z * K.z;
		xy = K.x * K.y;
		yz = K.y * K.z;
		zx = K.z * K.x;
		xs = K.x * s;
		ys = K.y * s;
		zs = K.z * s;
		one_c = 1 - c;

		x = ((one_c * xx) + c)*tmp.x + ((one_c * xy) - zs)*tmp.y + ((one_c * zx) + ys)*tmp.z;
		y = ((one_c * xy) + zs)*tmp.x + ((one_c * yy) + c)*tmp.y + ((one_c * yz) - xs)*tmp.z;
		z = ((one_c * zx) - ys)*tmp.x + ((one_c * yz) + xs)*tmp.y + ((one_c * zz) + c)*tmp.z;
	}
}

Vector3 stk::Center(const Vector3& v0, const Vector3& v1)
{
	return (v0 + v1) / 2.0;
}

double stk::angleBetweenVectors(const Vector3& v1, const Vector3& v2)
{
	const double cosTheta = v1 * v2;
	double n1 = v1.getNorm();
	double n2 = v2.getNorm();
	double val = cosTheta / (n1*n2);
	return(acos(val));
}

double stk::angleBetweenNormalizedVectors(const Vector3& v1, const Vector3& v2)
{
	const double cosTheta = v1 * v2;
	return(acos(cosTheta));
}

std::ostream& stk::operator<<(std::ostream& out, const Vector3& p)
{
	return out << p.x << " " << p.y << " " << p.z;
}
std::istream& stk::operator>>(std::istream& in, Vector3& p)
{
	return in >> p.x >> p.y >> p.z;
}
