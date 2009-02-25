#include "ptools.h"

#include <iostream>


using namespace std;
using namespace PTools;

int main()
{
Rigidbody r("1FIN_r_main.red");
Rigidbody l("1FIN_c_l.red");


AttractRigidbody ar(r);
AttractRigidbody al(l);

AttractForceField1 ff1("aminon.par", 14);

Constraint constr = CreateDefaultConstraint(ar,al,0,1);

Vdouble X(6);
ar.setRotation(false);
ar.setTranslation(false);

ff1.AddLigand(ar);
ff1.AddLigand(al);

std::cout << ff1.Function(X)<< std::endl;

ff1.AddConstraint(constr);
std::cout << ff1.Function(X)<< std::endl;



Lbfgs minim(ff1);
minim.minimize(200,X);







}

