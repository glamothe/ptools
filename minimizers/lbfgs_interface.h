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
            Vdouble GetX() {return x;}; // Quel nom donner a cette fonction ? 



      private:

            ForceField& objToMinimize ;
            Vdouble x ; // position variables
            Vdouble g ; // gradient



} ;

}

#endif //#ifndef Lbfgs_H

