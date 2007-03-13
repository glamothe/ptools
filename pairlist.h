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

//! Contains list of pairs of atoms in interaction
/*!
Very basic implementation. Will need further improvement. 

 */

class PairList
{
public:
    PairList(const Rigidbody& receptor, const Rigidbody& ligand, double cutoff );
    PairList(const Rigidbody& receptor,const Rigidbody& ligand); ///< constructor with infinite cutoff ;
    ~PairList();
    void update(); ///< update pairlist
    void addPair(const Rigidbody& ligand, const Rigidbody& receptor, AtomPair pair) ; ///< Add a pair and checks if correct ligand and receptor are provided

    bool no_update; ///< if true the pairlist is never updated
    double GetCutoff() {return sqrt(squarecutoff);};
    uint Size() {return vectl.size();};
    AtomPair operator[](int i) {AtomPair pair; pair.atlig=vectl[i];pair.atrec=vectr[i]; return pair;};

    void NewCutoff(double cutoff);


private:
    double squarecutoff ; ///< cutoff^2
    const Rigidbody* mp_ligand;
    const Rigidbody* mp_receptor;

    std::vector <uint> vectl ; ///< indice des atomes du ligand
    std::vector <uint> vectr ; ///< indice des atomes du receptor
};



struct NativeContacts
{
      double atoms;
      double residues;
};

// double NativeContactsFraction (Rigidbody& rec1, Rigidbody& lig1, Rigidbody& rec2, Rigidbody& lig2, double cutoff);


}//namespace PTools

#endif

