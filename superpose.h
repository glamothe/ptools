

#ifndef _SUPERPOSE_H_
#define _SUPERPOSE_H_

#include <iostream>

#include "screw.h"

#include "rigidbody.h"
// #include "Rotation.h"
#include "vissage.h"


namespace PTools
{


typedef double Mat33[3][3];
typedef double Mat44[4][4];


void Rotate(Rigidbody& rigid, Mat33 mat);

void Mat44toMat33trans(Mat44 mat44, Mat33 mat33, Coord3D& trans);
Vissage MatTrans2screw(Mat33 rotmatrix, Coord3D trans); // transforme t(r(X)) en un vissage d'axe de rotation colin�aire au vecteur translation.

Vissage superpose_sippl(const Rigidbody& ref, const Rigidbody& mob, int verbosity=0);

// Superpose_t superpose( const Rigidbody & ref, const Rigidbody& mob, int verbosity=0);
// 
}

#endif




