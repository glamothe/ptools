/* main.f -- translated by f2c (version 20061008).
removed libf2c dependency 
don't blame me for the goto !
*/



#include "driver.h"
#include <stdio.h>


extern "C" {


/* Table of constant values */

static integer c__1 = 1;


/*       -------------------------------------------------------------------- */
/*       Main program for running the conjugate gradient methods described in */
/*       the paper: */

/*       Gilbert, J.C. and Nocedal, J. (1992). "Global Convergence Properties */
/*       of Conjugate Gradient Methods", SIAM Journal on Optimization, Vol. 2, */
/*       pp. 21-42. */

/*       A web-based Server which solves unconstrained nonlinear optimization */
/*       problems using this Conjugate Gradient code can be found at: */

/*       http://www-neos.mcs.anl.gov/neos/solvers/UCO:CGPLUS/ */

/*       Written by G. Liu, J. Nocedal and R. Waltz */
/*       October 1998 */
/*       -------------------------------------------------------------------- */



/* Main program */ int cgminimize(CGstruct& params)
{
    /* Initialized data */

    static freal one = (float)1.;


    /* System generated locals */
    integer i__1;
    doublereal d__1;



    /* Local variables */
    
    static integer i__;
    
//     extern /* Subroutine */ int fcn_(integer *, doublereal *, doublereal *, doublereal *);
    static doublereal eps, tlev;

    extern /* Subroutine */ int cgfam_(integer *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, integer *, doublereal *,
	     doublereal *, integer *, integer *, integer *, logical *);
    static integer iflag, icall;

    
    static logical finish;
    

    int ncall=0;
    int niter = 0;

    finish = FALSE_;

    int n = params.n;
    doublereal *x = &params.x[0];
    integer *iprint = params.iprint;
    integer method = params.method;
    doublereal& f = params.f ;
    doublereal* g = &params.g[0];

    



/* Check for correct dimension value n */

    if (n < 0) {
	iflag = -3;
        printf("Error: negative N value\n");


	goto L50;
    }
    if (n > 10000) {
	iflag = -3;
        printf("Error: N too large, increase parameter ndim\n");
	goto L50;
    }


/* Print parameters */

    if (iprint[0] >= 0) {
        printf("\n Conjugate Gradient Minimization Routine\n");
        printf(" n\t =\t%i \n method\t =\t%i \n irest\t =\t%i \n", n , method, params.irest  );

    }
    icall = 0;

/* This is the convergence constant */

    eps = 1e-5;
/* IFLAG=0 indicates an initial entry to program */
    iflag = 0;


L20:

/* Calculate the function and gradient values here */

/* Rosenbrock test function */
//     func(&n, x, &f, g);

    f = params.p_forcefield->Function(params.x);
    params.p_forcefield->Derivatives(params.x, params.g);



    params.neval++;
L30:

/* Call the main optimization code */

    cgfam_(&n, x, &f, g, params.d__, params.gold, iprint, &eps, params.w, &iflag, &params.irest, &method, 
	    &finish);

/*       IFLAG= */
/*              0 : successful termination */
/*              1 : return to evaluate F and G */
/*              2 : return with a new iterate, try termination test */
/*             -i : error */

    if (iflag <= 0 || icall > 10000) {
	goto L50;
    }
    if (iflag == 1) {
	++icall;
	goto L20;
    }
    if (iflag == 2) {

/* Termination Test.  The user may replace it by some other test. However, */
/* the parameter 'FINISH' must be set to 'TRUE' when the test is satisfied. */
        params.niter++;

        //store X, G and f
        FuncState fs;
        for (int i=0; i<params.n; ++i)
         {
            fs.x.push_back(x[i]);
            fs.g.push_back(g[i]);
         }
        fs.f = f;
        params.timemachine.push_back(fs);


        if (params.niter > params.maxiter)
        {
            finish = TRUE_;
        }

	tlev = eps * (one + abs(f));
	i__ = 0;
L40:
	++i__;
	if (i__ > n) {
	    finish = TRUE_;
	    goto L30;
	}
	if ((d__1 = g[i__ - 1], fabs(d__1)) > tlev) {
	    goto L30;
	} else {
	    goto L40;
	}
    }
L50:


/* Code has terminated; print final results */

    if (iprint[0] >= 0 && iflag >= 0) {
    printf(" f(x) =  %.10e \n",f);
    }


    return 0;
} /* MAIN__ */


	}

