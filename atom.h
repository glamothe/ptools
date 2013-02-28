#ifndef ATOM_H
#define ATOM_H

#include "basetypes.h"

#include <string>
#include <iostream>
#include "coord3d.h"

namespace PTools{


struct Atomproperty {
    uint atomId; ///< atom number (PDB field: atom serial number)
    std::string atomType;  ///< CA, N, HN1, (PDB field: atom name)
    char altLoc; ///< alternate location indicator
    std::string residType; ///< LEU, ARG, ... (PDB field: residue name)
    char chainId; ///< A, B, etc. (PDB field: chain identifier)
    uint residId; ///< residue number (PDB field: Residue sequence number)    
    
    std::string atomElement; ///< C, N, H, O, etc.
    
    dbl atomCharge; ///< charge of the atom
    std::string extra; ///< extra data

    /// default constructor
    Atomproperty()
    {
        atomType="X";
        atomElement="X";
        residType="XXX";
        chainId='X';
        residId=1;
        atomId=1;
	altLoc = ' ';
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


