#include "pairlist.h"
#include "ptools.h"

#include <vector>

namespace pyplusplus{ namespace aliases{
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    typedef std::vector< double > Vdouble;
    typedef std::vector< uint > Vuint ;
    typedef std::vector< int > Vint ;
    } } //pyplusplus::aliases




namespace  py_details{
    inline void instantiate(){
        using namespace PTools;
        sizeof( PairList );
    }
}

