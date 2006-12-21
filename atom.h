

#ifndef ATOM_H
#define ATOM_H


#include <string>
#include <iostream>
#include "coord3d.h"



namespace PTools{

class Rigidbody;


class Atomproperty
{

private:
    std::string mAtomType;  ///< CA, N, HN1, ...
    std::string mAtomElement; /// C, N, H, O, etc.
    std::string mResidType; ///< LEU, ARG, ...
    std::string mChainId; ///< A, B, etc.
    uint mResidId; ///< residue number
    uint mAtomId; ///< atom number
    double mAtomCharge; ///< charge of the atom



public:

    Atomproperty(){};

    std::string GetType() const {return mAtomType;};
    void SetType(std::string newtype) { mAtomType = newtype;}

    std::string GetResidType() const {return mResidType;};
    void SetResidType(std::string residtype){mResidType=residtype;};

    inline double GetAtomCharge() const {return mAtomCharge;};
    inline void SetAtomCharge(double ch) {mAtomCharge=ch;};



    inline std::string GetChainId() const {return mChainId;};
    inline void SetChainId(std::string chainid) {mChainId=chainid;};

    inline uint GetResidId() const {return mResidId;};
    inline void SetResidId(uint id) {mResidId = id;};

    inline uint GetAtomId() const {return mAtomId;};
    inline void SetAtomId(uint atomnumber) {mAtomId=atomnumber;};


};


class Atom : public Atomproperty
{

private:
    Coord3D mCoords; ///< Atom cartesian coordinates


public:

    Atom(Atomproperty ap, Coord3D co)
            : Atomproperty(ap), mCoords(co) {};


    Coord3D GetCoords() const;
    inline void SetCoords(Coord3D& coords) {mCoords=coords;};


};



inline double Dist(const Atom& at1, const Atom& at2)
{
   return Norm(at1.GetCoords()-at2.GetCoords());
}


inline double Dist2(const Atom& at1, const Atom& at2)
{
   return Norm2(at1.GetCoords()-at2.GetCoords());
}



};

#endif


