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


//     double f;
//     int j;
// 
//     int icall;
//     //double x[N];
//     x.resize(n);
//     //double g[N];
//     g.resize(n);
// 
//     lbfgs_t* opt;
// 
//     opt = lbfgs_create(n, 10, 1.0E-3);  //n:proble size, 5: number of memory corrections, 1e-5: eps.
//     opt->iprint[0] = 0;  //be a little bit verbose
//     opt->iprint[1] = 0;  // TODO: add comment
//     opt->diagco = 0;    // ? -> TODO: add comment
// 
// 
//    for (j = 0; j < n; j++) {
//     x[j]=0.0;
//     }
// 
//     icall = 0;
//     while (1) {
//               //f and g evaluation:
// 
//                 f = objToMinimize.Function(x);
//                 //objToMinimize.NumDerivatives(x,g);
//                 //std::cout << " numerical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";
//                 objToMinimize.Derivatives(x,g);
// //                 std::cout << "analytical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";
// 
//         
//         if (lbfgs_run(opt, &x[0], &f, &g[0]) <= 0)
//             break;
//         ++icall;
//         // We allow at most 2000 evaluations of F and G 
//         if (icall > 2000)
//             break;
//     }
// 
//     lbfgs_destroy(opt);





    int n = objToMinimize.ProblemSize();
    std::cout  << "Problem size: " << n << std::endl;


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

    int m = 10;

    lbfgsb_t* opt = lbfgsb_create(n, m, &l[0], &u[0], &nbd[0]);
    assert(opt);

    opt->iprint=-1; //no output

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
                objToMinimize.Derivatives(x,g);
//                 std::cout << "analytical grad: "; for(uint i=0; i<6;i++) std::cout << g[i] << "  "; std::cout <<"\n";

   
        } else {
            assert(!"can not reach here");
        }
    }


    opt->task[59]='\0'; //add a null terminating character to task[]


    std::cout << opt->task <<"\n";
    lbfgsb_destroy(opt);




}




} //namespace lbfgs






