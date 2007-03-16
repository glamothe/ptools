/*
 * vi:ts=4:tw=78:shiftwidth=4:expandtab
 * vim600:fdm=marker
 *
 * lbfgsb.h  -  wrapper code for the Fortran L-BFGS-B optimization routine
 *
 * You can get the Fortran lbfgsb routine from:
 * http:// www.ece.northwestern.edu/~ciyou/code/lbcode.html
 * For detail usage, please consult the comment in the beginning of lbfgs.f.
 *
 * sdriver.c provides a simple use of the C interface.
 *
 * Copyright (C) 2004 by Zhang Le <ejoy@users.sourceforge.net>
 * Begin       : 25-Jul-2004
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

#ifndef _LBFGSB_H
#define _LBFGSB_H

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct {
    int     n;
    int     m;
    double* l;
    double* u;
    int* nbd;
    double* l_;
    double* u_;
    int* nbd_;
    int     niter;     /* number of iterations so far */
    int     max_iter;  /* max number of iters, default = 99, also check fun <= fun_min before stopping */
    double fun_min;    /* record the minimal obj. fun. val. */
    double factr;
    double pgtol;
    int     iprint;   /* see the comment in lbfgsb.f for usage of this field */
    double *wa;
    int* iwa;
    char    task[60];
    char csave[60];
    int lsave[4];
    int isave[44];
    double dsave[29];
} lbfgsb_t;

lbfgsb_t* lbfgsb_create(int n, int m, double* l, double* u, int* nbd);
int lbfgsb_run(lbfgsb_t* obj, double* x, double* f, double* g);
void lbfgsb_destroy(lbfgsb_t* obj);

#if defined(__cplusplus)
}
#endif

#endif /* ifndef _LBFGSB_H */

