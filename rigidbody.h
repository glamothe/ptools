#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <vector>
#include <cassert>


#include "coord3d.h"
#include "atom.h"
#include "basetypes.h"
// #include "geometry.h"

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

class Rigidbody
{

private:

    /* don't forget the constructors if you add some private data ! */
     std::vector<Atom> mAtoms; ///< vector of Atoms

    std::vector<Coord3D> mCoords; ///< vector of coordinates
    std::vector<Coord3D> mForces; ///< forces for each atom
    dbl mat44[4][4]; ///< rotation/tranlation matrix


    virtual void m_hookCoords(uint i, Coord3D & co) const ; ///< private hook to give a chance for class childs to change the coordinates before rotations and translations



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
        Atom at(mAtomProp[pos],GetCoords(pos));
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
        Coord3D c = mCoords[i];  //get the unrotated-untranslated coordinates
        m_hookCoords(i, c);  // gives a last chance to apply some modifications before translation/rotation (normal modes for example)

        Coord3D t = c ; //we need an intermediate object for mat44xVect
        mat44xVect(this->mat44, c , t);  //apply the 4x4 matrix to our internal coordinates filtered by the hook function

        return t;  //finally returns the final coordinates
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

    void AttractEulerRotate(dbl phi, dbl ssi, dbl rot); ///< Do an euler rotation (Attract convention)

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

    // operator =
    // TODO: one should check but this operator doesn't seem to be that necessary
    // (the compilers can use Rigidbody(const Rigidbody&) instead)
    //Rigidbody& operator= (const Rigidbody& rig);

    /// operator + : merge two Rigdibody by extending the first coordinates with the second coordinates.
    Rigidbody operator+ (const Rigidbody& rig);

    void ABrotate(const Coord3D& A, const Coord3D& B, dbl theta); ///< rotation around (AB) axis.

    void ResetMatrix(); ///< reset Rotation/translation matrix to identity.
    void PrintMatrix() const; ///< print the rotation/translation matrix
    void ApplyMatrix(dbl mat[4][4]);
    Vdouble GetMatrix() const;

    /// in some cases atoms may be ignored 
    virtual bool isAtomActive(uint i) const {return true;}; 



    // here is all the friends declarations I'm not very proud of...
//     friend void AttractEuler(const Rigidbody& source, Rigidbody& dest, dbl phi, dbl ssi, dbl rot);
    friend void mat44xrigid( const Rigidbody& source, Rigidbody& result, dbl mat[ 4 ][ 4 ] );
//     friend void Translate(const Rigidbody& source, Rigidbody& target, const Coord3D& trans);
    friend void XRotation( const Rigidbody& source, Rigidbody& result, dbl alpha );
    friend void EulerZYZ(const Rigidbody & source, Rigidbody & cible, dbl theta, dbl phi, dbl psi);

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
    explicit AttractRigidbody(const Rigidbody & rig) ; ///< initilize a new object from a regular Rigidbody object
    AttractRigidbody(){};
    virtual ~AttractRigidbody(){};

    uint getAtomTypeNumber(uint i) const
    {
        return m_atomTypeNumber[i];
    };
    dbl getCharge(uint i) const
    {
        return m_charge[i];
    };

	virtual bool isAtomActive(uint i) const {return (m_atomTypeNumber[i]!=0);  };

    void resetForces()
    {
       m_forces = std::vector<Coord3D> (this->Size() ) ;
    }

    void addForces(std::vector<Coord3D> forces)
    {
      for(uint i=0; i<forces.size(); i++)
         m_forces[i]+=forces[i];
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
     void addMode(VCoord3D & mode, dbl eigen);

     ///ask for a certain mode to be applied before getting the coordinates
     void applyMode(uint modenumber, dbl lambda){
         assert(m_lambdaMode.size()> modenumber );
         m_lambdaMode[modenumber] = lambda ;
     }


    ///get the eigenvalue of mode 'i'
    double getEigen(uint i);

    void setRotation(bool value) {hasrotation  = value;} //allow/disallow rotation
    void setTranslation(bool value) {hastranslation = value;} //allow/disallow translation


private:
    std::vector<uint> m_atomTypeNumber ;
    std::vector<dbl> m_charge ;
    std::vector<Coord3D> m_forces ;
    std::vector<dbl> m_lambdaMode ;
    std::vector<dbl> m_eigen; ///< array of eigenvalues

    std::vector<VCoord3D> m_modesArray; ///< array of modes (normal modes)

    bool hastranslation;
    bool hasrotation;

    void normalizeMode(VCoord3D & mode);

friend class BaseAttractForceField;
friend class AttractForceField2;
friend class AttractForceField1;


};  //end class AttractRigid



// void applyMode(AttractRigidbody & src, AttractRigidbody& dest, const std::vector<Coord3D> & mode, dbl scalar);


} // end namespace PTools

#endif //RIGIDBODY_H

