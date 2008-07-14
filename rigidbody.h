#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <vector>
#include <cassert>


#include "coord3d.h"
#include "atom.h"
#include "basetypes.h"
#include "coordsarray.h"


namespace PTools
{


namespace {
// anonymous namespace for copying the mat44xVect from geometry.h
// this is a pretty ugly hack and if someone finds a better way, then please do it !

inline void mat44xVect(const dbl mat[ 4 ][ 4 ], const Coord3D& vect, Coord3D& out )
{
    out.x = vect.x * mat[ 0 ][ 0 ] + vect.y * mat[ 0 ][ 1 ] + vect.z * mat[ 0 ][ 2 ] + mat[ 0 ][ 3 ] ;
    out.y = vect.x * mat[ 1 ][ 0 ] + vect.y * mat[ 1 ][ 1 ] + vect.z * mat[ 1 ][ 2 ] + mat[ 1 ][ 3 ] ;
    out.z = vect.x * mat[ 2 ][ 0 ] + vect.y * mat[ 2 ][ 1 ] + vect.z * mat[ 2 ][ 2 ] + mat[ 2 ][ 3 ] ;
}

}








class AtomSelection; // forward declaration

class Rigidbody:private CoordsArray
{

private:

    std::vector<Atom> mAtoms; ///< vector of Atoms
    std::vector<Coord3D> mForces; ///< forces for each atom

    std::string _description; ///< some string to describe the molecule

    virtual void m_hookCoords(uint i, Coord3D & co) const ; ///< private hook to give a chance for class childs to change the coordinates before rotations and translations


protected:
    std::vector<Atomproperty> mAtomProp; ///< array of atom properties


public:
    Rigidbody();  ///< basic constructor
    Rigidbody(std::string filename); ///create a Rigidbody object and loads a PDB file
    Rigidbody(const Rigidbody& model); ///< copy constructor

    virtual ~Rigidbody(){};

    uint Size() const {return CoordsArray::Size();};


    /// Make a deep copy of one atom (the Atom extracted is then totally independent)
    virtual Atom CopyAtom(uint i) const ;

/*    /// const version of GetAtom*/
    /*Atom GetAtom(uint pos) const
    {
        Coord3D co;
        CoordsArray::GetCoords(pos, co);
        Atom at(mAtomProp[pos], co );
        return at;
    }*/

    
    Atomproperty const & GetAtomProperty(uint pos) const
    {
        return mAtomProp[pos];
    }


    /// add an atom to the molecule (deep copy)
    void AddAtom(const Atomproperty& at, Coord3D co);

    /// add an atom to the molecule
    void AddAtom(const Atom& at);

    //returns the coordinates of atom i
    Coord3D GetCoords(uint i) const
    {
        assert(i<Size());
        Coord3D c;
        CoordsArray::GetCoords(i,c) ;  //get the coordinates after translation/rotation

        return c;  //finally returns the final coordinates
    };


    /// return geometric center of all atoms
    Coord3D FindCenter() const;

    /// center the rigidbody to the Origin (0,0,0)
    void CenterToOrigin();

    void Translate(const Coord3D& tr); ///< Translate the whole object

    ///Radius of gyration
    dbl RadiusGyration();

    /// returns the radius of a Rigidbody (max distance from center)
    dbl Radius();

    /// converts rigidbody to classical PDB-like string
    std::string PrintPDB() const ;

    /// selection : complete
    AtomSelection SelectAllAtoms();

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

    /// operator + : merge two Rigdibody by extending the first coordinates with the second coordinates.
    Rigidbody operator+ (const Rigidbody& rig);

    void ABrotate(const Coord3D& A, const Coord3D& B, dbl theta); ///< rotation around (AB) axis.

    /// in some cases atoms may be ignored
    virtual bool isAtomActive(uint i) const {return true;};

    /// set a description for the object (ex: "mutant A192G")
    void setDescription(const std::string & descr) {_description = descr;};
    /// return the object name/description
    std::string getDescription(){return _description;};

    void AttractEulerRotate(dbl phi, dbl ssi, dbl rot);

    //friends
    friend void ABrotate( Coord3D A, Coord3D B, Rigidbody& target, dbl theta );
    friend void XRotation( const Rigidbody& source, Rigidbody& result, dbl alpha );
    friend void EulerZYZ(const Rigidbody & source, Rigidbody & cible, dbl theta, dbl phi, dbl psi);

    friend class AtomSelection;


    // undocumented API
    // these functions are candidates for future official functions
    // Please don't use functions beginning by an undersocre '_'
    // they may be removed in future releases without justification

    /* empty section: good news !*/



}; // end class Rigidbody




} // end namespace PTools

#endif //RIGIDBODY_H

