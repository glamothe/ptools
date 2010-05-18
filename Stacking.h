/* 
 * File:   Stacking.h
 * Author: boyer
 *
 * Created on May 15, 2010, 5:12 AM
 */

#ifndef _STACKING_H
#define	_STACKING_H

#include "rigidbody.h"

namespace PTools {


class Stacking{

private:
    //private data


    //private methods
    Rigidbody axisPyrimidine(const Rigidbody& pyr, const Coord3D& centerBase1, const Coord3D& centerBase2)const;
    Rigidbody axisPurine(const Rigidbody& pur, const Coord3D& center )const;

public:
    Stacking();
    ~Stacking();
    Rigidbody axis(const Rigidbody& bp1, const Rigidbody& bp2)const;
    bool isStacked(const Rigidbody& axe1, const Rigidbody& axe2,double angleThreshold,double overlapThreshold)const;
};

}

#endif	/* _STACKING_H */

