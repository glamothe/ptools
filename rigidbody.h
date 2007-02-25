#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <boost/shared_ptr.hpp>
#include <vector>

#include "coord3d.h"
#include "atom.h"

namespace PTools {


class AtomSelection; // forward declaration

class Rigidbody
{

private:
    std::vector<Atom> mAtoms; ///< vector of Atoms

public:
    Rigidbody(){};
    ~Rigidbody(){};

    /// Make a deep copy of one atom (the Atom extracted is then totally independent)
    Atom CopyAtom(uint i) const ;

    ///  get the Atom of index i (with direct access to modifications)
    Atom& GetAtom(uint pos) {return mAtoms[pos];};

    /// synonym of GetAtom
    Atom& operator[](uint pos) {return GetAtom(pos);};

    /// return number of atoms
    inline uint Size() const {return mAtoms.size();};

    /// add an atom to the molecule (deep copy)
    void AddAtom(const Atomproperty& at, Coord3D co);

    /// add an atom to the molecule
    void AddAtom(const Atom& at);

    /// returns the coordinates of atom i
    Coord3D GetCoords(uint i) const {return mAtoms[i].GetCoords();};

    /// define the coordinates of atom i
    void SetCoords(uint i, Coord3D& co){mAtoms[i].SetCoords(co);};

    /// return geometric center of all atoms
    Coord3D FindCenter();

    void Translate(const Coord3D& tr); ///< Translate the whole object

    /// selection : complete
    AtomSelection SelectAllAtoms(Rigidbody& rigid);

    /// selection : by atom type
    AtomSelection SelectAtomType(std::string atomtype);

    /// selection by residue type
    AtomSelection SelectResidType(std::string residtype);

    /// selection by chain ID
    AtomSelection SelectChainId(std::string chainid);

    /// selection by range of residue ID
    AtomSelection SelectResRange(uint start, uint stop);

    /// selection shortcut for C-alpha
    AtomSelection CA();

    /// operator =
    Rigidbody& operator= (const Rigidbody& rig);

    /// operator +
    Rigidbody operator+ (const Rigidbody& rig);

}; // end class Rigidbody

} // end namespace PTools

#endif //RIGIDBODY_H

