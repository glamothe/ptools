#include <ptools.h>
#include <rigidbody.h>
#include <iostream>


using namespace PTools;

int main()
{
Rigidbody prot("1FIN_r.pdb");
AtomSelection chainA = prot.SelectChainId(" ");

AtomSelection sel1 = prot.SelectResRange(5,36);
AtomSelection sel2 = prot.SelectResRange(40,52);
AtomSelection sel3 = prot.CA();
AtomSelection sel4 = !sel3;

std::cout << sel3.Size()<< "  " <<sel4.Size() << std::endl;



AtomSelection result = (sel1 | sel2) & !sel3;

std::cout << sel1.Size() << "  " << sel2.Size() << "  " << (sel1 | sel2).Size() << " " << result.Size() << std::endl;


Rigidbody prot2 = result.CreateRigid();
WritePDB(prot2,"toto.txt");

}