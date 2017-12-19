
#include "stk/Vector2.h"
#include "stk/Macro.h"

#include <assert.h>
#include <math.h>

using namespace stk;

Vector2 Center(const Vector2& v0, const Vector2& v1)
{
	return (v0 + v1) / 2.0;
}

bool Vector2::setNorm(double newMagnitude)
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

bool Vector2::isEqual(
	const Vector2& 	compareVec,
	const double		 	zeroTolerance) const
{
	// this routine needs to take another vector
	if (zeroTolerance == 0.0)
	{
		return (x == compareVec.x && y == compareVec.y);
	}
	else
	{
		assert(zeroTolerance > 0.0);
		return (IS_CLOSE(x, compareVec.x, zeroTolerance) &&
			IS_CLOSE(y, compareVec.y, zeroTolerance));
	}
}


Vector2 stk::toVector2(const Vector3& pos)
{
	return Vector2( pos.x, pos.y);
}

double stk::angleBetweenVectors(const Vector2& v1, const Vector2& v2)
{
	Vector2 vector1 = v1;
	if (!vector1.normalize())
		return 0;

	Vector2 vector2 = v2;
	if (!vector2.normalize())
		return 0;

	const double cosTheta = vector1 * vector2;
	const double sinTheta = vector1 ^ vector2;
	return atan2(sinTheta, cosTheta);
}

double stk::angleBetweenNormalizedVectors(const Vector2& v1, const Vector2& v2)
{
	const double cosTheta = v1 * v2;
	const double sinTheta = v1 ^ v2;
	return atan2(sinTheta, cosTheta);
}


Vector2 stk::operator*(const double	s, const Vector2& v)
{
	return Vector2(v) *= s;
}



Vector2 operator-	(const Vector2& v)
{
	return Vector2(-v.x, -v.y);
}

// [inline implementation] -------------------------------------------------------------//

Vector2::Vector2(void)
: x(0.0), y(0.0)
{ }

Vector2::Vector2(const double xx, const double yy)
: x(xx), y(yy)
{ }

double Vector2::getNorm() const
{
	return sqrt(x*x + y*y);
}

double Vector2::getNormSquared() const
{
	return x*x + y*y;
}

double Vector2::getDistance(const Vector2& rhs) const
{
	return sqrt((x - rhs.x)*(x - rhs.x) + (y - rhs.y)*(y - rhs.y));
}

Vector2& Vector2::operator/=(const double scalar)
{
	const double invScalar = 1.0f / scalar;
	x *= invScalar; y *= invScalar;
	return *this;
}

Vector2& Vector2::operator*=			(const double scalar)
{
	x *= scalar; y *= scalar; return *this;
}

Vector2& Vector2::operator+=			(const Vector2& v)
{
	x += v.x; y += v.y; return *this;
}

Vector2& Vector2::operator-=			(const Vector2& v)
{
	x -= v.x; y -= v.y; return *this;
}

bool Vector2::normalize(double zeroTolerance)
{
	bool normalizationSucceeded = false;
	double	magnitude = getNorm();

	if (magnitude > zeroTolerance)
	{
		*this *= 1 / magnitude;

		normalizationSucceeded = true;
	}

	return normalizationSucceeded;
}


Vector2 ToVectord2(const Vector3& pos) { return Vector2(pos.x, pos.y); }

Vector2 Vector2::operator/(const double s) const
{
	return Vector2(*this) /= s;
}

Vector2 Vector2::operator*(const double s) const
{
	return Vector2(*this) *= s;
}

Vector2 Vector2::operator+(const Vector2& b) const
{
	return Vector2(*this) += b;
}

Vector2 Vector2::operator-(const Vector2& b) const
{
	return Vector2(*this) -= b;
}

Vector2 Vector2::operator-() const
{
	return Vector2(-x, -y);
}

bool  Vector2::operator==(const Vector2& rhs) const
{
	return ((x == rhs.x) && (y == rhs.y));
}

bool  Vector2::operator!=(const Vector2& right) const
{
	return ((x != right.x) || (y != right.y));
}

// dot product, cross product, and term by term product (a.x * b.x, a.y * b.y, ...)
double Vector2::operator*(const Vector2& b) const
{
	return x * b.x + y * b.y;
}

double Vector2::operator^	(const Vector2& b) const
{
	return x * b.y - y * b.x;
}

Vector2 Vector2::operator%(const Vector2& b) const
{
	return Vector2(x * b.x, y * b.y);
}

double Vector2::getArgument() const
{
	return (double)atan2(y, x);
}

std::ostream& stk::operator<<(std::ostream& out, const Vector2& p)
{
	return out << p.x << " " << p.y;
}
std::istream& stk::operator>>(std::istream& in, Vector2& p)
{
	return in >> p.x >> p.y;
}

