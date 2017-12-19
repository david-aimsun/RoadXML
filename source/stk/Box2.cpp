#include "stk/Box2.h"

#include "stk/Macro.h"
#include <algorithm>

// ---------------
stk::Box2::Box2() :
mIsEmpty(true),
mMin(0,0),
mMax(0, 0)
{
}

stk::Box2::Box2(const stk::Vector2& min, const stk::Vector2& max)
	: mIsEmpty(false),
	  mMin( min ),
	  mMax( max )
{
}

// add vertex and update the bounding volume
void stk::Box2::add (const stk::Vector2& p )
{
	if (mIsEmpty)
	{
		mMin = p;
		mMax = p;
		mIsEmpty = false;
	}
	else
	{
		mMin.x = CMIN(mMin.x, p.x);
		mMin.y = CMIN(mMin.y, p.y);

		mMax.x = CMAX(mMax.x, p.x);
		mMax.y = CMAX(mMax.y, p.y);
	}
};

void stk::Box2::setToIntersection( const stk::Box2& lhs, const stk::Box2& rhs )
{
	if (!lhs.isValid() || !rhs.isValid())
		return;

	// subtle change in comparison from union
	mMin.x = (lhs.mMin.x > rhs.mMin.x) ? lhs.mMin.x : rhs.mMin.x;
	mMax.x = (lhs.mMax.x < rhs.mMax.x) ? lhs.mMax.x : rhs.mMax.x;
	
	mMin.y = (lhs.mMin.y > rhs.mMin.y) ? lhs.mMin.y : rhs.mMin.y;
	mMax.y = (lhs.mMax.y < rhs.mMax.y) ? lhs.mMax.y : rhs.mMax.y;

	mIsEmpty = false;
}

void stk::Box2::setToUnion(const stk::Box2& lhs, const stk::Box2& rhs )
{
	if (!lhs.isValid() || !rhs.isValid())
		return;

	// subtle change in comparison from intersection
	mMin.x = (lhs.mMin.x < rhs.mMin.x) ? lhs.mMin.x : rhs.mMin.x;
	mMax.x = (lhs.mMax.x > rhs.mMax.x) ? lhs.mMax.x : rhs.mMax.x;
	
	mMin.y = (lhs.mMin.y < rhs.mMin.y) ? lhs.mMin.y : rhs.mMin.y;
	mMax.y = (lhs.mMax.y > rhs.mMax.y) ? lhs.mMax.y : rhs.mMax.y;

	mIsEmpty = false;
}

void stk::Box2::scaleAboutCenter(const double scale)
{
	if( !isValid() )
		return ;

	// if scale = 1.1, 10% scale up, 5% on each side
	const double normalizedScale = (scale - 1.0f) * 0.5f;	
	double 		adjust;
	
	adjust = (mMax.x - mMin.x) * normalizedScale;
	mMin.x -= adjust;
	mMax.x += adjust;
	
	adjust = (mMax.y - mMin.y) * normalizedScale;
	mMin.y -= adjust;
	mMax.y += adjust;
}

bool stk::Box2::isInsideOrIntersecting(const stk::Box2& checkIfInsideBox) const
{
	if ( this == &checkIfInsideBox ) // fast self intersection test
		return true;
		
	if( !isValid() || !checkIfInsideBox.isValid() )
		return false;
	
	return !((checkIfInsideBox.mMax.x <= mMin.x) ||
			 (checkIfInsideBox.mMin.x >= mMax.x) ||
			 (checkIfInsideBox.mMax.y <= mMin.y) ||
			 (checkIfInsideBox.mMin.y >= mMax.y));
}
		
bool stk::Box2::isInsideOrTouching(const stk::Box2& checkIfInsideBox) const
{
	// see if the min/max overlap the same range as us (rely on its validation checking)
	if ( !isInsideOrIntersecting( checkIfInsideBox ))
		return false;
	
	return  ((checkIfInsideBox.mMax.x <= mMax.x) &&
			 (checkIfInsideBox.mMin.x >= mMin.x) &&
			 (checkIfInsideBox.mMax.y <= mMax.y) &&
			 (checkIfInsideBox.mMin.y >= mMin.y));
}

bool stk::Box2::isSharingSomeSpace(const stk::Box2& otherBox) const
{
	if (isInsideOrIntersecting(otherBox))
		return true;
	else if(otherBox.isInsideOrIntersecting(*this))
		return true;
	else	
		return false;
}

bool stk::Box2::isPointInside(const stk::Vector2& vertex,const bool equalInside ) const
{
	if( !isValid() )
		return false;
	
	if ( equalInside )
	{
		if ( vertex.y > mMax.y )
			return false;
		if ( vertex.y < mMin.y )
			return false;
		
		if ( vertex.x > mMax.x )
			return false;
		if ( vertex.x < mMin.x )
			return false;
	}
	else
	{
		if ( vertex.y >= mMax.y )
			return false;
		if ( vertex.y <= mMin.y )
			return false;
		
		if ( vertex.x >= mMax.x )
			return false;
		if ( vertex.x <= mMin.x )
			return false;
	}
			
	return true;
}

void stk::Box2::inset( const double insetAmount )
{
	if( !isValid() )
		return;
	
	// preserve the center in case our dimensions go zero
	stk::Vector2 centerPoint = center();
	
	outset( -insetAmount );
	
	// check for invalid dimension
	if ( getWidth() < 0 )
	{
		mMax.x = centerPoint.x;
		mMin.x = mMax.x;
	}
	if ( getHeight() < 0 )
	{
	    mMax.y = centerPoint.y;
		mMin.y = mMax.y;
	}
}

void stk::Box2::outset(const double outsetAmount)
{
	if( !isValid() )
		return ;
	
	mMin.x -= outsetAmount;
	mMin.y -= outsetAmount;
	
	mMax.x += outsetAmount;
	mMax.y += outsetAmount;
}
	
void stk::Box2::offset(const double offsetAmount)
{
	if( !isValid() )
		return ;
	
	mMin.x += offsetAmount;
	mMin.y += offsetAmount;

	mMax.x += offsetAmount;
	mMax.y += offsetAmount;
}

bool stk::Box2::isSegmentInside( const stk::Vector2& point1, const stk::Vector2& point2 ) const
{
	if( !isValid() )
		return false;
	
	if( CMIN(point1.x, point2.x)>mMax.x ) return false;
	if( CMAX(point1.x, point2.x)<mMin.x ) return false;
	if( CMIN(point1.y, point2.y)>mMax.y ) return false;
	if( CMAX(point1.y, point2.y)<mMin.y ) return false;

	return true;
}
