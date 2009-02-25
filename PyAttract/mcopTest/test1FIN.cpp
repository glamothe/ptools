#include <iostream>
#include <vector>

#include <rigidbody.h>
#include <attractrigidbody.h>
#include <derivify.h>
#include <minimizers/lbfgs_interface.h>


// #include <pdbio.h>
#include <mcopff.h>



using namespace PTools;
using namespace std;



dbl func(dbl x)
{
return 4*x*x;
}







int main()
{





Rigidbody m("1FIN_r_main.red");
AttractRigidbody am (m);
Rigidbody c1("1FIN_r_1_2.red");
Rigidbody c2("1FIN_r_1_1.red");

AttractRigidbody ac1(c1);
AttractRigidbody ac2 (c2);


Ensemble reg ;
reg.addCopy(ac1);
reg.addCopy(ac2);


Mcoprigid mcrec ;
mcrec.setMain(am);
mcrec.addEnsemble(reg);



Rigidbody lig("1FIN_c_l.red");
AttractRigidbody alig (lig);

Mcoprigid mclig ;
mclig.setMain(alig);



// AttractForceField1 ff ("aminon.par", 12.);


// BaseAttractForceField* mff = attractforceFieldCreator<AttractForceField1>("aminon.par", 12);

FFcreator fcreator =  (attractforceFieldCreator<AttractForceField1>);


McopForceField FF (factoryCreator1() , "aminon.par", 12., mcrec, alig);

vector<dbl> v(6);
FF.Function(v);
FF.calculate_weights(true);
std::cout << FF.Function(v) << std::endl;


Lbfgs minim(FF);
minim.minimize(200,v);



/*



 vector<dbl> d(6);

 for (uint i=0; i<6; i++)
{
v[i]=0.0; d[i]=0.0;
}














// 
// 
// 
// 
// 
// 
// 
// 
// 
// for (int i=0; i<3; i++)
// {
// v[4]+=100.0;
// // v[2]+=100;
// // v[4]+=20;
// // v[5]+=120;
// 
// std::cout << FF.Function(v) << std::endl;
// 
// 
// }
// 




std::cout << FF.Function(v) << std::endl;
cout << "**************\n";




std::vector<dbl> autoderiv;

for (int i=0; i<6; i++)
 {
    Vdouble v2(v);
    v2[i]=v[i]+surreal(0,1);
   
   autoderiv.push_back(FF.Function(v2));
 }


for (int i=0; i<6; i++)
 cout << imag(autoderiv[i]) << "  ";

cout << endl;


FF.Function(v);
FF.Derivatives(v,d);
for (int i=0; i<6; i++) 
   cout << real(d[i]) << "  " ;


for (int i=0; i<50; i++)
 {
 cerr << "taille du pb: " << FF.ProblemSize() << endl;
 Lbfgs minim(FF);
 minim.minimize(200,v);

FF.Function(v);
FF.calculate_weights(true);

for (int i=0; i<6; i++)
   cout <<v[i] << " ";
cout <<"\n";

 }
*/

return 0;
}