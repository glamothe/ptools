#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <vector>
#include <cassert>


#include "coord3d.h"
#include "atom.h"
#include "basetypes.h"

namespace PTools
{


class AtomSelection; // forward declaration

class Rigidbody
{

private:
    std::vector<Atom> mAtoms; ///< vector of Atoms
    std::vector<Coord3D> mForces; ///< forces for each atom
    double mat44[4][4]; ///< rotation/tranlation matrix



 protected:





public:
    Rigidbody();  ///< basic constructor
    Rigidbody(std::string filename);
    Rigidbody(const Rigidbody& model); ///< copy

    virtual ~Rigidbody(){};

    /// Make a deep copy of one atom (the Atom extracted is then totally independent)
    Atom CopyAtom(uint i) const ;

    ///  get the Atom of index i (with direct access to modifications)
    Atom& GetAtomReference(uint pos)
    {
        assert(pos>=0);
        assert(pos<Size());
        return mAtoms[pos];
    };

	/// const version of GetAtom
	const Atom& GetAtom(uint pos) const
	{
	return mAtoms[pos];
	}

    /// synonym of GetAtom
    const Atom& operator[](uint pos) const
    {
        return GetAtom(pos);
    };

    /// return number of atoms
    inline uint Size() const
    {
        return mAtoms.size();
    };

    /// add an atom to the molecule (deep copy)
    void AddAtom(const Atomproperty& at, Coord3D co);

    /// add an atom to the molecule
    void AddAtom(const Atom& at);

    /// returns the coordinates of atom i
    Coord3D GetCoords(uint i) const
    {
        assert(i>=0);
        assert(i<Size());
        return mAtoms[i].GetCoords();
    };

    /// define the coordinates of atom i
    void SetCoords(uint i, Coord3D& co)
    {
        mAtoms[i].SetCoords(co);
    };

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

    /// in some cases atoms may be ignored 
    virtual bool isAtomActive(uint i) const {return true;}; 


    friend void AttractEuler(const Rigidbody& source, Rigidbody& dest, double phi, double ssi, double rot);
    friend void mat44xrigid( const Rigidbody& source, Rigidbody& result, double mat[ 4 ][ 4 ] );
    friend void Translate(const Rigidbody& source, Rigidbody& target, const Coord3D& trans);

    friend class AtomSelection;

}; // end class Rigidbody




/*! \brief Rigidbody specific to Attract's reduced model
*
*   this objects stores "atom type number" property
*   which is required for Attract's forcefields and for
*   some attract's specific pairlist
*/
class AttractRigidbody: public Rigidbody
{
public:
    AttractRigidbody(const Rigidbody & rig) ; ///< initilize a new object from a regular Rigidbody object

    uint getAtomTypeNumber(uint i)
    {
        return m_atomTypeNumber[i];
    };
    double getCharge(uint i)
    {
        return m_charge[i];
    };

	virtual bool isAtomActive(uint i) const {return (m_atomTypeNumber[i]!=0);  };

    void resetForces()
	{
	 m_forces = std::vector<Coord3D> (this->Size() ) ;
        }
    std::vector<uint> m_activeAtoms;



private:
    std::vector<uint> m_atomTypeNumber ;
    std::vector<double> m_charge ;
    std::vector<Coord3D> m_forces ;



friend class AttractForceField2;


};  //end class AttractRigid






} // end namespace PTools

#endif //RIGIDBODY_H

