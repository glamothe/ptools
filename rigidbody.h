#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <boost/shared_ptr.hpp>
#include <vector>

#include "coord3d.h"
#include "atom.h"

namespace PTools {

class Rigidbody
{

private:

    std::vector<Atom>     mArrayAtoms ;
    std::vector<coord3D>  mCoords ;  ///< Array of cartesian coordinates


public:
    Rigidbody(){};
    ~Rigidbody(){};

    inline Atom GetAtom(uint i) const {return mArrayAtoms[i] ;};
    inline uint size() const {return mArrayAtoms.size();};   ///< return number of atoms
    void AddAtom(const Atom& at); ///< add an atom to the molecule (deep copy)
    coord3D GetCoord(uint i){return mCoords[i];};
};



}




#endif //RIGIDBODY_H

