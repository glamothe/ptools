#include "lbfgs_interface.h"

#include <string.h>
#include <iostream>
#include <math.h>

namespace PTools{

inline void assign(char* dest, char* src)
{
    memcpy(dest,src,strlen(src));

}



Lbfgs::Lbfgs( ForceField& toMinim,  int problemsize, double* initialvector)
        :objToMinimize(toMinim)
{
    N=problemsize;
    X=initialvector; //this is not a copy!
    nMemCorr = 17 ; //default used by driver1.f
//    FACTR = 1e7 ;
    FACTR = 1e7 ;
//    FACTR = 1e1 ;
    PGTOL = 1.0e-3;


    //memory allocation (needed by the minimizer)
    int wa_size = 2*N*nMemCorr + 4*N + 12*N*N + 12*N ;
    WA = new double[wa_size];



    //memory allocation for L, H and NBD:
    //no lower or upper bound by default:
//    L= new double[N];
//    U= new double[N];
//    NBD=new int[N];
//    for (int i=0; i < N; i++) NBD[i]=0 ;

    G=new double[N];
    std::cout << G << std::endl;
    IWA = new int[3*N];
    IPRINT = 1 ; // one print every iteration

};


Lbfgs::~Lbfgs()
{
    std::cerr << "Appel du destructeur\n" ;
//    delete[] L;
//    delete[] U;
//    delete[] NBD;
    std::cout << G << std::endl;
    delete[] G;
    delete[] WA ;
    delete[] IWA ;
}


void Lbfgs::minimize(int maxiter=100)
{
    for (int i=0; i<60; i++) TASK[i]='\0' ;
    memcpy(TASK,"START",6);

    nMemCorr=5 ;

mainloop:

    setulb_( &N, &nMemCorr, X, L,
             U, NBD, &F , G ,
             &FACTR, &PGTOL, WA, IWA,
             TASK, &IPRINT, CSAVE, LSAVE,
             ISAVE, DSAVE);

    //  std::cout << "contenu de task: " << TASK << std::endl ;
    if ( TASK[0]=='F' && TASK[1]=='G')
    {
// //  std::cout << "\ndemande appel valeur+grad\n" ;
//           F = objToMinimize.Energy(X);
//
//         //calcul du gradient:
// //          objToMinimize.NumGradient(X,G);
//           objToMinimize.Gradient(X,G);
//
//
        F = objToMinimize.Function(X);
        objToMinimize.Derivatives(X,G);


        goto mainloop;
    }

    if (!strncmp(TASK,"NEW_X",5))
    {
        //     std::cout << "\n### et un NEW_X un\n" ;
        if (ISAVE[30]>maxiter)
        {
            memcpy(TASK,"STOP: Reached max iterations limit",35);
        }

        goto mainloop ;
    }
    std::cout << "\n***fin: contenu de TASK: " << TASK << "\n" ;

}


void Lbfgs::setLbounds(double* tabbounds)
{
    L =  tabbounds ;
}


void Lbfgs::setUbounds (double* tabbounds)
{
    U=tabbounds ;
}

void Lbfgs::setNBD(int*  nbd)
{
    NBD = nbd ;
}





} //namespace lbfgs









/*
int main() //test only !!!
{
    int taille = 25 ;
    double x[taille];
    double l[taille];
    double u[taille];
    int nbd[taille];

for (int i=0; i < taille; i+=2)
{
nbd[i]=2;
l[i]=1 ;
u[i]=100;
}

for (int i=1; i < taille; i+=2)
{
nbd[i] = 2 ;
l[i]= -100 ;
u[i]= 100 ;

}


    for (int i=0; i < taille; i++) x[i]=3 ;
    lbfgs::Lbfgs minf(taille,x ) ;
	minf.setNBD(nbd);
	minf.setLbounds(l);
	minf.setUbounds(u);

    minf.minimize();
    return 0;
}*/
