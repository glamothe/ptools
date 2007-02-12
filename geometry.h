#ifndef GEOMETRY
#define GEOMETRY


#include "rigidbody.h"
#include <iostream>


namespace PTools{


typedef double Mat44[4][4];

void MakeRotationMatrix(Coord3D A, Coord3D B, double theta,  double out[4][4]);
void XRotation(const Rigidbody& source, Rigidbody &result,  double alpha); // rotation autour de l'axe X
void mat44xVect(double mat[4][4], const Coord3D& vect, Coord3D& out);

void mat44xrigid(const Rigidbody& source, Rigidbody& result, double mat[4][4]);
void ABrotate(Coord3D A, Coord3D B, const Rigidbody& source, Rigidbody& result, double theta);
void EulerZYZ(const Rigidbody & source, Rigidbody & cible, double theta, double phi, double psi);


void MultMat4x4(double R1[4][4], double R2[4][4], double out[4][4]);


void ProdVect(const Coord3D& u,const Coord3D& v, Coord3D& UvectV);

void printmat44(const double mat[4][4]);

// TODO: metre ces fonctions dans un autre fichier, avec un namespace et tout et tout.
inline double prodscal( const Coord3D& a, const Coord3D& b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z ;
}

double dihedral(const Coord3D& a, const Coord3D& b, const Coord3D& c, const Coord3D& d);

double Angle(const Coord3D& vector1, const Coord3D& vector2);

#endif  //ifndef GEOMETRY

}

