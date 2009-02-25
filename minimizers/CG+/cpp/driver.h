#ifndef _CG_DRIVER_H_
#define _CG_DRIVER_H_

#include <vector>

#include "f2c_lite.h"
#include "../../../basetypes.h"

#include "../../../forcefield.h"

typedef double dbl;


typedef int (*fcn)(integer *n, doublereal *x, doublereal *f, doublereal *g);

using PTools::ForceField;

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
   doublereal d__[10000], f;
   doublereal w[10000], gold[10000];
   std::vector<dbl> x;
   std::vector<dbl> g;

   integer method, iprint[2];
   integer irest;

   ForceField* p_forcefield;

   int n;
   int maxiter;
   int niter ; //effective number of iterations
   int neval; // number of evaluations

   std::vector<FuncState> timemachine;

   CGstruct(ForceField* p_ff)
      : p_forcefield(p_ff)
   {

     n = p_forcefield->ProblemSize();
     method = 2;
     iprint[0]=1;
     iprint[1]=0;
     irest = 1;
     maxiter = 100;
     niter = 0;
     neval = 0;
     for (int i=0; i<10000; i++) x[i]=0.0;

     x = std::vector<doublereal> (n);
     g = std::vector<doublereal> (n);
   }




};



extern "C"
{
int cgminimize(CGstruct& params);
}
#endif