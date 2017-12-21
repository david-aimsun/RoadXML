
#include "stk/Matrix.h"

#include <algorithm>
#include <math.h>
#include <memory.h>
#include <float.h>

using namespace stk;



//--------------------------------------------------------------------------------------------
//							Functions déclarations
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
//	Assumes that matrix is a Matrix(3, 3)
bool determinantMatrix33(const Matrix &matrix, double &determinant);

//--------------------------------------------------------------------------------------------
//	Assumes that matrix is a Matrix(4, 4) and minorMatrices is an array of Matrix(3,3)
//	Use the first lines of the matrix
bool determinantMatrix44(const Matrix &matrix, double &determinant, Matrix minorMatrices[4], double minorMatricesDet[4]);

//--------------------------------------------------------------------------------------------
//	Assumes that matrix is an Matrix(4, 4)
bool inverseMatrix44(Matrix &matrix);


//--------------------------------------------------------------------------------------------
//							Matrix44 Functions Definitions
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
bool determinantMatrix44(const Matrix &matrix, double &determinant, Matrix minorMatrices[4], double minorMatricesDet[4])
{
	for (int i = 0; i < 4; ++i)
	{
		matrix.minorMatrix(minorMatrices[i], 3, i);
		if (!determinantMatrix33(minorMatrices[i], minorMatricesDet[i]))
			return false;
		std::cout << "Det=" << minorMatricesDet[i] << std::endl;
	}
	
	determinant = 0.0;
	double sign = -1.0;
	for (int i = 0; i < 4; ++i)
	{
		determinant += sign * matrix(3, i) * minorMatricesDet[i];
		sign *= -1.0;
	}
	return determinant != 0.;
}

//--------------------------------------------------------------------------------------------
bool inverseMatrix44(Matrix &matrix)
{
	Matrix oldMatrice(matrix);
	double minorMatricesDet[4][4];
	Matrix minorMatrices[4][4] = {
		{ Matrix(3, 3), Matrix(3, 3), Matrix(3, 3), Matrix(3, 3) },
		{ Matrix(3, 3), Matrix(3, 3), Matrix(3, 3), Matrix(3, 3) },
		{ Matrix(3, 3), Matrix(3, 3), Matrix(3, 3), Matrix(3, 3) },
		{ Matrix(3, 3), Matrix(3, 3), Matrix(3, 3), Matrix(3, 3) }
	};
	double determinant = 0.0;

	//	Calculate the determinant using the first line of the matrix
	if (!determinantMatrix44(oldMatrice, determinant, minorMatrices[3], minorMatricesDet[3]))
		return false;

	//	Calculate the last cofactor matrices and their determinant
	for (int i = 0; i < 4; ++i)
	{
		oldMatrice.minorMatrix(minorMatrices[0][i], 0, i);
		oldMatrice.minorMatrix(minorMatrices[1][i], 1, i);
		oldMatrice.minorMatrix(minorMatrices[2][i], 2, i);
		if (!determinantMatrix33(minorMatrices[0][i], minorMatricesDet[0][i]))
			return false;
		if (!determinantMatrix33(minorMatrices[1][i], minorMatricesDet[1][i]))
			return false;
		if (!determinantMatrix33(minorMatrices[2][i], minorMatricesDet[2][i]))
			return false;
	}

	//	Calculate the inverse matrix
	double sign_i = 1.0;
	for (int i = 0; i < 4; ++i)
	{
		double sign_j = 1.0;
		for (int j = 0; j < 4; ++j)
		{
			matrix(i, j) = sign_i * sign_j * minorMatricesDet[j][i] / determinant;
			sign_j *= -1.0;
		}
		sign_i *= -1.0;
	}

	return true;
}


//--------------------------------------------------------------------------------------------
//							Matrix33 Methods Definitions
//--------------------------------------------------------------------------------------------
Matrix33::Matrix33() : stk::Matrix(3,3) {}
Matrix33::~Matrix33() {}
Matrix33::Matrix33(double m00, double m01, double m02,
		 double m10, double m11, double m12,
		 double m20, double m21, double m22) :
	stk::Matrix(3,3)
{
	(*this)(0,0)=m00;
	(*this)(0,1)=m01;
	(*this)(0,2)=m02;
	(*this)(1,0)=m10;
	(*this)(1,1)=m11;
	(*this)(1,2)=m12;
	(*this)(2,0)=m20;
	(*this)(2,1)=m21;
	(*this)(2,2)=m22;
}

Matrix& Matrix33::operator=(const Matrix& Mat)
{
	assert(Mat.sizeI()==sizei && Mat.sizeJ()==sizej);
	*((Matrix*)this) = Mat;
	return *this;
}

///Local inversion
bool Matrix33::inverse()
{
	if (!isInversible())
		return false;
	Matrix33 temp(*this);
	double det = 1 / temp.determinant();
	data[0] = ( temp.data[4]*temp.data[8] - temp.data[5]*temp.data[7] ) * det;
	data[1] = ( temp.data[2]*temp.data[7] - temp.data[1]*temp.data[8] ) * det;
	data[2] = ( temp.data[1]*temp.data[5] - temp.data[2]*temp.data[4] ) * det;
	data[3] = ( temp.data[5]*temp.data[6] - temp.data[3]*temp.data[8] ) * det;
	data[4] = ( temp.data[0]*temp.data[8] - temp.data[2]*temp.data[6] ) * det;
	data[5] = ( temp.data[2]*temp.data[3] - temp.data[0]*temp.data[5] ) * det;
	data[6] = ( temp.data[3]*temp.data[7] - temp.data[4]*temp.data[6] ) * det;
	data[7] = ( temp.data[1]*temp.data[6] - temp.data[0]*temp.data[7] ) * det;
	data[8] = ( temp.data[0]*temp.data[4] - temp.data[1]*temp.data[3] ) * det;
	return true;
}


//--------------------------------------------------------------------------------------------
bool determinantMatrix33(const Matrix &matrix, double &determinant)
{
	determinant	=	matrix(0, 0) * (matrix(1, 1) * matrix(2, 2) - matrix(1, 2) * matrix(2, 1));
	determinant	-=	matrix(0, 1) * (matrix(1, 0) * matrix(2, 2) - matrix(1, 2) * matrix(2, 0));
	determinant	+=	matrix(0, 2) * (matrix(1, 0) * matrix(2, 1) - matrix(1, 1) * matrix(2, 0));
	return true;
}



//--------------------------------------------------------------------------------------------
//					Matrix Global Functions 
//--------------------------------------------------------------------------------------------

bool stk::isZero(const stk::Matrix& mat)
{
	for (int i=0 ; i<mat.sizeI() ; i++)
		for (int j=0 ; j<mat.sizeJ() ; j++)
			if (mat(i,j)!=0)
				return false;
	return true;
}

void stk::Add(Matrix& C, const Matrix& A, const Matrix& B)
{
	assert(C.sizei==A.sizei && C.sizei==B.sizei);
	assert(C.sizej==A.sizej && C.sizei==B.sizej);

	int size = A.sizei*A.sizej;
	for (int i=0 ;  i<size ; i++)
		C.data[i] = A.data[i] + B.data[i];
}

void stk::Sub(Matrix& C, const Matrix& A, const Matrix& B)
{
	assert(C.sizei==A.sizei && C.sizei==B.sizei);
	assert(C.sizej==A.sizej && C.sizei==B.sizej);

	int size = A.sizei*A.sizej;
	for (int i=0 ;  i<size ; i++)
		C.data[i] = A.data[i] - B.data[i];
}

//A = transposé de B
void stk::Transpose(Matrix& A, const Matrix& B)
{
	assert(A.sizeI()==B.sizeJ());
	assert(A.sizeJ()==B.sizeI());

	for(int row = 0; row<A.sizeI() ; row ++)
		for(int col = 0; col <A.sizeJ(); col ++)
			A(col, row) = B(row, col);
}

void stk::Mult(Matrix& C, const Matrix& A, const Matrix& B)
{
	assert(A.sizej==B.sizei);
	assert(C.sizei==A.sizei);
	assert(C.sizej==B.sizej);

	for (int i=0 ; i<C.sizei ; i++)
	{
		int i_x_Asizej = i*A.sizej;
		for (int j=0 ; j<C.sizej ; j++)
		{
			double sum = 0;
			for (int k=0, k_x_Bsizej=0; k<A.sizej ; k++, k_x_Bsizej+=B.sizej)
				sum += A.data[i_x_Asizej + k] * B.data[k_x_Bsizej + j];
			C.data[i*C.sizej + j] = sum;
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//		Classe Matrix
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


stk::Matrix::Matrix(int sizei, int sizej, bool Reset) : sizei(0), sizej(0), data(0)
{
	resize(sizei, sizej, Reset);
}

stk::Matrix::Matrix(const Matrix& Mat) : sizei(0), sizej(0), data(0)
{
	resize(Mat.sizei, Mat.sizej, false);
	memcpy(data, Mat.data, sizei*sizej*sizeof(double));
}

stk::Matrix::~Matrix()
{
	if( data )
		delete [] data;
}

stk::Matrix& Matrix::operator=(const Matrix& Mat)
{
	resize(Mat.sizei, Mat.sizej, false);
	memcpy(data, Mat.data, sizei*sizej*sizeof(double));
	return *this;
}

void stk::Matrix::resize(int I, int J, bool Reset)
{
	if (I!=sizei || J!=sizej)
	{
		if( data )
			delete [] data;
		sizei = I;
		sizej = J;
		data = new double[sizei*sizej];
	}
	if (Reset)
		memset(data, 0, sizeof(double)*sizei*sizej);
}

int stk::Matrix::sizeI() const
{
	return sizei;
}

int stk::Matrix::sizeJ() const
{
	return sizej;
}

Matrix stk::Matrix::operator*(const Matrix& Mat) const
{
	assert (sizej == Mat.sizei);
	Matrix tmp(sizei, Mat.sizej);
	Mult(tmp, *this, Mat);
	return tmp;
}

Matrix stk::Matrix::operator*(double val) const
{
	Matrix tmp(*this);
	tmp *= val;
	return tmp;
}

Matrix& stk::Matrix::operator*=(double val)
{
	int size = sizei*sizej;
	for (int i=0 ; i<size ; i++)
		data[i] *= val;
	return *this;
}

Matrix& stk::Matrix::operator*=(const Matrix& Mat)
{
	Matrix tmp(sizei, Mat.sizej);
	Mult(tmp, *this, Mat);
	*this = tmp;
	return *this;
}

Matrix stk::Matrix::operator+(const Matrix& Mat) const
{
	assert (sizei == Mat.sizei);
	assert (sizej == Mat.sizej);

	Matrix tmp(sizei, sizej);
	Add(tmp, *this, Mat);
	return tmp;
}

Matrix stk::Matrix::operator-(const Matrix& Mat) const
{
	assert (sizei == Mat.sizei);
	assert (sizej == Mat.sizej);

	Matrix tmp(sizei, sizej);
	Sub(tmp, *this, Mat);
	return tmp;
}

Matrix& stk::Matrix::operator+=(const Matrix& Mat)
{
	assert (sizei == Mat.sizei);
	assert (sizej == Mat.sizej);

	int size = sizei*sizej;
	for (int i=0 ; i<size ; i++)
		data[i] += Mat.data[i];
	return *this;
}

Matrix& stk::Matrix::operator-=(const Matrix& Mat)
{
	assert (sizei == Mat.sizei);
	assert (sizej == Mat.sizej);

	int size = sizei*sizej;
	for (int i=0 ; i<size ; i++)
		data[i] -= Mat.data[i];
	return *this;
}

void stk::Matrix::transpose()
{
	Matrix transposee(sizej,sizei);

	::Transpose(transposee, *this);

	(*this) = transposee;
}

void stk::Matrix::lmatrix( stk::Matrix &lmat, int n, int l) const
{
	int ld=0;
	for(int i=0;i<n;i++)
	{
		if(i!=l)
		{
			for(int j=1;j<n;j++)
			{
				lmat(ld,(j-1)) = (*this)(i,j);
			}
			ld++;
		}
	}
}

//--------------------------------------------------------------------------------------------
void stk::Matrix::minorMatrix(stk::Matrix &matrix, int line, int col) const
{
	if (sizei < 2 || sizej < 2)
		return;
	matrix.resize(sizei - 1, sizej - 1, false);

	int ni = 0;
	for (int i = 0; i < sizei; ++i)
	{
		if (i == line)
			continue;

		int nj = 0;
		for (int j = 0; j < sizej; ++j)
		{
			if (j == col)
				continue;
			matrix(ni, nj++) = (*this)(i, j);
		}

		ni += 1;
	}
}

//--------------------------------------------------------------------------------------------
double stk::Matrix::determinant() const
{
	double det = 0.;

	//	Optimized version for 3x3 matrix
	if (sizei == 3 && sizej == 3)
	{
		if (determinantMatrix33(*this, det))
			return det;
	}
	//	Optimized version for 4x4 matrix
	else if (sizei == 4 && sizej == 4)
	{
		double minorMatricesDet[4];
		Matrix minorMatrices[4] = { Matrix(3, 3), Matrix(3, 3), Matrix(3, 3), Matrix(3, 3) };
		if (determinantMatrix44(*this, det, minorMatrices, minorMatricesDet))
			return det;
	}

	//	General case
	int n = sizei;
	int k=n-1;

	double sign=1.;

	stk::Matrix lmat(k,k);
	if(n==1)
	{
		return (*this)(0,0);
	}
	for(int i=0;i<n;i++)
	{
		lmatrix(lmat,n,i);
		det = det + sign*(*this)(i, 0)*lmat.determinant();
		sign=-sign;
	}
	return det;
}

void exchangeRow(stk::Matrix &M, int k, int l)
{
	if ( k<0 || l<0 || k>=M.sizeI() || l>=M.sizeJ() || k==l)
		return;

	for (int j=0 ; j<M.sizeJ() ; j++)
	{
		std::swap(M(k,j),M(l,j));
	}	

	return;
}

// Matrix inversion using Gauss-Jordan method
// source: wikipedia
bool stk::Matrix::inverse()
{
	//the matrix must be square to be inverted
	if (sizei != sizej)
		return false;

	//	Optimized implementation for 4x4 matrix
	if (sizei == 4 && sizej == 4)
	{
		if (inverseMatrix44(*this))
			return true;
	}

	//std::vector<bool> I(sizei,false);
	bool *I = new bool[sizei];
	for( int i=0 ; i<sizei ; i++ )
		I[i] = false;

	//std::vector<bool> J(sizej,false);
	bool *J = new bool[sizej];
	for( int j=0 ; j<sizej ; j++ )
		J[j] = false;

	//copy the original matrix in A
	stk::Matrix A = *this;

	stk::Matrix B(sizei,sizej);
	InitIdentity(B);

	//Flag used to exit when input matrix can't be inverted
	int cnt_row = 0;
	int cnt_col = 0;

	for (int k=0; k<sizei ; k++) 
	{
		if ( !I[k] ) 
		{
			I[k] = true;
			cnt_row++;
			bool bl = true;
			for (int l=0; l<sizej && bl; l++) 
			{
				if ( !J[l] ) 
				{
					double a = A(k,l); 			
					if (a != 0) 
					{
						J[l] = true;
						cnt_col++;			    
						bl = false;
						for (int p=0; p<sizei; p++)
						{
							if (p != k)
							{
								double tmp = A(p,l);
								for (int q=0; q<sizej; q++)
								{
									A(p,q) = A(p,q) - A(k,q)*(tmp/a);
									B(p,q) = B(p,q) - B(k,q)*(tmp/a);
								}
							}	
						}
					}			
				}
			}
			if (cnt_row != cnt_col) 
			{
				//Matrix is singular : can't compute any inverse matrix;
				delete [] (I);
				delete [] (J);
				return false;
			}	       
		}
	}

	for (int l=0; l<sizej; l++)
	{
		for (int k=0; k<sizei; k++)
		{
			double a = A(k,l);
			//if (a != 0)
			if ( fabs(a) > DBL_EPSILON)//EPSILON
			{
				A(k,l) = 1;
				for (int p=0; p<sizej; p++)
				{
					B(k,p) = B(k,p)/a;
				}
				if (k != l)
				{
					exchangeRow(A,k,l);
					exchangeRow(B,k,l);
				}
				k = sizei;
			}
		}
	}
	(*this) = B;
	delete [] (I);
	delete [] (J);
	return true;
}

bool stk::Matrix::isInversible() const
{
	return determinant()!=0;
}

Matrix stk::toMatrix41(const Vector3& P)
{
	Matrix tmp(4,1);
	tmp.data[0] = P.x;
	tmp.data[1] = P.y;
	tmp.data[2] = P.z;
	tmp.data[3] = 1;
	return tmp;
}

Matrix stk::toMatrix31(const Vector3& P)
{
	Matrix tmp(3,1);
	tmp.data[0] = P.x;
	tmp.data[1] = P.y;
	tmp.data[2] = P.z;
	return tmp;
}


Vector3 stk::toVector3(const Matrix& M)
{	
	assert(M.sizei>=3 && M.sizej==1);
	return Vector3(M.data[0], M.data[1], M.data[2]);
}




void stk::InitIdentity(Matrix& M)
{
	for(int i = 0; i<M.sizeI() ; i ++)
	{
		for(int j = 0; j <M.sizeJ(); j ++)
		{
            M(i, j) = (i==j) ? 1 : 0;
		}
	}
}

void stk::InitTranslation(Matrix& M, const Vector3& P)
{
	assert(M.sizeI()==4 && M.sizeJ()==4);
	InitIdentity(M);
	M(0,3) = P.x,
		M(1,3) = P.y,
		M(2,3) = P.z;
}

void stk::InitRotation(Matrix& M, const Vector3& Axe, double angle)
{
	assert(M.sizeI()==4 && M.sizeJ()==4);
	InitIdentity(M);

	if (fabs(angle) < 1e-10)
		return;
	Vector3 K = Axe/Axe.getNorm();

	double s, c;
	double xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin(angle);
	c = cos(angle);

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

	M(0,0) = ((one_c * xx) + c);  M(0,1) = ((one_c * xy) - zs); M(0,2) = ((one_c * zx) + ys);
	M(1,0) = ((one_c * xy) + zs); M(1,1) = ((one_c * yy) + c);  M(1,2) = ((one_c * yz) - xs);
	M(2,0) = ((one_c * zx) - ys); M(2,1) = ((one_c * yz) + xs); M(2,2) = ((one_c * zz) + c);
}

void stk::InitScale(Matrix& M, const Vector3& Scale)
{
	assert(M.sizeI()==4 && M.sizeJ()==4);
	InitIdentity(M);
	M(0,0) = Scale.x,
		M(1,1) = Scale.y,
		M(2,2) = Scale.z;
}

void stk::Matrix::dump(std::ostream& out) const
{
	for(int row = 0; row < sizei; row++)
	{
		//out << "ligne n " << (row+1);
		for(int col = 0; col < sizej; col++)
			out << (*this)(row,col) << "\t";
		out <<  "\n";
	}
	out.flush();
}



//--------------------------------------------------------------------------------------------

//NR ALTERNATIVE
//#include <nr.h>
//#include <nrutil.h>
//
//double** toNRMatrix(const stk::Matrix& M)
//{
//	double** MatNR = dmatrix(1,M.sizeI(), 1, M.sizeJ());
//
//	for (int i = 0; i<M.sizeI(); i++)
//		for (int j = 0; j<M.sizeJ(); j++)
//			MatNR[i+1][j+1] = M(i,j);
//	
//	return MatNR;
//}
//
//void fromMatrixNR(stk::Matrix& M, double **MatNR, int taille)
//{
//	M.resize(taille, taille, false);
//
//	for (int i = 0; i<taille; i++)
//		for (int j = 0; j<taille; j++)
//			M(i,j) = MatNR[i+1][j+1];
//
//}
//
//bool stk::Matrix::inverse()
//{
//	assert(sizei==sizej);
//
//	double **a = toNRMatrix(*this);
//	int n =  sizei;
//	
//	double **y = dmatrix(1,n, 1, n);
//
//	double d;
//	double *col = dvector(1,n);
//	int	*indx = ivector(1,n);
//	///
//	ludcmp(a,n,indx,&d); //Decompose the matrix just once.
//	//assert(d!=0);
//	for(int j=1;j<=n;j++) 
//	{ //Find inverse by columns.
//		int i;
//		for(i=1;i<=n;i++) 
//			col[i]=0.0;
//		col[j]=1.0;
//		lubksb(a,n,indx,col);
//		for(i=1;i<=n;i++)
//			y[i][j]=col[i];
//	}
//
//
//	fromMatrixNR(*this,y,n);
//
//	free_dmatrix(a, 1,n, 1,n);
//	free_dmatrix(y, 1,n, 1,n);
//	free_dvector(col, 1,n);
//	free_ivector(indx, 1,n);
//	return true;
//}
//
//double stk::Matrix::determinant() const
//{
//	double d = 0;
//	double **a = toNRMatrix(*this);
//	int n =  sizei;
//
//	int	*indx = ivector(1,n);
////Decompose the matrix just once
//	if (!ludcmp(a,n,indx,&d))
//		return 0;
//	for(int j=1;j<=n;j++)
//		d *= a[j][j];
//
//	free_dmatrix(a, 1,n, 1,n);
//	free_ivector(indx, 1,n);
//	return d;
//}
//END NR ALTERNATIVE