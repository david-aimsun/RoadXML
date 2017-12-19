#include "stk/Transform2D.h"

#include <math.h>

stk::Transform2D::Transform2D() : stk::Matrix(3,3)
{
	localToGlobal( stk::Vector2(0,0), stk::Vector2(1,1), 0 );
}

stk::Transform2D::Transform2D(const stk::Vector2& translation, double rotation)  : stk::Matrix(3,3)
{
	localToGlobal( translation, stk::Vector2(1,1), rotation );
}

void stk::Transform2D::localToGlobal(const stk::Vector2& translation, const stk::Vector2& scaling, double rotation)
{
	const double cosR=cos(rotation);
	const double sinR=sin(rotation);

	// first Scale, then Rotate, then Translate => T*R*S
	data[0*3+0]=cosR*scaling.x;	data[0*3+1]=-sinR*scaling.y;	data[0*3+2]=translation.x;
	data[1*3+0]=sinR*scaling.x;	data[1*3+1]=cosR*scaling.y;		data[1*3+2]=translation.y;
	data[2*3+0]=0;							data[2*3+1]=0;								data[2*3+2]=1;
}

void stk::Transform2D::globalToLocal(const stk::Vector2& translation, const stk::Vector2& scaling, double rotation)
{
	const double cosR=cos(rotation);
	const double sinR=sin(rotation);

	stk::Matrix T(3,3);
	InitIdentity( T );
	T(0,2) = -translation.x;
	T(1,2) = -translation.y;

	stk::Matrix R(3,3);
	InitIdentity( R );
	R(0,0)=cosR;	R(0,1)=sinR;
	R(1,0)=-sinR;	R(1,1)=cosR;

	stk::Matrix S(3,3);
	InitIdentity( S );
	if (scaling.x!=0)
		S(0,0)=1/scaling.x;
	if (scaling.x!=0)
		S(1,1)=1/scaling.y;

	// Invert transform: first -Translate, then -Rotate, then invert Scale,  => S*R*T
	stk::Matrix::operator=(S*R*T);
}

stk::Vector2 stk::Transform2D::apply(const stk::Vector2 &v) const
{
	stk::Vector2 result;
	result.x = v.x*data[0*3+0] + v.y*data[0*3+1] + data[0*3+2];
	result.y = v.x*data[1*3+0] + v.y*data[1*3+1] + data[1*3+2];

	return result;
}
