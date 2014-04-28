#ifndef SCORPIONFORCEFIELD_H
#define SCORPIONFORCEFIELD_H

#include "attractforcefield.h"


namespace PTools{



class ScorpionForceField: public BaseAttractForceField
{
public:
    void InitParams(const std::string & paramsFileName);
    ScorpionForceField(std::string paramsFileName, dbl cutoff);
    dbl nonbon8_forces(AttractRigidbody& rec, AttractRigidbody& lig, AttractPairList & pairlist, std::vector<Coord3D>& forcerec, std::vector<Coord3D>& forcelig, bool print=false);

    virtual ~ScorpionForceField(){};
private:

    Vdouble m_epsilon, m_lambda, m_sigma ; 

    dbl m_lamb6[64][64];
    dbl m_epsij[64][64];
    dbl m_sig2[64][64];

    int m_ligRestraintIndex;

    dbl m_rstk;

    void setDummyTypeList(AttractRigidbody& lig){std::vector<uint> dummytypes; lig.setDummyTypes(dummytypes);}; //forcefield1 has no dummy type
};





} // namespace PTools



#endif //SCORPIONFORCEFIELD_H







