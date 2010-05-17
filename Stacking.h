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
    Rigidbody axisPyrimidines(const Rigidbody& pyr, const Coord3D& centerBase1, const Coord3D& centerBase2)const;
    

public:
    Stacking();
    ~Stacking();
    Rigidbody axis(const Rigidbody& bp1, const Rigidbody& bp2)const;
};

}

#endif	/* _STACKING_H */

