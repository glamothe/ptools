#ifndef PAIRLIST_H
#define PAIRLIST_H

#include "rigidbody.h"

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
*Very basic implementation. Will need further improvement.
*/

template <class T>
class T_PairList
{
public:
    T_PairList(const T & receptor, const T & ligand, double cutoff );
    T_PairList(const T & receptor,const T &  ligand); ///< constructor with infinite cutoff ;
    ~T_PairList();
    void update(); ///< update pairlist
    void addPair(const T& ligand, const T& receptor, AtomPair pair) ; ///< Add a pair and checks if correct ligand and receptor are provided

    bool no_update; ///< if true the pairlist is never updated
    double GetCutoff() {
        return sqrt(squarecutoff);
    };
    uint Size() {
        return vectl.size();
    };
    AtomPair operator[](int i) {
        AtomPair pair;
        pair.atlig=vectl[i];
        pair.atrec=vectr[i];
        return pair;
    };


private:
    double squarecutoff ; ///< cutoff^2
    const T* mp_ligand;
    const T* mp_receptor;

    std::vector <uint> vectl ; ///< indice des atomes du ligand
    std::vector <uint> vectr ; ///< indice des atomes du receptor

};



template <class T>
T_PairList<T>::T_PairList(const T & receptor , const T & ligand, double cutoff )
{
    mp_ligand = &ligand;
    mp_receptor = &receptor;
    squarecutoff = cutoff*cutoff;
    no_update = false;
    update();
}

template <class T>
T_PairList<T>::T_PairList(const T & receptor, const T & ligand)
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

template <class T>
T_PairList<T>::~T_PairList()
{

}


/**
   Very basic implementation of an atom pairlist.
*/
template<class T>
void T_PairList<T>::update()
{


    if (no_update) return ;
    vectl.clear(); // clears the pairlist
    vectr.clear();

    //basic implementation of a pairlist generation:
    uint ligsize = mp_ligand->Size();
    uint recsize = mp_receptor->Size();

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

                
                    //Atom at1 = mp_ligand->GetAtom(i);
                    //Atom at2 = mp_receptor->GetAtom(j);

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

template <class T>
void T_PairList<T>::addPair(const T & ligand, const T & receptor, AtomPair pair)
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









typedef T_PairList<Rigidbody> PairList;
typedef T_PairList<AttractRigidbody> Attract2PairList ;


// inline void instantiate(){
//
//         sizeof( PairList );
// }


}//namespace PTools

#endif

