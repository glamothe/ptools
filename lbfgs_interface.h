#ifndef LBFGS_H
#define LBFGS_H

#include "basetypes.h"
#include "forcefield.h"

#include "liblbfgs-1.9/include/lbfgs.h"
// #include "lbfgs_wrapper/lbfgsb.h"




namespace PTools
{



// new version, from sdrive.c
// no bounds !!
class Lbfgs
{
      public:
            Lbfgs(ForceField& toMinim);
            ~Lbfgs();
            void minimize(int maxiter);
            std::vector<double> GetMinimizedVars() const {return m_x;};

            std::vector<double> GetMinimizedVarsAtIter(uint iter);
            int GetNumberIter() {return m_iter;}



      private:

            ForceField& objToMinimize ;
	    std::vector<lbfgsfloatval_t> m_x;
            std::vector<lbfgsfloatval_t> m_g; // gradient
            
            int m_iter;



            std::vector<std::vector<double> > m_vars_over_time;
	    unsigned int m_n ; // problem size



} ;

}

#endif //#ifndef Lbfgs_H

