
#include "stk/Mark.h"

using namespace stk;

//CLASS Mark
stk::Mark::Mark() :
x(0.0), y(0.0), z(0.0), roll(0.0), pitch(0.0), heading(0.0)
{
}


stk::Mark::Mark(const Vector3& Pos, double Rx, double Ry, double Rz)
{
	x = Pos.x;
	y = Pos.y;
	z = Pos.z;
	roll = Rx;
	pitch = Ry;
	heading = Rz;
}


stk::Mark::Mark(const Vector3& Pos, const Vector3& Rot)
{
	x = Pos.x;
	y = Pos.y;
	z = Pos.z;
	roll = Rot.x;
	pitch = Rot.y;
	heading = Rot.z;
}

Vector3 stk::Mark::getOrigin() const
{
	return Vector3(x,y,z);
}

bool stk::Mark::operator== (const Mark &A) const
{
	return (x==A.x) && (y==A.y) && (z==A.z) && (roll==A.roll) && (pitch==A.pitch) && (heading==A.heading);
}

bool stk::Mark::operator!= (const Mark &A) const
{
	return !(*this == A);
}

Mark stk::Mark::operator+ (const Vector3& A) const
{
	return Mark(Vector3(x+A.x, y+A.y, z+A.z), roll, pitch, heading);
}

Mark stk::Mark::operator- (const Vector3& A) const
{
	return Mark(Vector3(x-A.x, y-A.y, z-A.z), roll, pitch, heading);
}

Mark& stk::Mark::operator+= (const Vector3& A)
{
	x+=A.x;
	y+=A.y;
	z+=A.z;
	return *this;
}

Mark& stk::Mark::operator-= (const Vector3& A)
{
	x-=A.x;
	y-=A.y;
	z-=A.z;
	return *this;
}

std::ostream &stk::operator<<(std::ostream &out, const Mark& r)
{
	return out << r.x << " " << r.y << " " << r.z << " " << r.roll << " " << r.pitch << " " << r.heading;
}
std::istream &stk::operator>>(std::istream &in, Mark &r)
{
	return in >> r.x >> r.y >> r.z >> r.roll >> r.pitch >> r.heading;
}
