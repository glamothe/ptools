#include "lbfgs_interface.h"

#include <string.h>
#include <iostream>
#include <math.h>

namespace PTools{

inline void assign(char* dest, char* src)
{
    memcpy(dest,src,strlen(src));

}






Lbfgs::Lbfgs( ForceField& toMinim)
        :objToMinimize(toMinim)
{
};


Lbfgs::~Lbfgs()
{

}


void Lbfgs::minimize(int maxiter)
{


    int n = objToMinimize.ProblemSize();
    assert(n==1);
    std::cout  << "number of free variables for the minimizer: " << n << std::endl;


    Vdouble l(n);
    Vdouble u(n);
    Vint nbd(n);

    x.resize(n);
    g.resize(n);

    for (int i=0;i<n; i++)
    {
        l[i]=0;
        u[i]=0;
        nbd[i]=0;
        x[i] = 0.0;
        g[i] = 0.0;
    }  //unconstrained problem



    int rc;

    int m = 5;

    lbfgsb_t* opt = lbfgsb_create(n, m, &l[0], &u[0], &nbd[0]);
    assert(opt);
    assert(opt->n ==1);
    opt->iprint=1;

    double f = 0.0;

    opt->max_iter = maxiter;
/*    opt->iprint = 0;*/
    while (1) {
        rc = lbfgsb_run(opt, &x[0], &f, &g[0]);
        if (rc == 0) {
            break;
        } else if (rc < 0) { 
            printf("lbfgsb stop with an error");
            break;
        } else if (rc == 1) { 

                f = objToMinimize.Function(x);
                //objToMinimize.NumDerivatives(x,g);
                //std::cout << " numerical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";
                objToMinimize.NumDerivatives(x,g);
//                 std::cout << "analytical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";

   
        } else {
            assert(!"can not reach here");
        }
    }


    opt->task[59]='\0'; //add a null terminating character to task[]


    std::cout << opt->task  << " |  " << opt->niter << " iterations\n";
    lbfgsb_destroy(opt);




}




} //namespace lbfgs






