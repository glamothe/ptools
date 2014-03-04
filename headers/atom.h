#ifndef ATOM_H
#define ATOM_H

#include "basetypes.h"

#include <string>
#include <iostream>
#include "coord3d.h"

namespace PTools{


struct Atomproperty {
    std::string atomType;  ///< CA, N, HN1, ...
    void setAtomType(const std::string &ty); ///< attempts to set a pdbAtomType from a string

    std::string _pdbAtomType; ///<  PDB atomType field is not really user-friendly ( " CA ", ...) 

    std::string atomElement; ///< C, N, H, O, etc.
    std::string residType; ///< LEU, ARG, ...
    std::string chainId; ///< A, B, etc.
    int residId; ///< residue number
    int atomId; ///< atom number
    dbl atomCharge; ///< charge of the atom
    std::string extra; ///< extra data

    /// default constructor
    Atomproperty()
    {
        atomType="X";
        atomElement="X";
        _pdbAtomType="?";
        residType="XXX";
        chainId="X";
        residId=1;
        atomId=1;
        atomCharge=0.0;
    };


};


struct Atom : public Atomproperty
{

public:
    Coord3D coords; ///< Atom cartesian coordinates


    Atom() {};
    Atom(Atomproperty ap, Coord3D co)
            : Atomproperty(ap), coords(co) {};
    
    /// convert atom (properties and coordinates) to std::string
    std::string ToString() const;

    /// convert atom (properties and coordinates) to classical PDB-like string
    std::string ToPdbString() const ;

    /// translation of an atom
    void Translate(const Coord3D& tr);

};


/// distance between two atoms
inline dbl Dist(const Atom& at1, const Atom& at2)
{
    return Norm(at1.coords-at2.coords);
}

/// distance**2 between two atoms
inline dbl Dist2(const Atom& at1, const Atom& at2)
{
    return Norm2(at1.coords - at2.coords );
}


}

#endif


