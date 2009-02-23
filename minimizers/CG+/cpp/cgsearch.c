/* cgsearch.f -- translated by f2c (version 20061008).
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



/* Subroutine */ int cvsmod_(integer *n, doublereal *x, doublereal *f, 
	doublereal *g, doublereal *s, doublereal *stp, doublereal *ftol, 
	doublereal *gtol, doublereal *xtol, doublereal *stpmin, doublereal *
	stpmax, integer *maxfev, integer *info, integer *nfev, doublereal *wa,
	 doublereal *dginit, doublereal *dgout)
{
    /* Initialized data */

    static doublereal p5 = .5;
    static doublereal p66 = .66;
    static doublereal xtrapf = 4.;
    static doublereal zero = 0.;

    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Local variables */
    static integer j;
    static doublereal dg, fm, fx, fy, dgm, dgx, dgy, fxm, fym, stx, sty, dgxm,
	     dgym;
    static integer infoc;
    static doublereal finit, width, stmin, stmax;
    static logical stage1;
    static doublereal width1, ftest1;
    static logical brackt;
    static doublereal dgtest;
    extern /* Subroutine */ int cstepm_(doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, logical *, doublereal *,
	     doublereal *, integer *);

/*     ********** */

/*     SUBROUTINE CVSMOD */

/*     *** This is a modification of More's line search routine ** */
/*                   * * * * * * */
/*     THE PURPOSE OF CVSMOD IS TO FIND A STEP WHICH SATISFIES */
/*     A SUFFICIENT DECREASE CONDITION AND A CURVATURE CONDITION. */
/*     THE USER MUST PROVIDE A SUBROUTINE WHICH CALCULATES THE */
/*     FUNCTION AND THE GRADIENT. */

/*     AT EACH STAGE THE SUBROUTINE UPDATES AN INTERVAL OF */
/*     UNCERTAINTY WITH ENDPOINTS STX AND STY. THE INTERVAL OF */
/*     UNCERTAINTY IS INITIALLY CHOSEN SO THAT IT CONTAINS A */
/*     MINIMIZER OF THE MODIFIED FUNCTION */

/*          F(X+STP*S) - F(X) - FTOL*STP*(GRADF(X)'S). */

/*     IF A STEP IS OBTAINED FOR WHICH THE MODIFIED FUNCTION */
/*     HAS A NONPOSITIVE FUNCTION VALUE AND NONNEGATIVE DERIVATIVE, */
/*     THEN THE INTERVAL OF UNCERTAINTY IS CHOSEN SO THAT IT */
/*     CONTAINS A MINIMIZER OF F(X+STP*S). */

/*     THE ALGORITHM IS DESIGNED TO FIND A STEP WHICH SATISFIES */
/*     THE SUFFICIENT DECREASE CONDITION */

/*           F(X+STP*S) .LE. F(X) + FTOL*STP*(GRADF(X)'S), */

/*     AND THE CURVATURE CONDITION */

/*           ABS(GRADF(X+STP*S)'S)) .LE. GTOL*ABS(GRADF(X)'S). */

/*     IF FTOL IS LESS THAN GTOL AND IF, FOR EXAMPLE, THE FUNCTION */
/*     IS BOUNDED BELOW, THEN THERE IS ALWAYS A STEP WHICH SATISFIES */
/*     BOTH CONDITIONS. IF NO STEP CAN BE FOUND WHICH SATISFIES BOTH */
/*     CONDITIONS, THEN THE ALGORITHM USUALLY STOPS WHEN ROUNDING */
/*     ERRORS PREVENT FURTHER PROGRESS. IN THIS CASE STP ONLY */
/*     SATISFIES THE SUFFICIENT DECREASE CONDITION. */

/*     THE SUBROUTINE STATEMENT IS */

/*        SUBROUTINE CVSMOD(N,X,F,G,S,STP,FTOL,GTOL,XTOL, */
/*                   STPMIN,STPMAX,MAXFEV,INFO,NFEV,WA,DG,DGOUT) */
/*     WHERE */

/*       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER */
/*         OF VARIABLES. */

/*       X IS AN ARRAY OF LENGTH N. ON INPUT IT MUST CONTAIN THE */
/*         BASE POINT FOR THE LINE SEARCH. ON OUTPUT IT CONTAINS */
/*         X + STP*S. */

/*       F IS A VARIABLE. ON INPUT IT MUST CONTAIN THE VALUE OF F */
/*         AT X. ON OUTPUT IT CONTAINS THE VALUE OF F AT X + STP*S. */

/*       G IS AN ARRAY OF LENGTH N. ON INPUT IT MUST CONTAIN THE */
/*         GRADIENT OF F AT X. ON OUTPUT IT CONTAINS THE GRADIENT */
/*         OF F AT X + STP*S. */

/*       S IS AN INPUT ARRAY OF LENGTH N WHICH SPECIFIES THE */
/*         SEARCH DIRECTION. */

/*       STP IS A NONNEGATIVE VARIABLE. ON INPUT STP CONTAINS AN */
/*         INITIAL ESTIMATE OF A SATISFACTORY STEP. ON OUTPUT */
/*         STP CONTAINS THE FINAL ESTIMATE. */

/*       FTOL AND GTOL ARE NONNEGATIVE INPUT VARIABLES. TERMINATION */
/*         OCCURS WHEN THE SUFFICIENT DECREASE CONDITION AND THE */
/*         DIRECTIONAL DERIVATIVE CONDITION ARE SATISFIED. */

/*       XTOL IS A NONNEGATIVE INPUT VARIABLE. TERMINATION OCCURS */
/*         WHEN THE RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY */
/*         IS AT MOST XTOL. */

/*       STPMIN AND STPMAX ARE NONNEGATIVE INPUT VARIABLES WHICH */
/*         SPECIFY LOWER AND UPPER BOUNDS FOR THE STEP. */

/*       MAXFEV IS A POSITIVE INTEGER INPUT VARIABLE. TERMINATION */
/*         OCCURS WHEN THE NUMBER OF CALLS TO FCN IS AT LEAST */
/*         MAXFEV BY THE END OF AN ITERATION. */

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

/*       NFEV IS AN INTEGER OUTPUT VARIABLE SET TO THE NUMBER OF */
/*         CALLS TO FCN. */

/*       WA IS A WORK ARRAY OF LENGTH N. */

/*       *** The following two parameters are a modification to the code */

/*       DG IS THE INITIAL DIRECTIONAL DERIVATIVE (IN THE ORIGINAL CODE */
/*                 IT WAS COMPUTED IN THIS ROUTINE0 */

/*       DGOUT IS THE VALUE OF THE DIRECTIONAL DERIVATIVE WHEN THE WOLFE */
/*             CONDITIONS HOLD, AND AN EXIT IS MADE TO CHECK DESCENT. */

/*     SUBPROGRAMS CALLED */

/*       CSTEPM */

/*       FORTRAN-SUPPLIED...ABS,MAX,MIN */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1983 */
/*     JORGE J. MORE', DAVID J. THUENTE */

/*     ********** */
    /* Parameter adjustments */
    --wa;
    --s;
    --g;
    --x;

    /* Function Body */
    if (*info == -1) {
	goto L45;
    }
    if (*info == 1) {
	goto L321;
    }
    infoc = 1;

/*     CHECK THE INPUT PARAMETERS FOR ERRORS. */

    if (*n <= 0 || *stp <= zero || *ftol < zero || *gtol < zero || *xtol < 
	    zero || *stpmin < zero || *stpmax < *stpmin || *maxfev <= 0) {
	return 0;
    }

/*     COMPUTE THE INITIAL GRADIENT IN THE SEARCH DIRECTION */
/*     AND CHECK THAT S IS A DESCENT DIRECTION. */

    if (*dginit >= zero) {
	return 0;
    }

/*     INITIALIZE LOCAL VARIABLES. */

    brackt = FALSE_;
    stage1 = TRUE_;
    *nfev = 0;
    finit = *f;
    dgtest = *ftol * *dginit;
    width = *stpmax - *stpmin;
    width1 = width / p5;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	wa[j] = x[j];
/* L20: */
    }

/*     THE VARIABLES STX, FX, DGX CONTAIN THE VALUES OF THE STEP, */
/*     FUNCTION, AND DIRECTIONAL DERIVATIVE AT THE BEST STEP. */
/*     THE VARIABLES STY, FY, DGY CONTAIN THE VALUE OF THE STEP, */
/*     FUNCTION, AND DERIVATIVE AT THE OTHER ENDPOINT OF */
/*     THE INTERVAL OF UNCERTAINTY. */
/*     THE VARIABLES STP, F, DG CONTAIN THE VALUES OF THE STEP, */
/*     FUNCTION, AND DERIVATIVE AT THE CURRENT STEP. */

    stx = zero;
    fx = finit;
    dgx = *dginit;
    sty = zero;
    fy = finit;
    dgy = *dginit;

/*     START OF ITERATION. */

L30:

/*        SET THE MINIMUM AND MAXIMUM STEPS TO CORRESPOND */
/*        TO THE PRESENT INTERVAL OF UNCERTAINTY. */

    if (brackt) {
	stmin = min(stx,sty);
	stmax = max(stx,sty);
    } else {
	stmin = stx;
	stmax = *stp + xtrapf * (*stp - stx);
    }

/*        FORCE THE STEP TO BE WITHIN THE BOUNDS STPMAX AND STPMIN. */

    *stp = max(*stp,*stpmin);
    *stp = min(*stp,*stpmax);

/*        IF AN UNUSUAL TERMINATION IS TO OCCUR THEN LET */
/*        STP BE THE LOWEST POINT OBTAINED SO FAR. */

    if (brackt && (*stp <= stmin || *stp >= stmax) || *nfev >= *maxfev - 1 || 
	    infoc == 0 || brackt && stmax - stmin <= *xtol * stmax) {
	*stp = stx;
    }

/*        EVALUATE THE FUNCTION AND GRADIENT AT STP */
/*        AND COMPUTE THE DIRECTIONAL DERIVATIVE. */

    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	x[j] = wa[j] + *stp * s[j];
/* L40: */
    }
/*        Return to compute function value */
    *info = -1;
    return 0;

L45:
    *info = 0;
    ++(*nfev);
    dg = zero;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	dg += g[j] * s[j];
/* L50: */
    }
    ftest1 = finit + *stp * dgtest;

/*        TEST FOR CONVERGENCE. */

    if (brackt && (*stp <= stmin || *stp >= stmax) || infoc == 0) {
	*info = 6;
    }
    if (*stp == *stpmax && *f <= ftest1 && dg <= dgtest) {
	*info = 5;
    }
    if (*stp == *stpmin && (*f > ftest1 || dg >= dgtest)) {
	*info = 4;
    }
    if (*nfev >= *maxfev) {
	*info = 3;
    }
    if (brackt && stmax - stmin <= *xtol * stmax) {
	*info = 2;
    }
/*        More's code has been modified so that at least one new */
/*        function value is computed during the line search (enforcing */
/*        at least one interpolation is not easy, since the code may */
/*        override an interpolation) */
    if (*f <= ftest1 && abs(dg) <= *gtol * (-(*dginit)) && *nfev > 1) {
	*info = 1;
    }

/*        CHECK FOR TERMINATION. */

    if (*info != 0) {
	*dgout = dg;
	return 0;
    }
L321:

/*        IN THE FIRST STAGE WE SEEK A STEP FOR WHICH THE MODIFIED */
/*        FUNCTION HAS A NONPOSITIVE VALUE AND NONNEGATIVE DERIVATIVE. */

    if (stage1 && *f <= ftest1 && dg >= min(*ftol,*gtol) * *dginit) {
	stage1 = FALSE_;
    }

/*        A MODIFIED FUNCTION IS USED TO PREDICT THE STEP ONLY IF */
/*        WE HAVE NOT OBTAINED A STEP FOR WHICH THE MODIFIED */
/*        FUNCTION HAS A NONPOSITIVE FUNCTION VALUE AND NONNEGATIVE */
/*        DERIVATIVE, AND IF A LOWER FUNCTION VALUE HAS BEEN */
/*        OBTAINED BUT THE DECREASE IS NOT SUFFICIENT. */

    if (stage1 && *f <= fx && *f > ftest1) {

/*           DEFINE THE MODIFIED FUNCTION AND DERIVATIVE VALUES. */

	fm = *f - *stp * dgtest;
	fxm = fx - stx * dgtest;
	fym = fy - sty * dgtest;
	dgm = dg - dgtest;
	dgxm = dgx - dgtest;
	dgym = dgy - dgtest;

/*           CALL CSTEPM TO UPDATE THE INTERVAL OF UNCERTAINTY */
/*           AND TO COMPUTE THE NEW STEP. */

	cstepm_(&stx, &fxm, &dgxm, &sty, &fym, &dgym, stp, &fm, &dgm, &brackt,
		 &stmin, &stmax, &infoc);

/*           RESET THE FUNCTION AND GRADIENT VALUES FOR F. */

	fx = fxm + stx * dgtest;
	fy = fym + sty * dgtest;
	dgx = dgxm + dgtest;
	dgy = dgym + dgtest;
    } else {

/*           CALL CSTEPM TO UPDATE THE INTERVAL OF UNCERTAINTY */
/*           AND TO COMPUTE THE NEW STEP. */

	cstepm_(&stx, &fx, &dgx, &sty, &fy, &dgy, stp, f, &dg, &brackt, &
		stmin, &stmax, &infoc);
    }

/*        FORCE A SUFFICIENT DECREASE IN THE SIZE OF THE */
/*        INTERVAL OF UNCERTAINTY. */

    if (brackt) {
	if ((d__1 = sty - stx, abs(d__1)) >= p66 * width1) {
	    *stp = stx + p5 * (sty - stx);
	}
	width1 = width;
	width = (d__1 = sty - stx, abs(d__1));
    }

/*        END OF ITERATION. */

    goto L30;

/*     LAST CARD OF SUBROUTINE CVSMOD. */

} /* cvsmod_ */

/* Subroutine */ int cstepm_(doublereal *stx, doublereal *fx, doublereal *dx, 
	doublereal *sty, doublereal *fy, doublereal *dy, doublereal *stp, 
	doublereal *fp, doublereal *dp, logical *brackt, doublereal *stpmin, 
	doublereal *stpmax, integer *info)
{
    /* System generated locals */
    doublereal d__1, d__2, d__3;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static doublereal p, q, r__, s, sgnd, stpc, stpf, stpq, gamma, theta;
    static logical bound;

/*     ********** */

/*     SUBROUTINE CSTEPM */

/*     THE PURPOSE OF CSTEPM IS TO COMPUTE A SAFEGUARDED STEP FOR */
/*     A LINESEARCH AND TO UPDATE AN INTERVAL OF UNCERTAINTY FOR */
/*     A MINIMIZER OF THE FUNCTION. */

/*     THE PARAMETER STX CONTAINS THE STEP WITH THE LEAST FUNCTION */
/*     VALUE. THE PARAMETER STP CONTAINS THE CURRENT STEP. IT IS */
/*     ASSUMED THAT THE DERIVATIVE AT STX IS NEGATIVE IN THE */
/*     DIRECTION OF THE STEP. IF BRACKT IS SET TRUE THEN A */
/*     MINIMIZER HAS BEEN BRACKETED IN AN INTERVAL OF UNCERTAINTY */
/*     WITH ENDPOINTS STX AND STY. */

/*     THE SUBROUTINE STATEMENT IS */

/*       SUBROUTINE CSTEPM(STX,FX,DX,STY,FY,DY,STP,FP,DP,BRACKT, */
/*                        STPMIN,STPMAX,INFO) */

/*     WHERE */

/*       STX, FX, AND DX ARE VARIABLES WHICH SPECIFY THE STEP, */
/*         THE FUNCTION, AND THE DERIVATIVE AT THE BEST STEP OBTAINED */
/*         SO FAR. THE DERIVATIVE MUST BE NEGATIVE IN THE DIRECTION */
/*         OF THE STEP, THAT IS, DX AND STP-STX MUST HAVE OPPOSITE */
/*         SIGNS. ON OUTPUT THESE PARAMETERS ARE UPDATED APPROPRIATELY. */

/*       STY, FY, AND DY ARE VARIABLES WHICH SPECIFY THE STEP, */
/*         THE FUNCTION, AND THE DERIVATIVE AT THE OTHER ENDPOINT OF */
/*         THE INTERVAL OF UNCERTAINTY. ON OUTPUT THESE PARAMETERS ARE */
/*         UPDATED APPROPRIATELY. */

/*       STP, FP, AND DP ARE VARIABLES WHICH SPECIFY THE STEP, */
/*         THE FUNCTION, AND THE DERIVATIVE AT THE CURRENT STEP. */
/*         IF BRACKT IS SET TRUE THEN ON INPUT STP MUST BE */
/*         BETWEEN STX AND STY. ON OUTPUT STP IS SET TO THE NEW STEP. */

/*       BRACKT IS A LOGICAL VARIABLE WHICH SPECIFIES IF A MINIMIZER */
/*         HAS BEEN BRACKETED. IF THE MINIMIZER HAS NOT BEEN BRACKETED */
/*         THEN ON INPUT BRACKT MUST BE SET FALSE. IF THE MINIMIZER */
/*         IS BRACKETED THEN ON OUTPUT BRACKT IS SET TRUE. */

/*       STPMIN AND STPMAX ARE INPUT VARIABLES WHICH SPECIFY LOWER */
/*         AND UPPER BOUNDS FOR THE STEP. */

/*       INFO IS AN INTEGER OUTPUT VARIABLE SET AS FOLLOWS: */
/*         IF INFO = 1,2,3,4,5, THEN THE STEP HAS BEEN COMPUTED */
/*         ACCORDING TO ONE OF THE FIVE CASES BELOW. OTHERWISE */
/*         INFO = 0, AND THIS INDICATES IMPROPER INPUT PARAMETERS. */

/*     SUBPROGRAMS CALLED */

/*       FORTRAN-SUPPLIED ... ABS,MAX,MIN,SQRT */

/*     ARGONNE NATIONAL LABORATORY. MINPACK PROJECT. JUNE 1983 */
/*     JORGE J. MORE', DAVID J. THUENTE */

/*     ********** */
    *info = 0;

/*     CHECK THE INPUT PARAMETERS FOR ERRORS. */

    if (*brackt && (*stp <= min(*stx,*sty) || *stp >= max(*stx,*sty)) || *dx *
	     (*stp - *stx) >= (float)0. || *stpmax < *stpmin) {
	return 0;
    }

/*     DETERMINE IF THE DERIVATIVES HAVE OPPOSITE SIGN. */

    sgnd = *dp * (*dx / abs(*dx));

/*     FIRST CASE. A HIGHER FUNCTION VALUE. */
/*     THE MINIMUM IS BRACKETED. IF THE CUBIC STEP IS CLOSER */
/*     TO STX THAN THE QUADRATIC STEP, THE CUBIC STEP IS TAKEN, */
/*     ELSE THE AVERAGE OF THE CUBIC AND QUADRATIC STEPS IS TAKEN. */

    if (*fp > *fx) {
	*info = 1;
	bound = TRUE_;
	theta = (*fx - *fp) * 3 / (*stp - *stx) + *dx + *dp;
/* Computing MAX */
	d__1 = abs(theta), d__2 = abs(*dx), d__1 = max(d__1,d__2), d__2 = abs(
		*dp);
	s = max(d__1,d__2);
/* Computing 2nd power */
	d__1 = theta / s;
	gamma = s * sqrt(d__1 * d__1 - *dx / s * (*dp / s));
	if (*stp < *stx) {
	    gamma = -gamma;
	}
	p = gamma - *dx + theta;
	q = gamma - *dx + gamma + *dp;
	r__ = p / q;
	stpc = *stx + r__ * (*stp - *stx);
	stpq = *stx + *dx / ((*fx - *fp) / (*stp - *stx) + *dx) / 2 * (*stp - 
		*stx);
	if ((d__1 = stpc - *stx, abs(d__1)) < (d__2 = stpq - *stx, abs(d__2)))
		 {
	    stpf = stpc;
	} else {
	    stpf = stpc + (stpq - stpc) / 2;
	}
	*brackt = TRUE_;

/*     SECOND CASE. A LOWER FUNCTION VALUE AND DERIVATIVES OF */
/*     OPPOSITE SIGN. THE MINIMUM IS BRACKETED. IF THE CUBIC */
/*     STEP IS CLOSER TO STX THAN THE QUADRATIC (SECANT) STEP, */
/*     THE CUBIC STEP IS TAKEN, ELSE THE QUADRATIC STEP IS TAKEN. */

    } else if (sgnd < (float)0.) {
	*info = 2;
	bound = FALSE_;
	theta = (*fx - *fp) * 3 / (*stp - *stx) + *dx + *dp;
/* Computing MAX */
	d__1 = abs(theta), d__2 = abs(*dx), d__1 = max(d__1,d__2), d__2 = abs(
		*dp);
	s = max(d__1,d__2);
/* Computing 2nd power */
	d__1 = theta / s;
	gamma = s * sqrt(d__1 * d__1 - *dx / s * (*dp / s));
	if (*stp > *stx) {
	    gamma = -gamma;
	}
	p = gamma - *dp + theta;
	q = gamma - *dp + gamma + *dx;
	r__ = p / q;
	stpc = *stp + r__ * (*stx - *stp);
	stpq = *stp + *dp / (*dp - *dx) * (*stx - *stp);
	if ((d__1 = stpc - *stp, abs(d__1)) > (d__2 = stpq - *stp, abs(d__2)))
		 {
	    stpf = stpc;
	} else {
	    stpf = stpq;
	}
	*brackt = TRUE_;

/*     THIRD CASE. A LOWER FUNCTION VALUE, DERIVATIVES OF THE */
/*     SAME SIGN, AND THE MAGNITUDE OF THE DERIVATIVE DECREASES. */
/*     THE CUBIC STEP IS ONLY USED IF THE CUBIC TENDS TO INFINITY */
/*     IN THE DIRECTION OF THE STEP OR IF THE MINIMUM OF THE CUBIC */
/*     IS BEYOND STP. OTHERWISE THE CUBIC STEP IS DEFINED TO BE */
/*     EITHER STPMIN OR STPMAX. THE QUADRATIC (SECANT) STEP IS ALSO */
/*     COMPUTED AND IF THE MINIMUM IS BRACKETED THEN THE THE STEP */
/*     CLOSEST TO STX IS TAKEN, ELSE THE STEP FARTHEST AWAY IS TAKEN. */

    } else if (abs(*dp) < abs(*dx)) {
	*info = 3;
	bound = TRUE_;
	theta = (*fx - *fp) * 3 / (*stp - *stx) + *dx + *dp;
/* Computing MAX */
	d__1 = abs(theta), d__2 = abs(*dx), d__1 = max(d__1,d__2), d__2 = abs(
		*dp);
	s = max(d__1,d__2);

/*        THE CASE GAMMA = 0 ONLY ARISES IF THE CUBIC DOES NOT TEND */
/*        TO INFINITY IN THE DIRECTION OF THE STEP. */

/* Computing MAX */
/* Computing 2nd power */
	d__3 = theta / s;
	d__1 = 0., d__2 = d__3 * d__3 - *dx / s * (*dp / s);
	gamma = s * sqrt((max(d__1,d__2)));
	if (*stp > *stx) {
	    gamma = -gamma;
	}
	p = gamma - *dp + theta;
	q = gamma + (*dx - *dp) + gamma;
	r__ = p / q;
	if (r__ < (float)0. && gamma != (float)0.) {
	    stpc = *stp + r__ * (*stx - *stp);
	} else if (*stp > *stx) {
	    stpc = *stpmax;
	} else {
	    stpc = *stpmin;
	}
	stpq = *stp + *dp / (*dp - *dx) * (*stx - *stp);
	if (*brackt) {
	    if ((d__1 = *stp - stpc, abs(d__1)) < (d__2 = *stp - stpq, abs(
		    d__2))) {
		stpf = stpc;
	    } else {
		stpf = stpq;
	    }
	} else {
	    if ((d__1 = *stp - stpc, abs(d__1)) > (d__2 = *stp - stpq, abs(
		    d__2))) {
		stpf = stpc;
	    } else {
		stpf = stpq;
	    }
	}

/*     FOURTH CASE. A LOWER FUNCTION VALUE, DERIVATIVES OF THE */
/*     SAME SIGN, AND THE MAGNITUDE OF THE DERIVATIVE DOES */
/*     NOT DECREASE. IF THE MINIMUM IS NOT BRACKETED, THE STEP */
/*     IS EITHER STPMIN OR STPMAX, ELSE THE CUBIC STEP IS TAKEN. */

    } else {
	*info = 4;
	bound = FALSE_;
	if (*brackt) {
	    theta = (*fp - *fy) * 3 / (*sty - *stp) + *dy + *dp;
/* Computing MAX */
	    d__1 = abs(theta), d__2 = abs(*dy), d__1 = max(d__1,d__2), d__2 = 
		    abs(*dp);
	    s = max(d__1,d__2);
/* Computing 2nd power */
	    d__1 = theta / s;
	    gamma = s * sqrt(d__1 * d__1 - *dy / s * (*dp / s));
	    if (*stp > *sty) {
		gamma = -gamma;
	    }
	    p = gamma - *dp + theta;
	    q = gamma - *dp + gamma + *dy;
	    r__ = p / q;
	    stpc = *stp + r__ * (*sty - *stp);
	    stpf = stpc;
	} else if (*stp > *stx) {
	    stpf = *stpmax;
	} else {
	    stpf = *stpmin;
	}
    }

/*     UPDATE THE INTERVAL OF UNCERTAINTY. THIS UPDATE DOES NOT */
/*     DEPEND ON THE NEW STEP OR THE CASE ANALYSIS ABOVE. */

    if (*fp > *fx) {
	*sty = *stp;
	*fy = *fp;
	*dy = *dp;
    } else {
	if (sgnd < (float)0.) {
	    *sty = *stx;
	    *fy = *fx;
	    *dy = *dx;
	}
	*stx = *stp;
	*fx = *fp;
	*dx = *dp;
    }

/*     COMPUTE THE NEW STEP AND SAFEGUARD IT. */

    stpf = min(*stpmax,stpf);
    stpf = max(*stpmin,stpf);
    *stp = stpf;
    if (*brackt && bound) {
	if (*sty > *stx) {
/* Computing MIN */
	    d__1 = *stx + (*sty - *stx) * (float).66;
	    *stp = min(d__1,*stp);
	} else {
/* Computing MAX */
	    d__1 = *stx + (*sty - *stx) * (float).66;
	    *stp = max(d__1,*stp);
	}
    }
    return 0;

/*     LAST CARD OF SUBROUTINE CSTEPM. */

} /* cstepm_ */

#ifdef __cplusplus
	}
#endif
