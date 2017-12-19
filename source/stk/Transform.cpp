
#include <assert.h>
#include "stk/Transform.h"

#include <math.h>

using namespace stk;

stk::Matrix stk::createScaleMatrix(const stk::Vector3& scale)
{
	stk::Matrix mat(4,4);
	for (int j=0 ; j<4 ; j++)
	{
		for (int i=0 ; i<4 ; i++)
		{
			mat(i,j) = 0;
		}
	}
	mat(0,0) = scale.x;
	mat(1,1) = scale.y;
	mat(2,2) = scale.z;
	mat(3,3) = 1.0;
	return mat;
}

stk::Matrix stk::createPosMatrix(const stk::Mark& pos)
{
	stk::Transform localToGlobal;
	localToGlobal.localToGlobal(pos);
	return localToGlobal;
}

stk::Transform::Transform() : Matrix(4,4)
{
	// Init to identity
	data[0*4+0] = 1;
	data[1*4+1] = 1;
	data[2*4+2] = 1;
	data[3*4+3] = 1;
}

stk::Transform::Transform(const Matrix& mat) : 
	Matrix(4, 4, false)
{
	assert(mat.sizeJ()==4 && mat.sizeI()==4);
	*(Matrix*)this = mat;
}

stk::Transform::~Transform() {}


void stk::Transform::localToGlobal(double roll, double pitch, double heading, const Vector3& D)
{
	double   cosl = cos(heading);
	double   sinl = sin(heading);
	double   cost = cos(pitch);
	double   sint = sin(pitch);
	double   cosr = cos(roll);
	double   sinr = sin(roll);

	//X = Vector3(cost*cosl, cost*sinl, -sint);
	data[0*4+0] = cost*cosl;
	data[1*4+0] = cost*sinl;
	data[2*4+0] = -sint;
	data[3*4+0] = 0;
	//Y = Vector3((sint*sinr*cosl - cosr*sinl), (sint*sinr*sinl + cosr*cosl), cost*sinr);
	data[0*4+1] = sint*sinr*cosl - cosr*sinl;
	data[1*4+1] = sint*sinr*sinl + cosr*cosl;
	data[2*4+1] = cost*sinr;
	data[3*4+1] = 0;
	//Z = Vector3((sint*cosr*cosl + sinr*sinl), (sint*cosr*sinl - sinr*cosl), cost*cosr);
	data[0*4+2] = sint*cosr*cosl + sinr*sinl;
	data[1*4+2] = sint*cosr*sinl - sinr*cosl;
	data[2*4+2] = cost*cosr;
	data[3*4+2] = 0;
	//Delta = D;
	data[0*4+3] = D.x; 
	data[1*4+3] = D.y; 
	data[2*4+3] = D.z;
	data[3*4+3] = 1;
}

void stk::Transform::localToGlobal(const Mark& Rep )
{
	localToGlobal(Rep.roll, Rep.pitch, Rep.heading, Vector3(Rep.x,Rep.y,Rep.z));
}

void stk::Transform::globalToLocal(double roll, double pitch, double heading, const Vector3& D)
{
	double   cosl = cos(heading);
	double   sinl = sin(heading);
	double   cost = cos(pitch);
	double   sint = sin(pitch);
	double   cosr = cos(roll);
	double   sinr = sin(roll);

	//X = Vector3(cost*cosl, (sint*sinr*cosl - cosr*sinl), (sint*cosr*cosl + sinr*sinl));
	data[0*4+0] = cost*cosl;
	data[1*4+0] = sint*sinr*cosl - cosr*sinl;
	data[2*4+0] = sint*cosr*cosl + sinr*sinl;
	data[3*4+0] = 0;
	//Y = Vector3(cost*sinl, (sint*sinr*sinl + cosr*cosl), (sint*cosr*sinl - sinr*cosl));
	data[0*4+1] = cost*sinl;
	data[1*4+1] = sint*sinr*sinl + cosr*cosl;
	data[2*4+1] = sint*cosr*sinl - sinr*cosl;
	data[3*4+1] = 0;
	//Z = Vector3(-sint, cost*sinr, cost*cosr);
	data[0*4+2] = -sint;
	data[1*4+2] = cost*sinr;
	data[2*4+2] = cost*cosr;
	data[3*4+2] = 0;
	//Delta = -(X*D.x + Y*D.y + Z*D.z);		//A VERIFIER   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	data[0*4+3] =  -(data[0*4+0] * D.x + data[0*4+1] * D.y + data[0*4+2] * D.z);
	data[1*4+3] =  -(data[1*4+0] * D.x + data[1*4+1] * D.y + data[1*4+2] * D.z);
	data[2*4+3] =  -(data[2*4+0] * D.x + data[2*4+1] * D.y + data[2*4+2] * D.z);
	data[3*4+3] = 1;
}

void stk::Transform::globalToLocal(const Mark& Rep)
{
	globalToLocal(Rep.roll, Rep.pitch, Rep.heading, Vector3(Rep.x,Rep.y,Rep.z));
}

Vector3 stk::Transform::apply(const Vector3& P) const
{
	Vector3 tmp;
	tmp.x = data[0*4+0]*P.x + data[0*4+1]*P.y + data[0*4+2]*P.z + data[0*4+3];
	tmp.y = data[1*4+0]*P.x + data[1*4+1]*P.y + data[1*4+2]*P.z + data[1*4+3];
	tmp.z = data[2*4+0]*P.x + data[2*4+1]*P.y + data[2*4+2]*P.z + data[2*4+3];
	return tmp;
}


Vector3 stk::Transform::applyTranslation(const Vector3& P) const
{
	return Vector3(data[0*4+3]+P.x, data[1*4+3]+P.y, data[2*4+3]+P.z);
}

Vector3 stk::Transform::applyRotation(const Vector3& P) const
{
	Vector3 tmp;
	tmp.x = data[0*4+0]*P.x + data[0*4+1]*P.y + data[0*4+2]*P.z;
	tmp.y = data[1*4+0]*P.x + data[1*4+1]*P.y + data[1*4+2]*P.z;
	tmp.z = data[2*4+0]*P.x + data[2*4+1]*P.y + data[2*4+2]*P.z;
	return tmp;
}


Transform& stk::Transform::operator= (const Matrix& mat)
{
	assert(sizei==4 && sizei==4);
	assert(mat.sizeJ()==4 && mat.sizeI()==4);

	Matrix::operator=(mat);
	
	return *this;
}

Mark stk::Transform::toMark() const
{
//from http://pc12gra.u-strasbg.fr/these_html/node133.html
	const Matrix& data = *this;
	Mark tmp;
	tmp.x = data(0,3);
	tmp.y = data(1,3);
	tmp.z = data(2,3);

	tmp.pitch = -asin(data(2,0));		//(X.z);
	double cost = cos(tmp.pitch);
	tmp.heading = -atan2( (-data(1,0) / cost), (data(0,0) / cost));	//(-X.y/cosr), (X.x/cosr));
	tmp.roll = -atan2( (-data(2,1) / cost), (data(2,2) / cost));	//(-Y.z/cosr), (Z.z/cosr));	

	return tmp;
}

// Optimized inversion for 4x4 matrices
// from OSG 2.8.2: bool Matrix_implementation::invert_4x4( const Matrix_implementation& mat )
template <class T>
inline T SGL_ABS(T a)
{
   return (a >= 0 ? a : -a);
}
#ifndef SGL_SWAP
#define SGL_SWAP(a,b,temp) ((temp)=(a),(a)=(b),(b)=(temp))
#endif
bool stk::Transform::inverse()
{
    //if (&mat==this) {
    //   Matrix_implementation tm(mat);
    //   return invert_4x4(tm);
    //}

    unsigned int indxc[4], indxr[4], ipiv[4];
    unsigned int i,j,k,l,ll;
    unsigned int icol = 0;
    unsigned int irow = 0;
    double temp, pivinv, dum, big;

    // copy in place this may be unnecessary
    //*this = mat;

    for (j=0; j<4; j++) ipiv[j]=0;

    for(i=0;i<4;i++)
    {
       big=0.0;
       for (j=0; j<4; j++)
          if (ipiv[j] != 1)
             for (k=0; k<4; k++)
             {
                if (ipiv[k] == 0)
                {
                   if (SGL_ABS(operator()(j,k)) >= big)
                   {
                      big = SGL_ABS(operator()(j,k));
                      irow=j;
                      icol=k;
                   }
                }
                else if (ipiv[k] > 1)
                   return false;
             }
       ++(ipiv[icol]);
       if (irow != icol)
          for (l=0; l<4; l++) SGL_SWAP(operator()(irow,l),
                                       operator()(icol,l),
                                       temp);

       indxr[i]=irow;
       indxc[i]=icol;
       if (operator()(icol,icol) == 0)
          return false;

       pivinv = 1.0/operator()(icol,icol);
       operator()(icol,icol) = 1;
       for (l=0; l<4; l++) operator()(icol,l) *= pivinv;
       for (ll=0; ll<4; ll++)
          if (ll != icol)
          {
             dum=operator()(ll,icol);
             operator()(ll,icol) = 0;
             for (l=0; l<4; l++) operator()(ll,l) -= operator()(icol,l)*dum;
          }
    }
    for (int lx=4; lx>0; --lx)
    {
       if (indxr[lx-1] != indxc[lx-1])
          for (k=0; k<4; k++) SGL_SWAP(operator()(k,indxr[lx-1]),
                                       operator()(k,indxc[lx-1]),temp);
    }

    return true;
}
