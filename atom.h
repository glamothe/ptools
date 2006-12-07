

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
    
    const std::string GetType() const {return mType;};
    void SetType(std::string newtype) { mType = newtype;}
    const double GetCharge() const {return mCharge;};
    void SetCharge(double ch) {mCharge=ch;};
    coord3D GetCoords() const {return mCoords;};
    void SetCoords(coord3D& coords) {mCoords=coords;};
    


};

};

 #endif
 
 
