#ifndef _CG_DRIVER_H_
#define _CG_DRIVER_H_

#include <f2c.h>

typedef int (*fcn)(integer *n, doublereal *x, doublereal *f, doublereal *g);


struct CGstruct
{
   doublereal d__[10000], f, g[10000];
   doublereal w[10000], x[10000], gold[10000];
   integer method, iprint[2];
   integer irest;
   fcn func;
   int n;
   int maxiter;

   CGstruct(fcn function, int nn )
      : func(function), n(nn)
   {
     method = 2;
     iprint[0]=1;
     iprint[1]=0;
     irest = 1;
     maxiter = 100;
   }

};



extern "C"
{
int cgminimize(CGstruct& params);
}
#endif