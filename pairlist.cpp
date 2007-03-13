#include "pairlist.h"

namespace PTools
{

PairList::PairList(const Rigidbody& receptor , const Rigidbody& ligand, double cutoff )
{
    mp_ligand = &ligand;
    mp_receptor = &receptor;
    squarecutoff = cutoff*cutoff;
    no_update = false;
    update();
}

PairList::PairList(const Rigidbody & receptor, const Rigidbody & ligand)
{

    mp_ligand = &ligand;
    mp_receptor = &receptor;
    no_update = true ; //if infinite cutoff

    for (uint i = 0 ; i < mp_ligand->Size(); i++)
        for (uint j = 0; j < mp_receptor->Size(); j++)
        {
            vectl.push_back(i); vectr.push_back(j);
        }

}


PairList::~PairList()
{

}


/**
   Very basic implementation of an atom pairlist. 
*/
void PairList::update()
{
    if (no_update) return ;
    vectl.clear(); // clears the pairlist
    vectr.clear();

    //basic implementation of a pairlist generation:
    for (uint i = 0 ; i < mp_ligand->Size(); i++)
        for (uint j = 0; j < mp_receptor->Size(); j++)
        {
            Coord3D c1 = mp_ligand->GetCoords(i) ;
            Coord3D c2 = mp_receptor->GetCoords(j);
            if (Norm2(c1-c2) <= squarecutoff)
            {
                  vectl.push_back(i);
                  vectr.push_back(j);
            }

        }

}



void PairList::addPair(const Rigidbody& ligand, const Rigidbody& receptor, AtomPair pair)
{
      if (&ligand==mp_ligand && &receptor==mp_receptor) 
          {
           vectl.push_back(pair.atlig);
           vectr.push_back(pair.atrec);
           }
      else
          {
            std::cerr << "error in PairList::add() : wrong receptor or ligand " << std::endl;
            abort();
           }
};



/*!
return percent of native contacts given initial and final state and cutoff (in Angstrom)

NOT FINISHED YET!!
TODO: FINISH IT...
 */
// double NativeContactsFraction (Rigidbody& rec1, Rigidbody& lig1, Rigidbody& rec2, Rigidbody& lig2, double cutoff)
// {
//     int compteur = 0 ;
//     PairList pair1(lig1, rec1, cutoff);
//     PairList pair2(lig2, rec2, cutoff);
// 
// 
// }


} //namespace PTools



