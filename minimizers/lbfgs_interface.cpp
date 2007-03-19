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
    std::cerr  << "Problem size: " << n << std::endl;

    double f;
    int j;

    int icall;
    //double x[N];
    x.resize(n);
    //double g[N];
    g.resize(n);

    lbfgs_t* opt;

    opt = lbfgs_create(n, 10, 1.0E-3);  //n:proble size, 5: number of memory corrections, 1e-5: eps.
    opt->iprint[0] = 0;  //be a little bit verbose
    opt->iprint[1] = 0;  // TODO: add comment
    opt->diagco = 0;    // ? -> TODO: add comment


   for (j = 0; j < n; j++) {
    x[j]=0.0;
    }

    icall = 0;
    while (1) {
              //f and g evaluation:

            f = objToMinimize.Function(x);
                //objToMinimize.NumDerivatives(x,g);
                //std::cout << " numerical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";
                objToMinimize.Derivatives(x,g);
//                 std::cout << "analytical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";

        
        if (lbfgs_run(opt, &x[0], &f, &g[0]) <= 0)
            break;
        ++icall;
        /* We allow at most 2000 evaluations of F and G */
        if (icall > 2000)
            break;
    }

    lbfgs_destroy(opt);


}






} //namespace lbfgs






