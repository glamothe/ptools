#ifndef LBFGS_H
#define LBFGS_H

#include "../basetypes.h"
#include "../forcefield.h"


#include "lbfgs_wrapper/lbfgs.h"




namespace PTools
{



// new version, from sdrive.c
// no bounds !!
class Lbfgs
{
      public:
            Lbfgs( ForceField& toMinim);
            ~Lbfgs(); 
            void minimize(int maxiter);



      private:

            ForceField& objToMinimize ; 



} ;

}

#endif //#ifndef Lbfgs_H

