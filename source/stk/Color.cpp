/****************************************************************************************************

		Color.cpp
		Copyright: (c) Oktal SA. All rights reserved.

		Author:	Julien
		Date:	mercredi 25 janvier 2006

****************************************************************************************************/
#include "stk/Color.h"
//#include "stk/CTMaths.h"
#include "stk/Macro.h"
#include <iomanip>

using namespace stk;

bool IsClose(double val0,double val1,double tolerance )
{
	return ((((val0) - (val1))>0)?((val0) - (val1)) <= (tolerance) :((val1) - (val0)) <= (tolerance));
}

const Color Color::kBlack(0.0,0.0,0.0,1.0);
const Color Color::kWhite(1.0,1.0,1.0,1.0);
const Color Color::kGrey(0.5,0.5,0.5,1.0);
const Color Color::kRed(1.0,0.0,0.0,1.0);
const Color Color::kGreen(0.0,1.0,0.0,1.0);
const Color Color::kBlue(0.0,0.0,1.0,1.0);


// linear interpolation, interpolants must be between 0 and 1
/*void Color::Interpolate(
		const Color&	from,
		const Color&	to,
		const double			interpolant ) // must be between 0 and 1
{
	CTAssert( interpolant >= 0.0 && interpolant <= 1.0 );
	
	red 	= LinearInterpol( from.red, to.red, interpolant );
	green 	= LinearInterpol( from.green, to.green, interpolant );
	blue 	= LinearInterpol( from.blue, to.blue, interpolant );
	alpha 	= LinearInterpol( from.alpha, to.alpha, interpolant );
}

// bilinear interpolation, interpolants must be betwen 0 and 1
void Color::Interpolate(
		const double interpolantsHor, // [left/right]
		const double interpolantsVer, // [lower/upper]
		const Color& lowerLeft,
		const Color& lowerRight,
		const Color& upperLeft,
		const Color& upperRight )
{
	Color lower, upper;
	
	lower.Interpolate( lowerLeft, lowerRight, interpolantsHor );
	upper.Interpolate( upperLeft, upperRight, interpolantsHor );
				
	this->Interpolate( lower, upper, interpolantsVer );
}*/

bool Color::isEqual(
		const Color& 	rhs,
		const double 		epsilon ) const
{
	if ( epsilon == 0.0 )
	{
		return 	(red 	== rhs.red) && 
				(green 	== rhs.green) &&
				(blue 	== rhs.blue) &&
				(alpha 	== rhs.alpha );
	}
	else
	{
		return 	IsClose( red, rhs.red, epsilon ) &&
				IsClose( green, rhs.green, epsilon ) &&
				IsClose( blue, rhs.blue, epsilon ) &&
				IsClose( alpha, rhs.alpha, epsilon );
	}
}

bool Color::IsClamped() const
{
	return 	red >= 0 	&& red <= 1 &&
			green >= 0 	&& green <= 1 &&
			blue >= 0 	&& blue <= 1 &&
			alpha >= 0	&& alpha <= 1;
}

void Color::Clamp()
{
	red = LIMIT_INTERVAL( 0, red, 	1 );
	green = LIMIT_INTERVAL( 0, green,	1 );
	blue = LIMIT_INTERVAL( 0, blue, 	1 );
	alpha = LIMIT_INTERVAL( 0, alpha,	1 );
}


Color::Color()
	{ }

Color::Color(const double rr,const double gg,const double bb,const double aa )
	: red( rr ), green( gg ), blue( bb ), alpha( aa )
	{ }

Color::Color(const std::string& color)
{
	if (color.size() == 7)
	{
		std::stringstream stringHexa(color.substr(1,6));
		int hexcolor =0;
		stringHexa >> std::hex >>hexcolor;
		red = (hexcolor >> 16) & 0xFF;
		green = (hexcolor >> 8) & 0xFF;
		blue = hexcolor & 0xFF;
		alpha = 1.0;
		Clamp();
	}
}

std::string Color::name() const
{
	int r = static_cast<int>(red * 255);
	int g = static_cast<int>(green *255);
	int b = static_cast<int>(blue *255);
	int hexcolor = (r << 16) + (g << 8) + b;
	std::ostringstream sred;
	sred << std::setfill('0') << std::setw(2) << std::hex << r;
	std::ostringstream sgreen;
	sgreen << std::setfill('0') << std::setw(2) << std::hex << g;
	std::ostringstream sblue;
	sblue << std::setfill('0') << std::setw(2) << std::hex << b;
	std::ostringstream oss;
	oss << "#" << sred.str() << sgreen.str() << sblue.str();
	return oss.str();
}

//void Color::Set(const double rr, const double gg, const double bb, const double aa )
//	{ red = rr; green = gg; blue = bb; alpha = aa; }

/*double Color::GetMagnitude() const
	{ return (double)sqrt( GetMagnitudeSquared() ); }

double Color::GetMagnitudeSquared() const
	{ return red * red + green * green + blue * blue + alpha * alpha; }
		
double Color::GetMinOfRGB() const
	{ double min = ( red < green ); return ( min < blue ) ? min : blue; }

double Color::GetMaxOfRGB() const
	{ double min = ( red > green ); return ( min > blue ) ? min : blue; }*/



bool Color::operator== (const Color& rhs) const 
	{ return isEqual( rhs ); }

bool Color::operator!= (const Color& rhs) const 
	{ return !isEqual( rhs ); }


		
Color& Color::operator+= (const Color& rhs)
	{ 		red 	+= rhs.red;
			green 	+= rhs.green;
			blue 	+= rhs.blue;
			//alpha 	+= rhs.alpha;
			return *this;
	}

Color& Color::operator-= (const Color& rhs)
	{ 		red 	-= rhs.red;
			green 	-= rhs.green;
			blue 	-= rhs.blue;
			//alpha 	-= rhs.alpha;
			return *this;
	}

Color& Color::operator*= (const double s)
	{ 		red 	*= s;
			green 	*= s;
			blue 	*= s;
			//alpha 	*= s;
			return *this;
	}

Color& Color::operator/= (const double s)
	{ 		red 	/= s;
			green 	/= s;
			blue 	/= s;
			//alpha 	/= s; 
			return *this;
	}



Color Color::operator+ (const Color& rhs) const
	{ return Color(*this) += rhs; }
Color Color::operator- (const Color& rhs) const
	{ return Color(*this) -= rhs; }
Color Color::operator* (const double s) const
	{ return Color(*this) *= s; }
Color Color::operator/ (const double s) const
	{ return Color(*this) /= s; }

/*double Color::operator*	(const Color& rhs) const
	{ return red * rhs.red + green * rhs.green + blue * rhs.blue + alpha * rhs.alpha; }

Color Color::operator% (const Color& rhs) const
	{ return Color( red * rhs.red, green * rhs.green, blue * rhs.blue, alpha * rhs.alpha ); }*/
		
/*void Color::AccumRGBA(const Color& rhs, const double	scalar )
	{ 		red 	+= ( rhs.red	* scalar);
			green 	+= ( rhs.green 	* scalar);
			blue 	+= ( rhs.blue 	* scalar);
			alpha 	+= ( rhs.alpha 	* scalar);
	}



void Color::AccumRGB(const Color& rhs, const double scalar )
	{ 		red 	+= ( rhs.red	* scalar);
			green 	+= ( rhs.green 	* scalar);
			blue 	+= ( rhs.blue 	* scalar);
	}*/
					

//Color& Color::operator=  (const double i) { CTAssert(i==0); red=i;green=i;blue=i;alpha=1; return *this; }

const double* Color::GetAddrFirstComponent() const 
	{ return &red; }

std::ostream& stk::operator<<(std::ostream& out, const Color& color)
{
	out << color.red << " " << color.green << " " << color.blue << " " << color.alpha;
	return out;
}

std::istream& stk::operator>> (std::istream& in, Color& color)
{
	color = stk::Color::kWhite;
	in >> color.red >> color.green >> color.blue >> color.alpha;
	return in;
}
