// $Id$
#ifndef PAIRLIST_H
#define PAIRLIST_H

#include "attractrigidbody.h"


#include <vector>


namespace PTools
{


struct AtomPair
{
    uint atlig;
    uint atrec;
};



/*! \brief Contains list of pairs of atoms in interaction
*
*
*/
template <class MoleculeType>
class AttractPairList
{
public:
    AttractPairList(const MoleculeType & receptor, const MoleculeType & ligand, dbl cutoff )
{
    mp_ligand = &ligand;
    mp_receptor = &receptor;
    squarecutoff = cutoff*cutoff;
    no_update = false;
    update();
}



    AttractPairList(const MoleculeType & receptor,const MoleculeType &  ligand) ///< constructor with infinite cutoff 
{

    mp_ligand = &ligand;
    mp_receptor = &receptor;
    no_update = true ; //if infinite cutoff

    for (uint i = 0 ; i < mp_ligand->Size(); i++)
        for (uint j = 0; j < mp_receptor->Size(); j++)
        {
            vectl.push_back(i);
            vectr.push_back(j);
        }

}




    AttractPairList(){}; //null constructor for use with std::vector

    ~AttractPairList(){};

    ///update pairlist
    void update()
{


    if (no_update) return ;
    vectl.clear(); // clears the pairlist
    vectr.clear();

    //basic implementation of a pairlist generation:

    std::vector<uint> activelig;
    std::vector<uint> activerec;

    for (uint i=0; i<mp_ligand->Size(); i++)
    {
        if (mp_ligand->isAtomActive(i))
        {
            activelig.push_back(i);
        }
    }

    for (uint i=0; i<mp_receptor->Size(); i++)
    {
        if (mp_receptor->isAtomActive(i))
        {
            activerec.push_back(i);
        }
    }

    uint activeligsize = activelig.size();
    uint activerecsize = activerec.size();


    for (uint ii = 0 ; ii < activeligsize ; ii++)
    {

        for (uint jj = 0; jj < activerecsize; jj++)
        {
            uint i=activelig[ii];
            uint j=activerec[jj];


            Coord3D c1 = mp_ligand->GetCoords(i) ;
            Coord3D c2 = mp_receptor->GetCoords(j);
            if (Norm2(c1-c2) <= squarecutoff)
            {
                vectl.push_back(i);
                vectr.push_back(j);
            }


        }
    }


}



    /// Add a pair and checks if correct ligand and receptor are provided
    void addPair(const MoleculeType& ligand, const MoleculeType& receptor, const AtomPair& pair)
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





    ///return the current cutoff
    dbl GetCutoff() {
        return sqrt(squarecutoff);
    };

    ///return number of pairs of atoms in interaction (distance <= cutoff)
    uint Size() {
        return vectl.size();
    };

    /// get atom pair number i of the pairlist
    AtomPair operator[](int i) {
        AtomPair pair;
        pair.atlig=vectl[i];
        pair.atrec=vectr[i];
        return pair;
    };


private:

    dbl squarecutoff ; ///< cutoff^2
    const MoleculeType* mp_ligand;
    const MoleculeType* mp_receptor;

    bool no_update; ///< if true the pairlist is never updated

    std::vector <uint> vectl ; ///< index of ligands atoms
    std::vector <uint> vectr ; ///< index of receptor atoms

};




}//namespace PTools

#endif

