

#ifndef ATOM_H
#define ATOM_H


#include <string>
#include <iostream>
#include "coord3d.h"

namespace PTools{

class Rigidbody;


class Atomproperty {
private:
    std::string mAtomType;  ///< CA, N, HN1, ...
    std::string mAtomElement; /// C, N, H, O, etc.
    std::string mResidType; ///< LEU, ARG, ...
    std::string mChainId; ///< A, B, etc.
    uint mResidId; ///< residue number
    uint mAtomId; ///< atom number
    double mAtomCharge; ///< charge of the atom
    std::string mExtra; ///< extra data

public:
    Atomproperty(){ mAtomType="X", ///< default constructor 
						   mAtomElement="X", 
						   mResidType="XXX", 
						   mChainId="X", 
						   mResidId=1,
						   mAtomId=1,
						   mAtomCharge=0.0;};

    std::string GetType() const {return mAtomType;}; ///< return atom type
    void SetType(std::string newtype) { mAtomType = newtype;}; ///< define atom type

    std::string GetResidType() const {return mResidType;}; ///< return residue type
    void SetResidType(std::string residtype){mResidType=residtype;}; ///< define residue type

    inline double GetAtomCharge() const {return mAtomCharge;}; ///< return atom charge
    inline void SetAtomCharge(double ch) {mAtomCharge=ch;}; ///< define atom charge

    inline std::string GetChainId() const {return mChainId;}; ///< return chain ID
    inline void SetChainId(std::string chainid) {mChainId=chainid;}; ///< define chain ID

    inline uint GetResidId() const {return mResidId;}; ///< return residue ID
    inline void SetResidId(uint id) {mResidId = id;}; ///< define residue ID

    inline uint GetAtomId() const {return mAtomId;}; ///< return atom ID
    inline void SetAtomId(uint atomnumber) {mAtomId=atomnumber;}; ///< define atom ID

    inline void SetExtra(std::string extra){mExtra=extra;}; ///< set the extra data field
    inline std::string GetExtra(){return mExtra;}; ///< get the extra data field

};


class Atom : public Atomproperty
{

private:
    Coord3D mCoords; ///< Atom cartesian coordinates

public:

    Atom(Atomproperty ap, Coord3D co)
            : Atomproperty(ap), mCoords(co) {};
    Coord3D GetCoords() const; ///< return atom coordinates
    inline void SetCoords(Coord3D& coords) {mCoords=coords;}; ///< define atom coordinates
    std::string ToString() const; ///< convert atom properties and coordinates to std::string
    std::string ToPdbString() const ; ///< converts atom to classical PDB-like string

    void Translate(const Coord3D& tr); ///< Translation of an atom

};



inline double Dist(const Atom& at1, const Atom& at2) ///< distance between two atoms
{
   return Norm(at1.GetCoords()-at2.GetCoords());
}


inline double Dist2(const Atom& at1, const Atom& at2) ///< distance**2 between two atoms
{
   return Norm2(at1.GetCoords()-at2.GetCoords());
}


};

#endif


