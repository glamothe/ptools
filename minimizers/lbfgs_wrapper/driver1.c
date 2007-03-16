/*
 * vi:ts=4:tw=78:shiftwidth=4:expandtab
 * vim600:fdm=marker
 *
 * sdrive.c  -  simple driver for lbfgsb, translated from driver1.f
 * usage:
 *      1. g77 -c lbfgsb.f
 *      2. gcc -o driver1 driver1.c lbfgsb_wrapper.c lbfgsb.o -lg2c 
 *      3. ./driver1
 *
 * Copyright (C) 2004 by Zhang Le <ejoy@users.sourceforge.net>
 * Begin       : 25-Jul-2004
 * Last Change : 25-Jul-2005.
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

#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lbfgsb.h"

int main(int argc, char **argv)
{
    double f;
    int i;
    double t1;
    double x[25];
    double g[25];
    double l[25];
    double u[25];
    int nbd[25];
    int rc;
    int n = 25;
    int m = 5;

    lbfgsb_t* opt;

    for (i = 0; i < n; i += 2) {
        nbd[i] = 2;
        l[i] = 1.0E0;
        u[i] = 1.0E2;
    }

    for (i = 1; i < n; i += 2) {
        nbd[i] = 2;
        l[i] = -1.0E2;
        u[i] = 1.0E2;
    }

    opt = lbfgsb_create(n, m, l, u, nbd);
    assert(opt);

    for (i = 0; i < n; ++i) {
        x[i] = 3.0E0;
        g[i] = 0.0;
    }
    printf("Solving sample problem.\n");
    printf("f = 0.0 at the optimal solution.)\n");


    f = 0.0;

/*    opt->maxiter = 10;*/
/*    opt->iprint = 0;*/
    while (1) {
        rc = lbfgsb_run(opt, x, &f, g);
        if (rc == 0) {
            break;
        } else if (rc < 0) { 
            printf("lbfgsb stop with an error");
            break;
        } else if (rc == 1) { 
            f = 0.25*(x[0] - 1)*(x[0] - 1);
            for (i = 1; i < n; ++i) {
                double t = x[i-1]*x[i-1];
                f = f + (x[i] - t)*(x[i]-t);
            }
            f = f*4.0;

            t1  = x[1] - x[0]*x[0];
            g[0] = 2.*(x[0] - 1.) - 1.6E1*x[0]*t1;
            for (i = 1; i < n-1; ++i) {
                double t2 = t1;
                t1 = x[i+1] - x[i]*x[i];
                g[i] = 8*t2-1.6E1*x[i]*t1;
            }
            g[n-1] = 8*t1;
        } else {
            assert(!"can not reach here");
        }
    }
    lbfgsb_destroy(opt);

    return 0;
}


