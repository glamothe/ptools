#include "attractrigidbody.h"
#include "scorpionforcefield.h"

#include <iostream>

using namespace std;
using namespace PTools;


int main()
{

AttractRigidbody rec("receptor.red");
AttractRigidbody lig("ligand.red");

ScorpionForceField ff("scorpion.par", 1000);

ff.AddLigand(rec);
ff.AddLigand(lig);

std::vector<double> X(20);


cout << ff.Function(X) << endl;

return 0;
}
