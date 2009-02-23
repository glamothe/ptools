/* cgfam.f -- translated by f2c (version 20061008).
   You must link the resulting object file with libf2c:
	on Microsoft Windows system, link with libf2c.lib;
	on Linux or Unix systems, link with .../path/to/libf2c.a -lm
	or, if you install libf2c.a in a standard place, with -lf2c -lm
	-- in that order, at the end of the command line, as in
		cc *.o -lf2c -lm
	Source for libf2c is in /netlib/f2c/libf2c.zip, e.g.,

		http://www.netlib.org/f2c/libf2c.zip
*/

#include "cgfam.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "f2c.h"

/* Common Block Declarations */

struct cgdd_1_ {
    integer mp, lp;
};

#define cgdd_1 (*(struct cgdd_1_ *) &cgdd_)

struct {
    integer iter, nfun;
} runinf_;

#define runinf_1 runinf_

/* Initialized data */

struct {
    integer e_1[2];
    } cgdd_ = { 6, 6 };


/* Table of constant values */

static integer c__1 = 1;
static integer c__9 = 9;


/*     -------------------------------------------------------------------- */
/*     Conjugate Gradient methods for solving unconstrained nonlinear */
/*     optimization problems, as described in the paper: */

/*     Gilbert, J.C. and Nocedal, J. (1992). "Global Convergence Properties */
/*     of Conjugate Gradient Methods", SIAM Journal on Optimization, Vol. 2, */
/*     pp. 21-42. */

/*     A web-based Server which solves unconstrained nonlinear optimization */
/*     problems using this Conjugate Gradient code can be found at: */

/*       http://www-neos.mcs.anl.gov/neos/solvers/UCO:CGPLUS/ */

/*     -------------------------------------------------------------------- */

/* Subroutine */ int cgfam_(integer *n, doublereal *x, doublereal *f, 
	doublereal *g, doublereal *d__, doublereal *gold, integer *iprint, 
	doublereal *eps, doublereal *w, integer *iflag, integer *irest, 
	integer *method, logical *finish)
{
    /* Initialized data */

    static doublereal one = 1.;
    static doublereal zero = 0.;


    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Builtin functions */
    double sqrt(doublereal);
    integer s_wsfe(cilist *), e_wsfe(), s_wsle(cilist *), do_lio(integer *, 
	    integer *, char *, ftnlen), e_wsle(), do_fio(integer *, char *, 
	    ftnlen);

    /* Local variables */
    static integer i__;
    static doublereal dg, gg;
    static integer im;
    static doublereal dg0, dg1, gg0;
    static logical new__;
    static doublereal stp, stp1;
    extern /* Subroutine */ int cgbd_(integer *, integer *, integer *, 
	    doublereal *, integer *, doublereal *, doublereal *, doublereal *,
	     doublereal *, logical *, integer *, integer *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal beta;
    static integer ides, ndes;
    extern doublereal ddot_(integer *, doublereal *, integer *, doublereal *, 
	    integer *);
    static integer info, nfev;
    static doublereal ftol, gtol, xtol;
    static integer nrst;
    static doublereal dgold, gnorm, dgout, betafr, betapr;
    static integer maxfev;
    extern /* Subroutine */ int cvsmod_(integer *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, integer *,
	     integer *, integer *, doublereal *, doublereal *, doublereal *);
    static doublereal stpmin, stpmax;



/* Subroutine parameters */


/*     N      =  NUMBER OF VARIABLES */
/*     X      =  ITERATE */
/*     F      =  FUNCTION VALUE */
/*     G      =  GRADIENT VALUE */
/*     GOLD   =  PREVIOUS GRADIENT VALUE */
/*     IPRINT =  FREQUENCY AND TYPE OF PRINTING */
/*               IPRINT(1) < 0 : NO OUTPUT IS GENERATED */
/*               IPRINT(1) = 0 : OUTPUT ONLY AT FIRST AND LAST ITERATION */
/*               IPRINT(1) > 0 : OUTPUT EVERY IPRINT(1) ITERATIONS */
/*               IPRINT(2)     : SPECIFIES THE TYPE OF OUTPUT GENERATED; */
/*                               THE LARGER THE VALUE (BETWEEN 0 AND 3), */
/*                               THE MORE INFORMATION */
/*               IPRINT(2) = 0 : NO ADDITIONAL INFORMATION PRINTED */
/* 		IPRINT(2) = 1 : INITIAL X AND GRADIENT VECTORS PRINTED */
/* 		IPRINT(2) = 2 : X VECTOR PRINTED EVERY ITERATION */
/* 		IPRINT(2) = 3 : X VECTOR AND GRADIENT VECTOR PRINTED */
/* 				EVERY ITERATION */
/*     EPS    =  CONVERGENCE CONSTANT */
/*     W      =  WORKING ARRAY */
/*     IFLAG  =  CONTROLS TERMINATION OF CODE, AND RETURN TO MAIN */
/*               PROGRAM TO EVALUATE FUNCTION AND GRADIENT */
/*               IFLAG = -3 : IMPROPER INPUT PARAMETERS */
/*               IFLAG = -2 : DESCENT WAS NOT OBTAINED */
/*               IFLAG = -1 : LINE SEARCH FAILURE */
/*               IFLAG =  0 : INITIAL ENTRY OR */
/*                            SUCCESSFUL TERMINATION WITHOUT ERROR */
/*               IFLAG =  1 : INDICATES A RE-ENTRY WITH NEW FUNCTION VALUES */
/*               IFLAG =  2 : INDICATES A RE-ENTRY WITH A NEW ITERATE */
/*     IREST  =  0 (NO RESTARTS); 1 (RESTART EVERY N STEPS) */
/*     METHOD =  1 : FLETCHER-REEVES */
/*               2 : POLAK-RIBIERE */
/*               3 : POSITIVE POLAK-RIBIERE ( BETA=MAX{BETA,0} ) */

/* Local variables */


/*     THE FOLLOWING PARAMETERS ARE PLACED IN COMMON BLOCKS SO THEY */
/*     CAN BE EASILY ACCESSED ANYWHERE IN THE CODE */

/*     MP = UNIT NUMBER WHICH DETERMINES WHERE TO WRITE REGULAR OUTPUT */
/*     LP = UNIT NUMBER WHICH DETERMINES WHERE TO WRITE ERROR OUPUT */

/*     ITER: KEEPS TRACK OF THE NUMBER OF ITERATIONS */
/*     NFUN: KEEPS TRACK OF THE NUMBER OF FUNCTION/GRADIENT EVALUATIONS */
    /* Parameter adjustments */
    --w;
    --gold;
    --d__;
    --g;
    --x;
    --iprint;

    /* Function Body */

/* IFLAG = 1 INDICATES A RE-ENTRY WITH NEW FUNCTION VALUES */
    if (*iflag == 1) {
	goto L72;
    }

/* IFLAG = 2 INDICATES A RE-ENTRY WITH A NEW ITERATE */
    if (*iflag == 2) {
	goto L80;
    }

/*     INITIALIZE */
/*     ---------- */


/*     IM =   NUMBER OF TIMES BETAPR WAS NEGATIVE FOR METHOD 2 OR */
/*            NUMBER OF TIMES BETAPR WAS 0 FOR METHOD 3 */

/*     NDES = NUMBER OF LINE SEARCH ITERATIONS AFTER WOLFE CONDITIONS */
/*            WERE SATISFIED */

    runinf_1.iter = 0;
    if (*n <= 0) {
	goto L96;
    }
    runinf_1.nfun = 1;
    new__ = TRUE_;
    nrst = 0;
    im = 0;
    ndes = 0;

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L5: */
	d__[i__] = -g[i__];
    }
    gnorm = sqrt(ddot_(n, &g[1], &c__1, &g[1], &c__1));
    stp1 = one / gnorm;

/*     PARAMETERS FOR LINE SEARCH ROUTINE */
/*     ---------------------------------- */

/*     FTOL AND GTOL ARE NONNEGATIVE INPUT VARIABLES. TERMINATION */
/*       OCCURS WHEN THE SUFFICIENT DECREASE CONDITION AND THE */
/*       DIRECTIONAL DERIVATIVE CONDITION ARE SATISFIED. */

/*     XTOL IS A NONNEGATIVE INPUT VARIABLE. TERMINATION OCCURS */
/*       WHEN THE RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY */
/*       IS AT MOST XTOL. */

/*     STPMIN AND STPMAX ARE NONNEGATIVE INPUT VARIABLES WHICH */
/*       SPECIFY LOWER AND UPPER BOUNDS FOR THE STEP. */

/*     MAXFEV IS A POSITIVE INTEGER INPUT VARIABLE. TERMINATION */
/*       OCCURS WHEN THE NUMBER OF CALLS TO FCN IS AT LEAST */
/*       MAXFEV BY THE END OF AN ITERATION. */
    ftol = 1e-4;
    gtol = .1;
    if (gtol <= 1e-4) {
	if (cgdd_1.lp > 0) {
// 	    io___12.ciunit = cgdd_1.lp;
// 	    s_wsfe(&io___12);
// 	    e_wsfe();
            printf(" GTOL IS LESS THAN OR EQUAL TO 1.D-04 IT HAS BEEN RESET TO 1.D-02\n");
	}
	gtol = .01;
    }
    xtol = 1e-17;
    stpmin = 1e-20;
    stpmax = 1e20;
    maxfev = 40;

    if (iprint[1] >= 0) {
	cgbd_(&iprint[1], &runinf_1.iter, &runinf_1.nfun, &gnorm, n, &x[1], f,
		 &g[1], &stp, finish, &ndes, &im, &betafr, &betapr, &beta);
    }

/*     MAIN ITERATION LOOP */
/*    --------------------- */

L8:
    ++runinf_1.iter;
/*     WHEN NRST>N AND IREST=1 THEN RESTART */
    ++nrst;
    info = 0;


/*     CALL THE LINE SEARCH ROUTINE OF MOR'E AND THUENTE */
/*     (modified for our CG method) */
/*     ------------------------------------------------- */

/*       JJ Mor'e and D Thuente, "Linesearch Algorithms with Guaranteed */
/*       Sufficient Decrease". ACM Transactions on Mathematical */
/*       Software 20 (1994), pp 286-307. */

    nfev = 0;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L70: */
	gold[i__] = g[i__];
    }
    dg = ddot_(n, &d__[1], &c__1, &g[1], &c__1);
    dgold = dg;
    stp = one;

/* Shanno-Phua's Formula For Trial Step */

    if (! new__) {
	stp = dg0 / dg;
    }
    if (runinf_1.iter == 1) {
	stp = stp1;
    }
    ides = 0;
    new__ = FALSE_;
L72:

/*     write(6,*) 'step= ', stp */

/* Call to the line search subroutine */

    cvsmod_(n, &x[1], f, &g[1], &d__[1], &stp, &ftol, &gtol, &xtol, &stpmin, &
	    stpmax, &maxfev, &info, &nfev, &w[1], &dg, &dgout);
/*       INFO IS AN INTEGER OUTPUT VARIABLE SET AS FOLLOWS: */
/*         INFO = 0  IMPROPER INPUT PARAMETERS. */
/*         INFO =-1  A RETURN IS MADE TO COMPUTE THE FUNCTION AND GRADIENT. */
/*         INFO = 1  THE SUFFICIENT DECREASE CONDITION AND THE */
/*                   DIRECTIONAL DERIVATIVE CONDITION HOLD. */
/*         INFO = 2  RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY */
/*                   IS AT MOST XTOL. */
/*         INFO = 3  NUMBER OF CALLS TO FCN HAS REACHED MAXFEV. */
/*         INFO = 4  THE STEP IS AT THE LOWER BOUND STPMIN. */
/*         INFO = 5  THE STEP IS AT THE UPPER BOUND STPMAX. */
/*         INFO = 6  ROUNDING ERRORS PREVENT FURTHER PROGRESS. */
/*                   THERE MAY NOT BE A STEP WHICH SATISFIES THE */
/*                   SUFFICIENT DECREASE AND CURVATURE CONDITIONS. */
/*                   TOLERANCES MAY BE TOO SMALL. */
    if (info == -1) {
/*       RETURN TO FETCH FUNCTION AND GRADIENT */
	*iflag = 1;
	return 0;
    }
    if (info != 1) {
	goto L90;
    }

/*     TEST IF DESCENT DIRECTION IS OBTAINED FOR METHODS 2 AND 3 */
/*     --------------------------------------------------------- */

    gg = ddot_(n, &g[1], &c__1, &g[1], &c__1);
    gg0 = ddot_(n, &g[1], &c__1, &gold[1], &c__1);
/* Computing 2nd power */
    d__1 = gnorm;
    betapr = (gg - gg0) / (d__1 * d__1);
    if (*irest == 1 && nrst > *n) {
	nrst = 0;
	new__ = TRUE_;
	goto L75;
    }

    if (*method == 1) {
	goto L75;
    } else {
	dg1 = -gg + betapr * dgout;
	if (dg1 < 0.) {
	    goto L75;
	}
	if (iprint[1] >= 0) {
// 	    s_wsle(&io___31);
// 	    do_lio(&c__9, &c__1, "no descent", (ftnlen)10);
// 	    e_wsle();
            printf("no descent\n");

	}
	++ides;
	if (ides > 5) {
	    goto L95;
	}
	goto L72;
    }

/*     DETERMINE CORRECT BETA VALUE FOR METHOD CHOSEN */
/*     ---------------------------------------------- */

/*     IM =   NUMBER OF TIMES BETAPR WAS NEGATIVE FOR METHOD 2 OR */
/*            NUMBER OF TIMES BETAPR WAS 0 FOR METHOD 3 */

/*     NDES = NUMBER OF LINE SEARCH ITERATIONS AFTER WOLFE CONDITIONS */
/*            WERE SATISFIED */

L75:
    runinf_1.nfun += nfev;
    ndes += ides;
/* Computing 2nd power */
    d__1 = gnorm;
    betafr = gg / (d__1 * d__1);
    if (nrst == 0) {
	beta = zero;
    } else {
	if (*method == 1) {
	    beta = betafr;
	}
	if (*method == 2) {
	    beta = betapr;
	}
	if ((*method == 2 || *method == 3) && betapr < 0.) {
	    ++im;
	}
	if (*method == 3) {
	    beta = max(zero,betapr);
	}
    }

/*     COMPUTE THE NEW DIRECTION */
/*     -------------------------- */

    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
/* L78: */
	d__[i__] = -g[i__] + beta * d__[i__];
    }
    dg0 = dgold * stp;

/*     RETURN TO DRIVER FOR TERMINATION TEST */
/*     ------------------------------------- */

    gnorm = sqrt(ddot_(n, &g[1], &c__1, &g[1], &c__1));
    *iflag = 2;
    return 0;
L80:

/* Call subroutine for printing output */

    if (iprint[1] >= 0) {
	cgbd_(&iprint[1], &runinf_1.iter, &runinf_1.nfun, &gnorm, n, &x[1], f,
		 &g[1], &stp, finish, &ndes, &im, &betafr, &betapr, &beta);
    }
    if (*finish) {
	*iflag = 0;
	return 0;
    }
    goto L8;

/*     ---------------------------------------- */
/*     END OF MAIN ITERATION LOOP. ERROR EXITS. */
/*     ---------------------------------------- */

L90:
    *iflag = -1;
    if (cgdd_1.lp > 0) {
// 	io___32.ciunit = cgdd_1.lp;
// 	s_wsfe(&io___32);
// 	do_fio(&c__1, (char *)&info, (ftnlen)sizeof(integer));
// 	e_wsfe();
        printf(" IFLAG= -1 \n LINE SEARCH FAILED. SEE DOCUMENTATION OF ROUTINE CVSMOD\n\n ERROR RETURN OF LINE SEARCH: INFO= %i \n\
POSSIBLE CAUSE: FUNCTION OR GRADIENT ARE INCORRECT\n", info);



    }
    return 0;
L95:
    *iflag = -2;
    if (cgdd_1.lp > 0) {
// 	io___33.ciunit = cgdd_1.lp;
// 	s_wsfe(&io___33);
// 	do_fio(&c__1, (char *)&i__, (ftnlen)sizeof(integer));
// 	e_wsfe();

        printf(" IFLAG= -2\n DESCENT WAS NOT OBTAINED\n");

    }
    return 0;
L96:
    *iflag = -3;
    if (cgdd_1.lp > 0) {
// 	io___34.ciunit = cgdd_1.lp;
// 	s_wsfe(&io___34);
// 	e_wsfe();

printf(" IFLAG= -3\n IMPROPER INPUT PARAMETERS (N IS NOT POSITIVE)\n");

    }

/*     FORMATS */
/*     ------- */

    return 0;
} /* cgfam_ */


/*     LAST LINE OF ROUTINE CGFAM */
/*     *************************** */


/* ************************************************************************** */
/* Subroutine */ int cgbd_(integer *iprint, integer *iter, integer *nfun, 
	doublereal *gnorm, integer *n, doublereal *x, doublereal *f, 
	doublereal *g, doublereal *stp, logical *finish, integer *ndes, 
	integer *im, doublereal *betafr, doublereal *betapr, doublereal *beta)
{

    /* System generated locals */
    integer i__1;

    /* Builtin functions */
    integer s_wsle(cilist *), e_wsle(), s_wsfe(cilist *), e_wsfe(), do_fio(
	    integer *, char *, ftnlen);

    /* Local variables */
    static integer i__;




/*     --------------------------------------------------------------------- */
/*     THIS ROUTINE PRINTS MONITORING INFORMATION. THE FREQUENCY AND AMOUNT */
/*     OF OUTPUT ARE CONTROLLED BY IPRINT. */
/*     --------------------------------------------------------------------- */


    /* Parameter adjustments */
    --iprint;
    --g;
    --x;

    /* Function Body */
    if (*iter == 0) {

       printf("*************************************************\n");
        printf(" N=\t %i \n\n INITIAL VALUES:\n\n",*n);
        printf(" F= %e   GNORM= %e\n",*f,*gnorm);


	if (iprint[2] >= 1) {
            printf(" VECTOR X= ");

	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
                printf("%e  ",x[i__]);

	    }


            printf("\n\n GRADIENT VECTOR G=\n");

	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {

                printf("%e  ", g[i__]);
	    }
            printf("\n");

	}



        printf("*************************************************\n");

        printf("   I  NFN    FUNC       GNORM      STEPLEN    BETA\n\
----------------------------------------------------\n");


    } else {
	if (iprint[1] == 0 && (*iter != 1 && ! (*finish))) {
	    return 0;
	}
	if (iprint[1] != 0) {
	    if ((*iter - 1) % iprint[1] == 0 || *finish) {
		if (iprint[2] > 1 && *iter > 1) {
                    printf("   I  NFN    FUNC       GNORM      STEPLEN    BETA\n----------------------------------------------------\n");

		}


                printf("%4i %3i  %10.3e  %10.3e  %8.3e  %8.3e\n",*iter, *nfun, *f, *gnorm, *stp, *beta);




	    } else {
		return 0;
	    }
	} else {
	    if (iprint[2] > 1 && *finish) {
                printf("   I  NFN    FUNC       GNORM      STEPLEN    BETA\n----------------------------------------------------\n");

	    }

            printf("%4i %3i  %10.3e  %10.3e  %8.2e  %8.2e\n",*iter, *nfun, *f, *gnorm, *stp, *beta);

	}
	if (iprint[2] == 2 || iprint[2] == 3) {
            printf(" VECTOR X= ");
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
                printf("%e  ",x[i__]);
	    }

	    if (iprint[2] == 3) {
                printf("\n\n GRADIENT VECTOR G=\n");
		i__1 = *n;
		for (i__ = 1; i__ <= i__1; ++i__) {
                    printf("%e  ", g[i__]);
		}
	    }
	}
	if (*finish) {
            printf(" SUCCESSFUL CONVERGENCE (NO ERRORS).\n IFLAG = 0\n");
	}
    }


    return 0;
} /* cgbd_ */


/*     LAST LINE OF CGBD */
/* ************************************************************************* */


/*   ---------------------------------------------------------- */
/*     DATA */
/*   ---------------------------------------------------------- */

/*     MP = UNIT NUMBER WHICH DETERMINES WHERE TO WRITE REGULAR OUTPUT */
/*     LP = UNIT NUMBER WHICH DETERMINES WHERE TO WRITE ERROR OUPUT */

/* Subroutine */ int cgcd_()
{
    return 0;
} /* cgcd_ */


#ifdef __cplusplus
	}
#endif
