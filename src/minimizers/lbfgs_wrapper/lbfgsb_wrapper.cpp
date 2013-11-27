/*
 * vi:ts=4:tw=78:shiftwidth=4:expandtab
 * vim600:fdm=marker
 *
 * lbfgsb_wrapper.c  - wrapper code for Fortran L-BFGS optimization routine
 *
 * You can get the Fortran lbfgsb routine from:
 * http:// www.netlib.org/opt/lbfgsb_um.shar
 *
 * For detail usage, please consult the comment in the beginning of lbfgsb.f.
 *
 * Copyright (C) 2004 by Zhang Le <ejoy@users.sourceforge.net>
 * Begin       : 17-Nov-2004
 * Last Change : 20-Aug-2005.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>
#include <float.h>
#include <string.h>
#include <stdio.h>
#include "lbfgsb.h"

#if defined(WIN32) && !defined(__GNUC__) /* assume use Intel Fortran Compiler on win32 platform if not compiled with GCC */
#    define lbfgsb_FUN setulb
#else
#    define lbfgsb_FUN setulb_  /* assume use GNU f77 otherwise */
#endif
    /* Fortran lbfgsb interface */
#if defined(__cplusplus)
extern "C" {
#endif

extern void lbfgsb_FUN(int* n, int* m, double* x, double* l, double* u,
        int* nbd, double* f, double* g, double* factr, double* pgtol,
        double* wa, int* iwa, char* task, int* iprint, char* csave, int*
        lsave, int* isave, double* dsave);

#if defined(__cplusplus)
}
#endif

/* create an opt object
 * n is the dimension of the variable
 * m is the number of the corrections used in BFGS update
 * l is the array of lower bounds
 * u is the array of upper bounds
 * nbd is the indicator array:
   nbd(i)=0 if x(i) is unbounded,
          1 if x(i) has only a lower bound,
          2 if x(i) has both lower and upper bounds, 
          3 if x(i) has only an upper bound.
 * if all of l, u, nbd are NULL, this is equivalent to unconstrained
 * optimization
 */
lbfgsb_t* lbfgsb_create(int n, int m, double* l, double* u, int* nbd) {
    int i;
    lbfgsb_t* opt = (lbfgsb_t*)malloc(sizeof(lbfgsb_t));

    if (!opt)
        return 0;

    opt->iwa = (int*) malloc(sizeof(int)*3*n);
    if (!opt->iwa) {
        free(opt);
        return 0;
    }

    opt->wa = (double*)malloc(sizeof(double) * ((2*m+4)*n + 11*m*m + 8*m));
    if (!opt->wa) {
        free(opt->iwa);
        free(opt);
        return 0;
    }


    opt->n         = n;
    opt->m         = m;
    opt->niter     = 0;
    opt->max_iter  = 99;
    opt->l = l;
    opt->u = u;
    opt->nbd = nbd;
    opt->l_ = 0;
    opt->u_ = 0;
    opt->nbd_ = 0;
    opt->factr  = 1.0E+7;
    opt->pgtol  = 1.0E-5;
    opt->iprint = 0;
    opt->fun_min = DBL_MAX;

    if (l == 0 || u == 0 || nbd == 0) {
        if (!(l == 0 && u == 0 && nbd == 0)) {
            fprintf(stderr, "(lbfgsb_create) for unconstrained optimization, l, u, nbd must all be NULL\n");
            free(opt->iwa);
            free(opt);
            return 0;
        }
        opt->nbd_ = (int*) malloc(sizeof(int)*n);
/*        opt->l_ = malloc(sizeof(double)*n);*/
/*        opt->u_ = malloc(sizeof(double)*n);*/
        for (i = 0; i < n; ++i) {
            opt->nbd_[i] = 0;
/*            opt->l_[i] = 0.0;*/
/*            opt->u_[i] = 0.0;*/
        }
        opt->nbd = opt->nbd_;
/*        opt->l = opt->l_;*/
/*        opt->u = opt->u_;*/
    }

    for (i = 0; i < 60; ++i)
        opt->task[i] = ' ';
    strncpy(opt->task, "START", 5);

    return opt;
}

/* free all the memory used by the optimizer */
void lbfgsb_destroy(lbfgsb_t* opt) {
    free(opt->l_);
    free(opt->u_);
    free(opt->nbd_);
    free(opt->iwa);
    free(opt->wa);
    free(opt);
}


/* x is the n-dimension variable
 * f is the current value of objective function
 * g is the n-dimension gradient of f
 *
 * return value:
 *       = 0: success
 *       < 0: some error occur, see comment in lbfgsb.f for detail
 *       = 1: user must evaluate F and G
 */
int lbfgsb_run(lbfgsb_t* opt, double* x, double* f, double* g) {

    if (*f < opt->fun_min) {
        opt->fun_min = *f;
    }

    lbfgsb_FUN(&opt->n, &opt->m, x, opt->l, opt->u, opt->nbd, f, g,
            &opt->factr, &opt->pgtol, opt->wa, opt->iwa, opt->task,
            &opt->iprint, opt->csave, opt->lsave, opt->isave, opt->dsave);

    if (opt->task[0] == 'F' && opt->task[1] == 'G') {
        return 1;
    } else if (strncmp(opt->task, "NEW_X", 5) == 0) {
        opt->niter++;
        if (opt->niter >= opt->max_iter) {
            if (*f > opt->fun_min) {
                /* fprintf(stdout, "(lbfgsb_run) max iter reached but fun val is not minimum, wait for another iteration\n"); */
                if (opt->niter >= opt->max_iter + 10) {
                /* really too many iterations made. Stopping */
                strncpy(opt->task, "STOP,ERR: could not converge", 28); 
                }
            } else {
                strncpy(opt->task, "STOP, max iterations reached", 28);
            }
        }
        return 1;
    } else if (strncmp(opt->task, "STOP", 4) == 0) {
        return 0;
    } else if (strncmp(opt->task, "CONV", 4) == 0) {
        return 0;
    } else if (strncmp(opt->task, "ABNO", 4) == 0) {
        return -1;
    } else if (strncmp(opt->task, "ERROR", 5) == 0) {
        return -1;
    } else {
        opt->task[59] = '\0';
        fprintf(stderr, "(lbfgsb_run) unknown return value in task:%s\n", opt->task);
        return -1;
    }
}

