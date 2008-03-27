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

class Attract2PairList
{
public:
    Attract2PairList(const AttractRigidbody & receptor, const AttractRigidbody & ligand, dbl cutoff );
    Attract2PairList(const AttractRigidbody & receptor,const AttractRigidbody &  ligand); ///< constructor with infinite cutoff ;
    Attract2PairList(){}; //null constructor for use with std::vector

    ~Attract2PairList();
    void update(); ///< update pairlist
    void addPair(const AttractRigidbody& ligand, const AttractRigidbody& receptor, const AtomPair& pair) ; ///< Add a pair and checks if correct ligand and receptor are provided

    bool no_update; ///< if true the pairlist is never updated
    dbl GetCutoff() {
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
    dbl squarecutoff ; ///< cutoff^2
    const AttractRigidbody* mp_ligand;
    const AttractRigidbody* mp_receptor;

    std::vector <uint> vectl ; ///< indice des atomes du ligand
    std::vector <uint> vectr ; ///< indice des atomes du receptor

};











// typedef Attract2PairList<Rigidbody> PairList;
// typedef Attract2PairList<AttractRigidbody> Attract2PairList ;


// inline void instantiate(){
//
//         sizeof( PairList );
// }


}//namespace PTools

#endif

