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

    std::vector<Atomproperty>  mArrayAtomProp ; ///< array of atom properties
    std::vector<Coord3D>  mCoords ;  ///< Array of cartesian coordinates


public:
    Rigidbody(){};
    ~Rigidbody(){};

    Atom CopyAtom(uint i) const ; ///< Make a deep copy of one atom (the Atom extracted is then totally independent)
    inline uint Size() const {return mArrayAtomProp.size();};   ///< return number of atoms
    void AddAtom(const Atomproperty& at, Coord3D co); ///< add an atom to the molecule (deep copy)
    void AddAtom(const Atom& at); ///< add an atom to the molecule
    Coord3D GetCoords(uint i) const {return mCoords[i];}; ///< returns the coordinates of atom i
    void SetCoords(uint i, Coord3D& co){mCoords[i]=co;}; ///< define the coordinates of atom i
    Coord3D FindCenter(); ///< return geometric center of all atoms
    
};

}

#endif //RIGIDBODY_H

