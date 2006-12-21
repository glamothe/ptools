#include "stdio.h" 

#include "pdbout.h"
#include "atom.h"

namespace PTools{

void WritePDB(const Rigidbody& rigid, std::string filename)
{


 file= fopen(filename.c_str(),"w") ;


    for(uint i=0; i<rigid.Size();i++)
    {

        const char * chainID="A" ;

        Atom at = rigid.GetAtom(i);
        const char* atomname=at.GetType.c_str();
        const char* residName=at.GetResidType.c_str();
        int residnumber = at.GetResidId;
        chainID=at.GetChainId.c_str();

        int atomnumber = at.GetAtomId;

        Coord3D coord = at.GetCoords();
        double x = coord.x;
        double y = coord.y;
        double z = coord.z ;



        fprintf(fich,"ATOM  %5d  %-4s%3s %1s%4d    %8.3f%8.3f%8.3f",atomnumber,atomname,residName,chainID,residnumber,x,y,z);
        fprintf(fich,"\n");
    }


} //namespace PTools

}