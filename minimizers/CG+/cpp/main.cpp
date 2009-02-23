#include "driver.h"



/* Subroutine */ int rosen(integer *n, doublereal *x, doublereal *f, doublereal *g)
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







int main()
{
CGstruct params(rosen, 2);
cgminimize(params);
}