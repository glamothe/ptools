#include "driver.h"
#include <stdio.h>
#include <iostream>



/* Subroutine */ int rosen(integer *n, const doublereal *x, doublereal *f, doublereal *g)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3;

/* Rosenbrock */
    /* Parameter adjustments */
    --g;
    --x;

    /* Function Body */
/* Computing 2nd power */
    d__2 = x[1];
/* Computing 2nd power */
    d__1 = x[2] - d__2 * d__2;
/* Computing 2nd power */
    d__3 = (float)1. - x[1];
    *f = d__1 * d__1 * (float)100. + d__3 * d__3;
/* Computing 2nd power */
    d__1 = x[1];
    g[1] = (x[2] - d__1 * d__1) * 200 * (x[1] * -2) - (1 - x[1]) * 2;
/* Computing 2nd power */
    d__1 = x[1];
    g[2] = (x[2] - d__1 * d__1) * 200;
    return 0;
} /* fcn_ */


template <class T>
void printvector(const std::vector<T>& v  )
{

for (int i=0; i<v.size(); i++)
{
   std::cout << v[i] << "  " ;
}

std::cout << std::endl;
}


class Rosen: public PTools::ForceField
{
public:
uint ProblemSize() {return 2;}


dbl Function(const Vdouble& x) { int n=2; dbl f; rosen(&n,&x[0],&f,&g[0]); return f;}

void Derivatives(const Vdouble& x, Vdouble& d) {d=g;}
std::vector<dbl> g;

Rosen() 
{g = Vdouble (2);}

void initMinimization() {};


};




int main()
{
Rosen r;

CGstruct params(&r);
params.maxiter = 100;
cgminimize(params);


printf("from main   :  niter = %i\n", params.niter);

for (int i=0; i<params.timemachine.size(); ++i)
   printvector(params.timemachine[i].x);


std::cout << params.timemachine.size() << "   " << params.niter << std::endl;
}