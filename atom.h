

#ifndef ATOM_H
#define ATOM_H


#include <string>
#include <iostream>
#include "coord3d.h"


namespace PTools{

class Atom
{

private:
    std::string mType;  ///< CA, H, N, ...
    std::string mResidType; ///< LEU, ARG, ...
    std::string mChainId; ///< A, B, etc.
    uint mResidId; ///< residue number
    uint mAtomNumber; ///< atom number
    double mCharge; ///< charge of the atom
    coord3D mCoords; ///< cartesian coordinates of the atom


public:
    std::string GetType() const {return mType;};
    void SetType(std::string newtype) { mType = newtype;}

    double GetCharge() const {return mCharge;};
    void SetCharge(double ch) {mCharge=ch;};

    coord3D GetCoords() const {return mCoords;};
    void SetCoords(coord3D& coords) {mCoords=coords;};

    std::string GetChainId() const {return mChainId;};
    void SetChainId(std::string chainid) {mChainId=chainid;};

    uint GetResidId() const {return mResidId;};
    void SetResidId(uint id) {mResidId = id;};

    uint GetAtomNumber() const {return mAtomNumber;};
    void SetAtomNumber(uint atomnumber) {mAtomNumber=atomnumber;};

};

};

#endif


