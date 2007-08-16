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

    /* don't forget the constructors if you add some private data ! */
//     std::vector<Atom> mAtoms; ///< vector of Atoms

    std::vector<Coord3D> mCoords; ///< vector of coordinates
    std::vector<Coord3D> mForces; ///< forces for each atom
    double mat44[4][4]; ///< rotation/tranlation matrix

 protected:
    std::vector<Atomproperty> mAtomProp; ///< array of atom properties


public:
    Rigidbody();  ///< basic constructor
    Rigidbody(std::string filename);
    Rigidbody(const Rigidbody& model); ///< copy

    virtual ~Rigidbody(){};

    /// Make a deep copy of one atom (the Atom extracted is then totally independent)
    virtual Atom CopyAtom(uint i) const ;

//     ///  get the Atom of index i (with direct access to modifications)
//     Atom& GetAtomReference(uint pos)
//     {
//         assert(pos<Size());
//         return mAtoms[pos];
//     };


	/// const version of GetAtom
	Atom GetAtom(uint pos) const
	{
        Atom at(mAtomProp[pos],mCoords[pos]);
	return at;
	}


        /// const version of GetAtom
        Atomproperty const & GetAtomProperty(uint pos) const
        {
          return mAtomProp[pos];
        }



    /// return number of atoms
    inline uint Size() const
    {
        return mAtomProp.size();
    };

    /// add an atom to the molecule (deep copy)
    void AddAtom(const Atomproperty& at, Coord3D co);

    /// add an atom to the molecule
    void AddAtom(const Atom& at);

    //returns the coordinates of atom i
    virtual Coord3D GetCoords(uint i) const
    {
        assert(i<Size());
        return mCoords[i];
    };

    /// define the coordinates of atom i
     void SetCoordsValid(uint i, const Coord3D& co)
     {
         mCoords[i] = co ;
     };

    /// return geometric center of all atoms
    Coord3D FindCenter() const;

    /// center the rigidbody to the Origin (0,0,0)
    void CenterToOrigin();

    void Translate(const Coord3D& tr); ///< Translate the whole object

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

    // operator =
    // TODO: one should check but this operator doesn't seem to be that necessary
    // (the compilers can use Rigidbody(const Rigidbody&) instead)
    //Rigidbody& operator= (const Rigidbody& rig);

    /// operator +
    Rigidbody operator+ (const Rigidbody& rig);

    void ABrotate(const Coord3D& A, const Coord3D& B, double theta); ///< rotation around (AB) axis.

    void ResetMatrix(); ///< reset Rotation/translation matrix to identity.
    void PrintMatrix() const; ///< print the rotation/translation matrix
    void ApplyMatrix(double mat[4][4]);
    Vdouble GetMatrix() const;

    /// in some cases atoms may be ignored 
    virtual bool isAtomActive(uint i) const {return true;}; 


    // here is all the friends declarations I'm not very proud of...
    friend void AttractEuler(const Rigidbody& source, Rigidbody& dest, double phi, double ssi, double rot);
    friend void mat44xrigid( const Rigidbody& source, Rigidbody& result, double mat[ 4 ][ 4 ] );
    friend void Translate(const Rigidbody& source, Rigidbody& target, const Coord3D& trans);
    friend void XRotation( const Rigidbody& source, Rigidbody& result, double alpha );
    friend void EulerZYZ(const Rigidbody & source, Rigidbody & cible, double theta, double phi, double psi);

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
    

    uint getAtomTypeNumber(uint i) const
    {
        return m_atomTypeNumber[i];
    };
    double getCharge(uint i) const
    {
        return m_charge[i];
    };

	virtual bool isAtomActive(uint i) const {return (m_atomTypeNumber[i]!=0);  };

    void resetForces()
	{
	 m_forces = std::vector<Coord3D> (this->Size() ) ;
        }


    std::vector<uint> m_activeAtoms; ///< list of active atoms (atoms that are taken into account for interaction)

    ///overload of the virtual function GetCoords
    virtual Coord3D GetCoords(uint i) const
    {
       Coord3D t = Rigidbody::GetCoords(i) ;
       for(uint nmode=0; nmode<m_lambdaMode.size(); nmode++)
       {
          t += m_modesArray[nmode][i] * m_lambdaMode[nmode];
       }

       return t;
    }

    ///adds a mode to the mode list
    void addMode(VCoord3D & mode) {m_modesArray.push_back(mode); m_lambdaMode.push_back(0.0); };
    void applyMode(uint modenumber, double lambda){
      assert(m_lambdaMode.size()> modenumber );
      m_lambdaMode[modenumber] = lambda ;
    }


private:
    std::vector<uint> m_atomTypeNumber ;
    std::vector<double> m_charge ;
    std::vector<Coord3D> m_forces ;
    std::vector<double> m_lambdaMode ;

    std::vector<VCoord3D> m_modesArray; ///< array of modes (normal modes)


friend class AttractForceField2;


};  //end class AttractRigid



// void applyMode(AttractRigidbody & src, AttractRigidbody& dest, const std::vector<Coord3D> & mode, double scalar);


} // end namespace PTools

#endif //RIGIDBODY_H

