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

    std::vector<Atomproperty>  mArrayAtomProp ;
    std::vector<Coord3D>  mCoords ;  ///< Array of cartesian coordinates


public:
    Rigidbody(){};
    ~Rigidbody(){};

    Atom CopyAtom(uint i) const ;
    inline uint size() const {return mArrayAtomProp.size();};   ///< return number of atoms
    void AddAtom(const Atomproperty& at, Coord3D co); ///< add an atom to the molecule (deep copy)
    Coord3D GetCoord(uint i){return mCoords[i];};
};



}




#endif //RIGIDBODY_H

