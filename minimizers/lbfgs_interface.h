#ifndef LBFGS_H
#define LBFGS_H

#include "../forcefield.h"

namespace PTools
{

namespace{
extern "C" {void setulb_(int* n,int *m, double* x, double* l,
                        double* u,int* nbd, double* f ,  double* g ,
                        double* factr, double* pgtol, double* wa, int* iwa,
                        char* task, int* iprint, char* csave,int* lsave,
                        int* isave,double* dsave );  }; //fortran minimizer
      }


class Lbfgs
{
      public:
            Lbfgs( ForceField& toMinim, int problemsize, double* initialvector);
            ~Lbfgs(); 
            void minimize(int maxiter);
	    void setLbounds(double * tabbounds); // set lower bounds
	    void setUbounds(double * tabbounds); // set upper bounds
	    void setNBD(int* nbd) ; //temporaire !!!


      private:
            
            ForceField& objToMinimize ; 

            
            // Variables used by the minimizer. 
            // comments are short because each var is 
            // well documented by the minimizer itself
            // (see driver1.f and routines.f for details)   

            int N; // number of variables
            int nMemCorr ; // number of corrections in the limited memory matrix (cf driver1.f)
            double * X; // size n .vector which contains variables values
            double * L; //size n .vector for lower bounds of each variable
            double * U; //size n. upper bounds
            int* NBD; //size n: which kind of bound for each var.
            double F; //function value
            double* G; // gradient values G[i]=dF/dx[i]
            double FACTR; // tolerance in the termination test
            double PGTOL; // gradient tolerance
            double* WA ; // reserved memory for internal use by the minimizer
            int* IWA ; //idem WA
            char TASK[60]     ; // for communication with minimizer drivers
            int IPRINT ; //options of minimizer verbosity
            char CSAVE[60]; // character working array of size 60
            int LSAVE[4];  // some final informations. cf driver1.h - boolean
            int ISAVE[44]; // minimization summary informations. See driver1.h for details
            double DSAVE[29]; //summary informations

} ;

}

#endif //#ifndef Lbfgs_H

