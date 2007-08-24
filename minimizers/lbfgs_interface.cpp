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
lbfgsb_destroy(m_opt);
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

    m_opt = lbfgsb_create(n, m, &l[0], &u[0], &nbd[0]);
    assert(m_opt);


   m_opt->iprint=1;

   double f = 0.0;

    m_opt->max_iter = maxiter;

    int last_iter = 0;

/*    opt->iprint = 0;*/
    while (1) {
        rc = lbfgsb_run(m_opt, &x[0], &f, &g[0]);
        if (rc == 0) {
            break;
        } else if (rc < 0) { 
            printf("lbfgsb stop with an error");
            break;
        } else if (rc == 1) { 

                if(last_iter < m_opt->niter)
                {
                    //this is a new iteration
                    last_iter = m_opt->niter;
                    //saves the minimizer variables for each iteration (can be useful for generating animations)
                    m_vars_over_time.push_back(x);
                }


                f = objToMinimize.Function(x);
                objToMinimize.Derivatives(x,g);


        } else {
            assert(!"can not reach here");
        }
    }


    m_opt->task[59]='\0'; //add a null terminating character to task[]


    std::cout << m_opt->task  << " |  " << m_opt->niter << " iterations\n";
    




}




} //namespace lbfgs






