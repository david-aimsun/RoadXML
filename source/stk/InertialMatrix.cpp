
#include "stk/InertialMatrix.h"
#include "stk/Math.h"

using namespace stk;

////////////////////////////////////////////////////////////////////////////
//classe Matrix Inertie : en cours de développement
//
//	Usage:
//  Moments = InertialMatrix * AccelerationAngulaires
//
//	(Vector3) =  ........... * (Vector3)
////////////////////////////////////////////////////////////////////////////
	
stk::InertialMatrix::InertialMatrix() :
	data(3,3), //met la bonne taille et des 0 partout
	posCoG(0,0,0),	//(0,0,0)
	massSolid(0)
{}


/*stk::InertialMatrix::InertialMatrix(const Solide& S) :
	data(3,3), //met la bonne taille et des 0 partout
	posCoG(0,0,0),	//(0,0,0)
	massSolid(0)
{
	posCoG = S.Pos;
	massSolid = S.Masse;

	//Que des termes diagonaux
	//S'ils sont nuls, on leur met un petit truc
	data(0,0) =  (S.RayonGir.x == 0) ? 0.001 : S.Inertie().x;	//Ixx
	data(1,1) =  (S.RayonGir.y == 0) ? 0.001 : S.Inertie().y;	//Iyy
	data(2,2) =  (S.RayonGir.z == 0) ? 0.001 : S.Inertie().z;	//Izz
}*/

stk::InertialMatrix::InertialMatrix(double Ixx, double Iyy,double Izz,
							   double Ixy, double Ixz, double Iyz, const Vector3& CdG, double Masse) :
	data(3,3,false),
	posCoG(CdG),
	massSolid(Masse)
{
	data(0,0) =	Ixx;
	data(0,1) =	-Ixy;
	data(0,2) =	-Ixz;
	
	data(1,0) =	-Ixy;
	data(1,1) =	Iyy;
	data(1,2) =	-Iyz;

	data(2,0) =	-Ixz;
	data(2,1) =	-Iyz;
	data(2,2) =	Izz;

}

stk::InertialMatrix::InertialMatrix(double Ixx, double Iyy,double Izz, const Vector3& posCoG, double Masse) :
	data(3,3),
	posCoG(posCoG),
	massSolid(Masse)
{
	data(0,0) =	Ixx;
	data(1,1) =	Iyy;
	data(2,2) =	Izz;

}

stk::InertialMatrix stk::InertialMatrix::operator+(const stk::InertialMatrix& Mat) const
{
	InertialMatrix tmp1 = Mat;
	InertialMatrix tmp2 = *this;

	if(Mat.posCoG != posCoG)
	{
		Vector3 NouveauBary = barycenter(massSolid, posCoG, Mat.massSolid, Mat.posCoG);
		tmp1.changePoint(NouveauBary);
		tmp2.changePoint(NouveauBary);
	}

	tmp1.data += tmp2.data;
	tmp1.massSolid += tmp2.massSolid;

	return tmp1;
}

stk::InertialMatrix& stk::InertialMatrix::operator =(const stk::InertialMatrix& Mat)
{
	posCoG = Mat.posCoG;
	massSolid = Mat.massSolid;
	data = Mat.data;
	return *this;
}

void stk::InertialMatrix::changePoint (const Vector3& NouveauPoint)
{
	Vector3 OG = posCoG - NouveauPoint;

	//ici les nouveaux termes de la matrice (Huygens)
	data(0,0) =	data(0,0) + (OG.y * OG.y + OG.z * OG.z) * massSolid;
	data(0,1) =	data(0,1) + (- OG.x * OG.y) * massSolid;
	data(0,2) =	data(0,2) + (- OG.x * OG.z) * massSolid;
	
	data(1,0) =	data(1,0) + (- OG.x * OG.y) * massSolid;
	data(1,1) =	data(1,1) + (OG.x * OG.x +  OG.z * OG.z) * massSolid;
	data(1,2) =	data(1,2) + (- OG.y * OG.z) * massSolid;

	data(2,0) =	data(2,0) + (- OG.x * OG.z) * massSolid;
	data(2,1) =	data(2,1) + (- OG.y * OG.z) * massSolid;
	data(2,2) =	data(2,2) + (OG.x * OG.x +  OG.y * OG.y) * massSolid;

	posCoG = NouveauPoint;
}

void stk::InertialMatrix::applyTransformation(const Transform& MT)
{
	posCoG = MT.apply(posCoG);
	
	Matrix MatrixRotation(3,3);
	for(int i = 0; i<3; i++)
		for(int j = 0; j<3; j++)
			MatrixRotation(i,j) = MT(i,j);

	Matrix MatrixRotationTransposee(3,3);
	Transpose(MatrixRotationTransposee, MatrixRotation);
	Mult(data, (MatrixRotationTransposee * data), MatrixRotation);
}


double stk::InertialMatrix::getMass() const
{
	return massSolid;
}

const Vector3& stk::InertialMatrix::getPosCoG() const
{
	return posCoG;
}

double stk::InertialMatrix::getInertia(int i, int j) const
{
	return data(i,j);
}

/*Solide stk::InertialMatrix::getSolide() const
{
	//on note que cette conversion n'est exacte que si le repère du solide est orienté selon ses axes d'inertie principaux
	Vector3 I(GetInertia(0,0), GetInertia(1,1), GetInertia(2,2));

	if(GetMass()>0)
	{
		Vector3 Rgir2 = I / GetMass();
		Vector3 Rgir(sqrt(Rgir2.x), sqrt(Rgir2.y), sqrt(Rgir2.z));

		return Solide(GetMass(), GetPosCoG(), Rgir);
	}
	return Solide(0);
}*/


Vector3 stk::computeTorque(const InertialMatrix& MI, const Vector3& AccelAngulaire)
{
	Matrix MatPoint = toMatrix31(AccelAngulaire);
	MatPoint = MI.data * MatPoint;

	return toVector3(MatPoint);
}

Vector3 stk::computeAngularAccel(const InertialMatrix& MI, const Vector3& Couple)
{	
	Matrix MatPoint = toMatrix31(Couple);
	
	Matrix MatInverse = MI.data;
	MatInverse.inverse();

	MatPoint = MatInverse * MatPoint;

	return toVector3(MatPoint);
}
