#include "stk/Box3.h"
#include "stk/Macro.h"

#include <cmath>
#include <algorithm>

using namespace stk;
/*------------------------------------------------------------*/
/*Box3*/
/*------------------------------------------------------------*/

Box3::Box3()
:isEmpty(true),
myMax(0, 0, 0),
myMin(0, 0, 0)
{
}

Box3::Box3(const Vector3& min, const Vector3& max)
:isEmpty(false),
myMax(max),
myMin(min)
{
}


/*------------------------------------------------------------*/

void Box3::add(const Vector3 &p)
{
	if (isEmpty)
	{
		myMin = p;
		myMax = p;
		isEmpty = false;
	}
	else
	{
		myMin.x = CMIN(myMin.x, p.x);
		myMin.y = CMIN(myMin.y, p.y);
		myMin.z = CMIN(myMin.z, p.z);

		myMax.x = CMAX(myMax.x, p.x);
		myMax.y = CMAX(myMax.y, p.y);
		myMax.z = CMAX(myMax.z, p.z);
	}
}

/*------------------------------------------------------------*/

bool Box3::contains(const Vector3 &p) const
{
	if (isEmpty)
		return(false);

	return ( p.x >= myMin.x
	 && p.y >= myMin.y
	 && p.z >= myMin.z
	 && p.x <= myMax.x
	 && p.y <= myMax.y
	 && p.z <= myMax.z);
}

/*------------------------------------------------------------*/

bool Box3::contains(const Vector2 &p) const
{
	if (isEmpty)
		return(false);

	return ( p.x >= myMin.x
	 && p.y >= myMin.y
	 && p.x <= myMax.x
	 && p.y <= myMax.y);
}

/*------------------------------------------------------------*/

bool Box3::circleContains(const Vector2 &p) const
{
	if (isEmpty)
		return(false);

	return (p.getDistance(center()) < radius());
}

/*------------------------------------------------------------*/

double Box3::radius() const
{
	if (isEmpty)
		return(false);

	return (myMax.getDistance(center()));
}



/*------------------------------------------------------------*/

bool Box3::intersects(const Box3 &b2) const
{
	if (isEmpty || b2.isEmpty)
		return(false);

	if ( (myMax.x < b2.myMin.x)
	  || (myMin.x > b2.myMax.x) )
		return false;

	if ( (myMax.y < b2.myMin.y)
	  || (myMin.y > b2.myMax.y) )
		return false;

	if ( (myMax.z < b2.myMin.z)
	  || (myMin.z > b2.myMax.z) )
		return false;

	return true;
}

/*------------------------------------------------------------*/
Vector3 Box3::upperLeftCorner() const
{
	return myMin;
}

/*------------------------------------------------------------*/
Vector3 Box3::bottomLeftCorner() const
{
	Vector3 blc(0,0,0);

	blc.x = myMin.x;
	blc.y = myMax.y;

	return blc;
}

/*------------------------------------------------------------*/
Vector3 Box3::upperRightCorner() const
{
	Vector3 urc(0,0,0);

	urc.x = myMax.x;
	urc.y = myMin.y;

	return urc;
}

/*------------------------------------------------------------*/
Vector3 Box3::bottomRightCorner() const
{
	return myMax;
}

/*------------------------------------------------------------*/
Vector3 Box3::center() const
{
	return (myMax+myMin)*0.5;
}


/*------------------------------------------------------------*/
double	Box3::minDist(const Vector2 &pos) const
{
	double CMINX = CMIN(fabs(pos.x-myMin.x),fabs(pos.x-myMax.x));
	double CMINY = CMIN(fabs(pos.y-myMin.y),fabs(pos.y-myMax.y));

	return CMAX(CMINX,CMINY);
}

/*------------------------------------------------------------*/
double	Box3::maxDist(const Vector2 &pos) const
{
	double CMAXX = CMAX(fabs(pos.x-myMin.x),fabs(pos.x-myMax.x));
	double CMAXY = CMAX(fabs(pos.y-myMin.y),fabs(pos.y-myMax.y));

	return CMAX(CMAXX,CMAXY);
}

/*------------------------------------------------------------*/
Vector3 Box3::farestCorner(const Vector2 &pos) const
{
	Vector3 corner(0,0,0);

	double distCMINX = pos.x - myMin.x;
	double distCMINY = pos.y - myMin.y;
	double distCMAXX = pos.x - myMax.x;
	double distCMAXY = pos.y - myMax.y;

	if(fabs(distCMINX) > fabs(distCMAXX))
		corner.x = myMin.x;
	else
		corner.x = myMax.x;

	if(fabs(distCMINY) > fabs(distCMAXY))
		corner.y = myMin.y;
	else
		corner.y = myMax.y;
	
	return corner;
}

/*------------------------------------------------------------*/
Vector3 Box3::nearestCorner(const Vector2 &pos) const
{
	Vector3 corner(0,0,0);

	double distCMINX = pos.x - myMin.x;
	double distCMINY = pos.y - myMin.y;
	double distCMAXX = pos.x - myMax.x;
	double distCMAXY = pos.y - myMax.y;

	if(fabs(distCMINX) < fabs(distCMAXX))
		corner.x = myMin.x;
	else
		corner.x = myMax.x;

	if(fabs(distCMINY) < fabs(distCMAXY))
		corner.y = myMin.y;
	else
		corner.y = myMax.y;
	
	return corner;
}

/*------------------------------------------------------------*/
double  Box3::centerDist(const Vector2 &pos) const
// distance (pos, center of bounding circle)
{
	return (pos.getDistance(center()));
}

/*------------------------------------------------------------*/
double  Box3::minCircleDist(const Vector2 &pos) const
// CMIN positive distance (pos, bounding circle)
{
	return fabs(centerDist(pos)-radius());
}

/*------------------------------------------------------------*/
double  Box3::maxCircleDist(const Vector2 &pos) const
// CMAX positive distance (pos, bounding circle)
{
	return(centerDist(pos)+radius());
}

void Box3::retrieveVertexList(std::vector<stk::Vector3>& vertexList) const
{
	vertexList.resize(8);

	if( !isValid() )
		return;
	
	// x = xmin
	vertexList[0] = stk::Vector3( myMin.x, myMin.y, myMin.z );
	vertexList[1] = stk::Vector3( myMin.x, myMin.y, myMax.z );
	vertexList[2] = stk::Vector3( myMin.x, myMax.y, myMin.z );
	vertexList[3] = stk::Vector3( myMin.x, myMax.y, myMax.z );
	
	// x = xmax
	vertexList[4] = stk::Vector3( myMax.x, myMin.y, myMin.z );
	vertexList[5] = stk::Vector3( myMax.x, myMin.y, myMax.z );
	vertexList[6] = stk::Vector3( myMax.x, myMax.y, myMin.z );
	vertexList[7] = stk::Vector3( myMax.x, myMax.y, myMax.z );
}

void Box3::retrieveFaceCenterList(std::vector<stk::Vector3>& vertexList) const
{
	vertexList.resize(6);

	if( !isValid() )
		return;
	
	stk::Vector3 centerPoint = center();

	for ( size_t cnt = 0; cnt < 6; ++cnt )
		vertexList[cnt] = centerPoint;
		
	vertexList[0].x = myMin.x;
	vertexList[1].x = myMax.x;
	vertexList[2].y = myMin.y;
	vertexList[3].y = myMax.y;
	vertexList[4].z = myMin.z;
	vertexList[5].z = myMax.z;
}

void Box3::setToIntersection( const Box3& lhs, const Box3& rhs )
{
	if( !lhs.isValid() && !rhs.isValid() )
		return;

	if( !lhs.isValid() )
	{
		*this = rhs;
		return;
	}

	if( !rhs.isValid() )
	{
		*this = lhs;
		return;
	}

	// subtle change in comparison from union
	myMin.x = (lhs.myMin.x > rhs.myMin.x) ? lhs.myMin.x : rhs.myMin.x;
	myMax.x = (lhs.myMax.x < rhs.myMax.x) ? lhs.myMax.x : rhs.myMax.x;
	
	myMin.y = (lhs.myMin.y > rhs.myMin.y) ? lhs.myMin.y : rhs.myMin.y;
	myMax.y = (lhs.myMax.y < rhs.myMax.y) ? lhs.myMax.y : rhs.myMax.y;
	
	myMin.z = (lhs.myMin.z > rhs.myMin.z) ? lhs.myMin.z : rhs.myMin.z;
	myMax.z = (lhs.myMax.z < rhs.myMax.z) ? lhs.myMax.z : rhs.myMax.z;

	checkValidity();
}

void Box3::setToUnion(const Box3& lhs, const Box3& rhs )
{
	if( !lhs.isValid() && !rhs.isValid() )
		return;

	if( !lhs.isValid() )
	{
		*this = rhs;
		return;
	}

	if( !rhs.isValid() )
	{
		*this = lhs;
		return;
	}

	// subtle change in comparison from intersection
	myMin.x = (lhs.myMin.x < rhs.myMin.x) ? lhs.myMin.x : rhs.myMin.x;
	myMax.x = (lhs.myMax.x > rhs.myMax.x) ? lhs.myMax.x : rhs.myMax.x;
	
	myMin.y = (lhs.myMin.y < rhs.myMin.y) ? lhs.myMin.y : rhs.myMin.y;
	myMax.y = (lhs.myMax.y > rhs.myMax.y) ? lhs.myMax.y : rhs.myMax.y;
	
	myMin.z = (lhs.myMin.z < rhs.myMin.z) ? lhs.myMin.z : rhs.myMin.z;
	myMax.z = (lhs.myMax.z > rhs.myMax.z) ? lhs.myMax.z : rhs.myMax.z;

	checkValidity();
}


void Box3::inset( double insetAmount )
{
	if( !isValid() )
		return;
	
	// preserve the center in case our dimensions go zero
	stk::Vector3 centerPoint = center();
	
	outset( -insetAmount );
	
	// check for invalid dimension
	if ( getWidth() < 0 )
	{
		myMax.x = centerPoint.x;
		myMin.x = myMax.x;
	}
	if ( getHeight() < 0 )
	{
	    myMax.y = centerPoint.y;
		myMin.y = myMax.y;
	}
	if ( getDepth() < 0 )
	{
		myMax.z = centerPoint.z;
		myMin.z = myMax.z;
	}
}

void Box3::outset(double outsetAmount)
{
	if( !isValid() )
		return;
	
	myMin.x -= outsetAmount;
	myMin.y -= outsetAmount;	
	myMin.z -= outsetAmount;
	
	myMax.x += outsetAmount;
	myMax.y += outsetAmount;
	myMax.z += outsetAmount;

	checkValidity();
}
	
void Box3::offset(double offsetAmount)
{
	if( !isValid() )
		return;
	
	myMin.x += offsetAmount;
	myMin.y += offsetAmount;
	myMin.z += offsetAmount;
	
	myMax.x += offsetAmount;
	myMax.y += offsetAmount;
	myMax.z += offsetAmount;

	checkValidity();
}

void Box3::scaleAboutCenter(double scale)
{
	if( !isValid() )
		return;
	
	// if scale = 1.1, 10% scale up, 5% on each side
	const double normalizedScale = (scale - 1.0) * 0.5;	
	double 		adjust;
	
	adjust = (myMax.x - myMin.x) * normalizedScale;
	myMin.x -= adjust;
	myMax.x += adjust;
	
	adjust = (myMax.y - myMin.y) * normalizedScale;
	myMin.y -= adjust;
	myMax.y += adjust;
	
	adjust = (myMax.z - myMin.z) * normalizedScale;
	myMin.z -= adjust;
	myMax.z += adjust;

	checkValidity();
}

void Box3::checkValidity()
{
	if ( (myMin!=stk::Vector3(0.0, 0.0, 0.0)) || (myMax!=stk::Vector3(0.0, 0.0, 0.0)) )
		isEmpty = false;
	else
		isEmpty = true;
}
