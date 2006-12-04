#ifndef RIGIDBODY_H
#define RIGIDBODY_H


#include <boost/shared_ptr.hpp>
#include <vector>

#include "coord3d.h"
#include "atom.h"

namespace PTools {

class Rigidbody
{

private:

    std::vector<Atom>     mArrayAtoms ;
    std::vector<Coord3D>  mcoords ;


public:

    inline Atom & GetAtom(uint i) {return mArrayAtoms[i] ;};
    inline uint size() {return mArrayAtoms.size();}


};



}




#endif //RIGIDBODY_H

