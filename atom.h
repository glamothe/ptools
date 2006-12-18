

#ifndef ATOM_H
#define ATOM_H


#include <string>
#include <iostream>
#include "coord3d.h"



namespace PTools{

class Rigidbody;


class Atom
{

private:
    Rigidbody* mOwner;
    bool hasOwner;
    uint mIndex; ///< internal index in Rigidbody object

    std::string mType;  ///< CA, H, N, ...
    std::string mResidType; ///< LEU, ARG, ...
    std::string mChainId; ///< A, B, etc.
    uint mResidId; ///< residue number
    uint mAtomNumber; ///< atom number
    double mCharge; ///< charge of the atom
    coord3D mCoords; ///< cartesian coordinates of the atom


public:

    Atom(){hasOwner=false; mOwner=0; };

    std::string GetType() const {return mType;};
    void SetType(std::string newtype) { mType = newtype;}

    std::string GetResidType() const {return mResidType;};
    void SetResidType(std::string residtype){mResidType=residtype;};



    inline double GetCharge() const {return mCharge;};
    inline void SetCharge(double ch) {mCharge=ch;};

    coord3D GetCoords() const;
    inline void SetCoords(coord3D& coords) {mCoords=coords;};

    inline std::string GetChainId() const {return mChainId;};
    inline void SetChainId(std::string chainid) {mChainId=chainid;};

    inline uint GetResidId() const {return mResidId;};
    inline void SetResidId(uint id) {mResidId = id;};

    inline uint GetAtomNumber() const {return mAtomNumber;};
    inline void SetAtomNumber(uint atomnumber) {mAtomNumber=atomnumber;};

    inline void SetOwner(Rigidbody* owner, uint index)
        {hasOwner=true; mOwner=owner;mIndex=index;};

};

};

#endif


