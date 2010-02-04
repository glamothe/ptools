#include "lbfgs_interface.h"

#include <string.h>
#include <iostream>
#include <math.h>
#include <float.h>


// #include <lbfgs.h>

//#include "../complexify.h"

namespace PTools{

inline void assign(char* dest, char* src)
{
    memcpy(dest,src,strlen(src));

}






Lbfgs::Lbfgs( ForceField& toMinim)
        :objToMinimize(toMinim)
{
    //let the object do some initialization before beginning a new minimization
    //(for example, create new pairlists...)

    objToMinimize.initMinimization();
    m_n = objToMinimize.ProblemSize();
    
    
    /*lbfgsfloatval_t *m_x = lbfgs_malloc(m_n);
       
     if (m_x == NULL) 
     {
            printf("ERROR: Failed to allocate a memory block for variables.\n");
            return;
     }
    */
    
    
    
    
         
     
    
};


Lbfgs::~Lbfgs()
{
//     if (m_x) lbfgs_free(m_x);
}


// vector<double> to vector<double> converter. used for genericity. should not impact performances too much.
inline void tocplx(const std::vector<double> & vdblin, std::vector<double> & vdblout ){vdblout=vdblin;};



#ifdef AUTO_DIFF
//convert from vector<cplx> to vector<double>
inline void tocplx(const std::vector<double> & vdblin, std::vector<surreal> & vcplx)
{
    vcplx = std::vector<surreal>();
    for (uint i=0; i<vdblin.size(); i++)
    {
        vcplx.push_back(vdblin[i]);
    }
}

#endif

inline std::vector<double> todbl(std::vector<double> & vdbl) {return vdbl;};

#ifdef AUTO_DIFF
inline std::vector<double> todbl(std::vector<surreal> & vcplx)
{
    std::vector<double> vdbl;
    for (uint i=0; i<vcplx.size(); i++)
    {
        vdbl.push_back(real(vcplx[i]));
    }
    return vdbl;
};

#endif


    static lbfgsfloatval_t _evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step
        )
    {
        ForceField* ff = reinterpret_cast<ForceField*>(instance);
	std::vector<lbfgsfloatval_t> xx(n);
	std::vector<lbfgsfloatval_t> gg(n);
	
	for (int i=0; i<n; i++)
	{
	 xx[i]=x[i];
	 gg[i]=g[i];
	}
	
        double val = ff-> Function(xx);
	ff-> Derivatives(xx,gg);
	
	double gsum = 0;
	for (int i = 0; i<n; i++)
	{
	 g[i]=gg[i]; 
	 gsum+=g[i]*g[i];
	}
        gsum = sqrt(gsum);
        
	std::cout << "x: " ;
	for (int i=0; i<n; i++)
            std::cout << xx[i] << "  " ;
        std::cout << "\n  value: " << val ;
	std::cout << "    |g|: " << gsum << std::endl;
	std::cout << "---\n";
	
	
        return val;
    }










void Lbfgs::minimize(int maxiter)
{
    
    const unsigned int n = m_n;
    std::cout  << "number of free variables for the minimizer: " << m_n << std::endl;
    
    m_iter = 0;
    
    m_x.resize(n);
    m_g.resize(n);

    for(int i=0; i<m_x.size(); i++) m_x[i]=0.0;
 

    lbfgs_parameter_t param;
    lbfgs_parameter_init(&param);    
    
//     std::cout << "params: ftol gtol xtol: " << param.ftol << "  " << param.gtol << "  " << param.xtol << std::endl;
    param.epsilon = 1e-3 ;
    param.max_iterations = maxiter ;    
    
    lbfgsfloatval_t fx;
   
//     int ret = lbfgs(m_n, &m_x[0], &fx, _evaluate, _progress, &objToMinimize, params);
    int ret = lbfgs(m_n, &m_x[0], &fx, _evaluate, NULL, &objToMinimize, &param);
    
    std::cout << ret << std::endl;


//    f = objToMinimize.Function(vdblx);
//     objToMinimize.Derivatives(vdblx,vdblg);


}




std::vector<double>Lbfgs::GetMinimizedVarsAtIter(uint iter)
{
if (iter>=m_vars_over_time.size())
  {
   std::string msg = "";
   msg+= iter;
   msg += " is out of range (max: ";
   msg += m_vars_over_time.size()-1 ;
   msg += " )\n"; 
   throw std::out_of_range(msg);
  }
return m_vars_over_time[iter];
}






} //namespace lbfgs






