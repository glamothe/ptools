#ifndef _CONSTRAINT_H_
#define _CONSTRAINT_H_

#include "attractrigidbody.h"


namespace PTools
{

struct Constraint
{
int lig1, lig2, at1 , at2;
double rstk;
};



Constraint CreateDefaultConstraint(const AttractRigidbody& rec,const AttractRigidbody& lig, uint recnb, uint lignb);

}

#endif //_CONSTRAINT_H_


