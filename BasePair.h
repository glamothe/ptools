
#ifndef BASE_PAIR_H
#define BASE_PAIR_H

#include <vector>  
#include <Movement.h>

namespace PTools
{
  
  class BasePair 
  {
    //atribut
    Rigidbody rigbody;
    
    public:
    BasePair(std::string filename);
    BasePair(Rigidbody rigbody);
    ~BasePair();
    
    std::string printPDB();
    
    void apply(Movement);
    void apply(Matrix);
    
    Matrix getMatrix();
    
    Movement getMovement();
    
    std::string getChainID();
    
    Rigidbody getRigidBody();
    void setRigidBody(Rigidbody);
    
    uint getResID();    
    void setResID(int);
  };
  
}//end namespace

#endif // BASE_PAIR_H 