#include <iostream>
#include <vector>

#include <rigidbody.h>
#include <attractrigidbody.h>
#include <mcopff.h>


using namespace PTools;



int main()
{





Rigidbody m("mainR.pdb");
AttractRigidbody am (m);
Rigidbody c1("copy1R.pdb");
Rigidbody c2("copy2R.pdb");

AttractRigidbody ac1(c1);
AttractRigidbody ac2 (c2);


Region reg ;
reg.addCopy(ac1);
reg.addCopy(ac2);


Mcoprigid mcrec ;
mcrec.setMain(am);
mcrec.addEnsemble(reg);



Rigidbody lig("mainR.pdb");
AttractRigidbody alig (lig);

Mcoprigid mclig ;
mclig.setMain(alig);



AttractForceField1 ff ("aminon.par", 12.);
McopForceField FF (ff, 12.);

FF.setLigand(mclig);
FF.setReceptor(mcrec);

vector<dbl> v(6);

FF.calculate_weights(mclig);
std::cout << FF.Function(v) << std::endl;

return 0;
}