#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <vector>
#include <cassert>


#include "coord3d.h"
#include "atom.h"
#include "basetypes.h"

namespace PTools {


class AtomSelection; // forward declaration

class Rigidbody
{

private:
    std::vector<Atom> mAtoms; ///< vector of Atoms
    std::vector<Coord3D> mForces; ///< forces for each atom
    double mat44[4][4]; ///< rotation/tranlation matrix

public:
    Rigidbody();  ///< basic constructor
    Rigidbody(std::string filename);
    Rigidbody(const Rigidbody& model); ///< copy

    ~Rigidbody(){};

    /// Make a deep copy of one atom (the Atom extracted is then totally independent)
    Atom CopyAtom(uint i) const ;

    ///  get the Atom of index i (with direct access to modifications)
    Atom& GetAtom(uint pos) {
        assert(pos>=0);
        assert(pos<Size());
        return mAtoms[pos];
    };

    /// synonym of GetAtom
    Atom& operator[](uint pos) {return GetAtom(pos);};

    /// return number of atoms
    inline uint Size() const {return mAtoms.size();};

    /// add an atom to the molecule (deep copy)
    void AddAtom(const Atomproperty& at, Coord3D co);

    /// add an atom to the molecule
    void AddAtom(const Atom& at);

    /// returns the coordinates of atom i
    Coord3D GetCoords(uint i) const {
     assert(i>=0);
     assert(i<Size());
        return mAtoms[i].GetCoords();
    };

    /// define the coordinates of atom i
    void SetCoords(uint i, Coord3D& co){mAtoms[i].SetCoords(co);};

    /// return geometric center of all atoms
    Coord3D FindCenter() const;

    /// center the rigidbody to the Origin (0,0,0)
    void CenterToOrigin();

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

    void ABrotate(const Coord3D& A, const Coord3D& B, double theta); ///< rotation around (AB) axis.

    void ResetMatrix(); ///< reset Rotation/translation matrix to identity.
    void PrintMatrix() const; ///< print the rotation/translation matrix
    void ApplyMatrix(double mat[4][4]);
    Vdouble GetMatrix() const;



    friend void AttractEuler(const Rigidbody& source, Rigidbody& dest, double phi, double ssi, double rot);
    friend void mat44xrigid( const Rigidbody& source, Rigidbody& result, double mat[ 4 ][ 4 ] );
    friend void Translate(const Rigidbody& source, Rigidbody& target, const Coord3D& trans);
}; // end class Rigidbody

} // end namespace PTools

#endif //RIGIDBODY_H

