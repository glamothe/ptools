/* timer.f -- translated by f2c (version 20061008).
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

/* Subroutine */ int timer_(doublereal *ttime)
{
    static real temp;
    extern doublereal etime_(real *);
    static real tarray[2];

/*     ********* */

/*     Subroutine timer */

/*     This subroutine is used to determine user time. In a typical */
/*     application, the user time for a code segment requires calls */
/*     to subroutine timer to determine the initial and final time. */

/*     The subroutine statement is */

/*       subroutine timer(ttime) */

/*     where */

/*       ttime is an output variable which specifies the user time. */

/*     Argonne National Laboratory and University of Minnesota. */
/*     MINPACK-2 Project. */

/*     Modified October 1990 by Brett M. Averick. */

/*     ********** */
/*     The first element of the array tarray specifies user time */
    temp = etime_(tarray);
    *ttime = (doublereal) tarray[0];
    return 0;
} /* timer_ */

#ifdef __cplusplus
	}
#endif
