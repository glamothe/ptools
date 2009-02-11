// $Id$
/****************************************************************************
 *   Copyright (C) 2006-2008   Adrien Saladin                               *
 *   adrien.saladin@gmail.com                                               *
 *   Copyright (C) 2008   Pierre Poulain                                    *
 *   Copyright (C) 2008   Sebastien Fiorucci                                *
 *   Copyright (C) 2008   Chantal Prevost                                   *
 *   Copyright (C) 2008   Martin Zacharias                                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 ***************************************************************************/


#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <vector>
#include <cassert>


#include "coord3d.h"
#include "atom.h"
#include "basetypes.h"
#include "coordsarray.h"

// #include "geometry.h"


namespace PTools
{


bool isBackbone(const std::string &  atomtype);

template <class T>
class AtomSelection_t; // forward declaration

//T: it's a Policy class for using modes or not
template<class T>
class Rigidbody_t:public CoordsArray<T>
{
private:

    std::vector<Atom> mAtoms; ///< vector of Atoms
    std::vector<Coord3D> mForces; ///< forces for each atom

    std::string _description; ///< some string to describe the molecule


//    bool isBackbone(const std::string &  atomtype); ///<return true if a given atomtype string matches a backbone atom name

protected:
    std::vector<Atomproperty> mAtomProp; ///< array of atom properties


public:

    typedef T modepolicy;

    typedef AtomSelection_t<Rigidbody_t<T> > selection;

    /// basic constructor
    Rigidbody_t<T>()
    {
        CoordsArray<T>::ResetMatrix();
    }

    /// constructor that loads a PDB file
    Rigidbody_t<T>(std::string filename)
    {
        ReadPDB(filename,*this);
        CoordsArray<T>::ResetMatrix();
    }



    /// copy constructor
    Rigidbody_t<T>(const Rigidbody_t& model):
            CoordsArray<T>(model)
    {
//this copy constructor is needed because dbl[4][4] is not
// automatically copied with the default copy constructor
//TODO: verifier si c'est toujours le cas ...


//     this->mAtoms = model.mAtoms;
        this->mForces = model.mForces;
//     this->_coords = model._coords;
        this->mAtomProp = model.mAtomProp;
        this->mAtoms = model.mAtoms;
        this-> _description = model._description;

    }



    virtual ~Rigidbody_t() {};

// return number of atoms in the rigidbody
//     uint Size() const {
//         return CoordsArray<T>::Size();
//     };


    void PrintMatrix() const {
        std::cout << CoordsArray<T>::PrintMatrix() << std::endl;
    }

    /// make a deep copy of one atom (the atom extracted is then totally independent)
    virtual Atom CopyAtom(uint i) const
    {
        Atom at(mAtomProp[i],GetCoords(i));
        return at;
    }

    /*    /// const version of GetAtom*/
    /*Atom GetAtom(uint pos) const
    {
        Coord3D co;
        CoordsArray::GetCoords(pos, co);
        Atom at(mAtomProp[pos], co );
        return at;
    }*/

    /// return atom properties
    Atomproperty const & GetAtomProperty(uint pos) const
    {
        return mAtomProp[pos];
    }

    /// define atom properties
    void SetAtomProperty(uint pos, const Atomproperty& atprop)
    {
        mAtomProp[pos] = atprop;
    }

    /// define atom pos
    void SetAtom(uint pos, const Atom& atom)
    {

        if (pos<0  || pos >= this->Size())
        {
            std::string message = "SetAtom: position ";
            message += pos;
            message += " is out of range";
            throw std::out_of_range(message);
        }
        Atomproperty atp(atom);
        Coord3D co(atom.GetCoords());
        SetAtomProperty(pos, atp);
        SetCoords(pos,co);
    }




    /// add an atom to the molecule (deep copy)
    void AddAtom(const Atomproperty& at, Coord3D co)
    {
        mAtomProp.push_back(at);
        CoordsArray<T>::AddCoord(co);
    }





    /// add an atom to the molecule
    void AddAtom(const Atom& at)
    {
        Atomproperty atp(at);
        Coord3D co = at.GetCoords();
        AddAtom(atp,co);
    }



    //returns the coordinates of atom i
    Coord3D GetCoords(uint i) const
    {
        assert(i<this->Size());
        Coord3D c;
        CoordsArray<T>::GetCoords(i,c) ;  //get the coordinates after translation/rotation

        return c;  //finally returns the final coordinates
    }


    void unsafeGetCoords(uint i, Coord3D& co)
    {
        CoordsArray<T>::unsafeGetCoords(i,co);
    }

    void syncCoords()
    {
        GetCoords(0);
    }

    /// define coordinates of atom i
    void SetCoords(uint i, const Coord3D& co)
    {
        assert(i<this->Size());
        CoordsArray<T>::SetCoords(i,co);
    }

    /// return geometric center of all atoms
    Coord3D FindCenter() const
    {
        Coord3D center(0.0,0.0,0.0);
        for (uint i=0; i< this->Size() ; i++)
        {
            center =  center + GetCoords(i);
        }
        return ( (1.0/(dbl)this->Size())*center);
    }


    /// center the rigidbody to the Origin (0,0,0)
    void CenterToOrigin()
    {
        Coord3D c = FindCenter();
        this->Translate(Coord3D()-c);
    }

    /// translate the whole object
//     void Translate(const Coord3D& tr);

    /// apply a 4x4 matrix
    void ApplyMatrix(const Matrix & mat)
    {

        dbl mat44[4][4];
        for (uint i=0; i<4;i++)
            for (uint j=0;j<4;j++)
                mat44[i][j]=mat(i,j);
        CoordsArray<T>::MatrixMultiply(mat44);
    }



    /// get the 4x4 matrix
    Matrix GetMatrix()
    {
        return CoordsArray<T>::GetMatrix();
    }


    /// returns radius of gyration
    dbl RadiusGyration()
    {
        Coord3D c = this->FindCenter();
        dbl r=0.0;
        for (uint i=0; i< this->Size(); i++)
        {
            r += Norm2( c - this->GetCoords(i) );
        }

        dbl result = sqrt( r/ (double) this->Size() );
        return result;
    }


    /// returns the radius of a Rigidbody (max distance from center)
    dbl Radius()
    {
        Coord3D center = this->FindCenter();
        uint size = this->Size();
        dbl radius = 0.0;
        for (uint i=0; i < size; i++)
        {
            dbl rad=Norm(center - this->GetCoords(i));
            if (radius < rad) {
                radius=rad;
            }
        }
        return radius;
    }



    /// converts rigidbody to classical PDB-like string
    std::string PrintPDB() const
    {
        uint size=this->Size();
        std::string output;
        for (uint i=0; i < size ; i++)
        {
            output = output + mAtoms[i].ToPdbString();
        }
        return(std::string) output;
    }





    /// selection : complete
    selection SelectAllAtoms() const
    {
        selection newsel;
        newsel.SetRigid(*this);
        for (uint i=0; i < this->Size(); i++)
        {
            newsel.AddAtomIndex(i);
        }


        return newsel;

    }





    /// selection : by atom type
    selection SelectAtomType(std::string atomtype)
    {
        selection newsel;
        newsel.SetRigid(*this);

        for (uint i=0; i<this->Size(); i++)
        {
            if ( mAtomProp[i].GetType()==atomtype)
                newsel.AddAtomIndex(i);
        }

        return newsel;
    }


    /// selection by residue type
    selection SelectResidType(std::string residtype)
    {
        selection newsel;
        newsel.SetRigid(*this);

        for (uint i=0; i<this->Size(); i++)
        {
            if (mAtomProp[i].GetResidType()==residtype)
                newsel.AddAtomIndex(i);
        }
        return newsel;
    }

    /// selection by chain ID
    selection SelectChainId(std::string chainid)
    {
        selection newsel;
        newsel.SetRigid(*this);
        for (uint i=0; i<this->Size(); i++)
        {
            if (mAtomProp[i].GetChainId()==chainid)
                newsel.AddAtomIndex(i);
        }
        return newsel;
    }



    /// selection by range of residue ID
    selection SelectResRange(uint start, uint stop)
    {
        selection newsel;
        newsel.SetRigid(*this);

        for (uint i=0; i < this->Size(); i++)
        {
            Atomproperty& atp ( mAtomProp[i] );
            if (atp.GetResidId() >=start && atp.GetResidId() <= stop) newsel.AddAtomIndex(i);
        }
        return newsel;
    }



    /// selection shortcut for C-alpha
    selection CA()
    {
        return SelectAtomType("CA");
    }


    /// selection of backbone atoms:
    selection Backbone()
    {
        selection newsel;
        newsel.SetRigid(*this);

        for (uint i=0; i<this->Size(); i++)
        {
            if (isBackbone(CopyAtom(i).GetType()) )
            {
                newsel.AddAtomIndex(i);
            }

        }
        return newsel;
    }




    /// operator + : merge two Rigdibody by extending the first coordinates with the second coordinates.
    Rigidbody_t<T> operator+ (const Rigidbody_t<T>& rig)
    {
        Rigidbody_t<T> rigFinal(*this);
        for (uint i=0; i< rig.Size() ; i++) {
            rigFinal.AddCoord(rig.GetCoords(i));
            rigFinal.mAtomProp.push_back(rig.mAtomProp[i]);
        }
        return rigFinal;
    }



    /// rotation around (AB) axis.
    void ABrotate(const Coord3D& A, const Coord3D& B, dbl theta)
    {
        freeABrotate(A,B, *this, theta);
    }


    /// in some cases atoms may be ignored
    virtual bool isAtomActive(uint i) const {
        return true;
    };

    /// set a description for the object (ex: "mutant A192G")
    void setDescription(const std::string & descr) {
        _description = descr;
    };
    /// return the object name/description
    std::string getDescription() {
        return _description;
    };

//     void AttractEulerRotate(dbl phi, dbl ssi, dbl rot);

    //friends
//     friend void ABrotate( Coord3D A, Coord3D B, Rigidbody& target, dbl theta );
//     friend void XRotation( const Rigidbody& source, Rigidbody& result, dbl alpha );
//     friend void EulerZYZ(const Rigidbody & source, Rigidbody & cible, dbl theta, dbl phi, dbl psi);

    friend class AtomSelection_t<T>;

    CoordsArray<T> ToCoordsArray() const {
        return static_cast<CoordsArray<T> > (*this);
    }
    // undocumented API
    // these functions are candidates for future official functions
    // Please don't use functions beginning by an undersocre '_'
    // they may be removed in future releases without justification

    /* empty section: good news !*/



}; // end class Rigidbody



typedef Rigidbody_t<NoMode> Rigidbody;

// now using public inheritance from CoordsArray
// void Rigidbody::Translate(const Coord3D& tr)
// {
//     CoordsArray::Translate(tr);
// }
//
// void Rigidbody::AttractEulerRotate(dbl phi, dbl ssi, dbl rot)
// {
//    CoordsArray::AttractEulerRotate(phi, ssi, rot);
// }



} // end namespace PTools

#endif //RIGIDBODY_H

