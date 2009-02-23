/* fcn.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Subroutine */ int fcn_(integer *n, doublereal *x, doublereal *f, doublereal *g)
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

#ifdef __cplusplus
	}
#endif
