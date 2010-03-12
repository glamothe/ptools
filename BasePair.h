
#ifndef BASE_PAIR_H
#define BASE_PAIR_H

#include <vector>  
#include <Movement.h>

/*! \brief Rigidbody use by the DNA object
*
*/
namespace PTools
{
  
  class BasePair 
  {
    //atribut
    Rigidbody rigbody;
    
    public:
    ///initialize a new object with a file
    BasePair(std::string filename);
    ///initialize a new object from a regular Rigidbody
    BasePair(Rigidbody rigbody);
    ~BasePair();
    
    ///return a string containing the atoms data following the PDB format 
    std::string printPDB();
    
    /// apply a Movement to the BasePair 
    void apply(Movement);
    /// apply a Matrix to the BasePair
    void apply(Matrix);
    
    /// return the Matrix of the BasePair
    Matrix getMatrix();
    
    /// return the Movemeny of the BasePair
    Movement getMovement();
    
    /// return the ID of the chain of the BasePair (A for a AT, T for a TA, ...)
    std::string getChainID();
    
    /// return the Rigidbody of the BasePair
    Rigidbody getRigidBody();
    /// define the Rigidbody of the BasePair
    void setRigidBody(Rigidbody);
    
    /// return the Residue ID of the BasePair (it's rank in the DNA strand) 
    uint getResID();
    /// define the Residue ID of the BasePair (it's rank in the DNA strand)
    void setResID(int);
  };
  
}//end namespace

#endif // BASE_PAIR_H 