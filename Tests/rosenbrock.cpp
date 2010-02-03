//test program for the new minimizer
//rosenbrock function

#include "forcefield.h"
#include "lbfgs_interface.h"


using namespace PTools;

class MyFF: public ForceField
{
  public: 
    dbl Function(const Vdouble& x)
     {
        return (1-x[0])*(1-x[0]) + 100* (x[1] - x[0]*x[0])*(x[1] - x[0]*x[0]);
     }

    void Derivatives(const Vdouble& X, Vdouble& delta)
    {
      
        double t = 100 * 2 * (X[1] - X[0]*X[0]);
	
        delta[0] = -2*( 1 - X[0]) + t * -2 * X[0];
	delta[1] = t ;
    }

    unsigned int ProblemSize() { return 2;}
     
    void initMinimization() {}
    
        
     
};

int main()
{
  MyFF ff; 
  Lbfgs lb(ff);
   
  lb.minimize(50);
  
  
  
 
}
