#ifndef _CG_DRIVER_H_
#define _CG_DRIVER_H_

#include <vector>

#include "f2c_lite.h"
#include "../../../basetypes.h"


typedef double dbl;


typedef int (*fcn)(integer *n, doublereal *x, doublereal *f, doublereal *g);



/** \brief store a state of a function evaluation
This class represents a step of the minimization path
**/
struct FuncState
{
std::vector<dbl> x;
std::vector<dbl> g;
dbl f;
};


struct CGstruct
{
   doublereal d__[10000], f, g[10000];
   doublereal w[10000], x[10000], gold[10000];
   integer method, iprint[2];
   integer irest;
   fcn func;
   int n;
   int maxiter;
   int niter ; //effective number of iterations
   int neval; // number of evaluations

   std::vector<FuncState> timemachine;

   CGstruct(fcn function, int nn )
      : func(function), n(nn)
   {
     method = 2;
     iprint[0]=1;
     iprint[1]=0;
     irest = 1;
     maxiter = 100;
     niter = 0;
     neval = 0;
   }

};



extern "C"
{
int cgminimize(CGstruct& params);
}
#endif