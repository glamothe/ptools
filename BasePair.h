
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
    public:
    ///initialize a new object with a file
    BasePair(std::string filename);
    ///initialize a new object from a regular Rigidbody
    BasePair(const Rigidbody& rigbody);
    ~BasePair();
    
    ///return a string containing the atoms data following the PDB format 
    std::string printPDB() const;

    ///return a string containing the atoms data of a base (identified by its chain) following the PDB format
    std::string printPDBofBase(std::string chain) const;

    ///change the chainID of the internal bases to 'A' for the first base (coresponding to the type) and 'B' for the second one
    void setChainID();

    /// apply a Movement to the BasePair 
    void apply(const Movement& );
    /// apply a Matrix to the BasePair
    void apply(const Matrix&);
    
    /// return the Matrix of the BasePair
    Matrix getMatrix()const;
    
    /// return the Movemeny of the BasePair
    Movement getMovement()const;
    
    /// return the Rigidbody of the BasePair
    Rigidbody getRigidBody()const;
    /// define the Rigidbody of the BasePair
    void setRigidBody(const Rigidbody&);
    
    /// return the Residue ID of the BasePair (it's rank in the DNA strand) 
    uint getResID()const;
    /// define the Residue ID of the BasePair (it's rank in the DNA strand)
    void setResID(int);
    
    std::string getType() const;

    void setType(std::string type);
    
    private:
    //atribut
    Rigidbody rigbody;
    std::string type;
    
  };
  
}



#endif // BASE_PAIR_H 