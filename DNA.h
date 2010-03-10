
#ifndef DNA_H
#define DNA_H

#include <vector>  
#include <BasePair.h>
#include <Movement.h>

namespace PTools
{
  
  class DNA 
  {
    //atribut
    std::vector<BasePair> strand;
    
    //method
    std::string getChainIDs(Rigidbody);
    void buildStrand(std::string,std::string,std::vector<Rigidbody>);
    void applyInitialMov(Movement);
    Matrix reconstruct(int pos,Matrix);
    void makeResIDs();
    void applylocalMov(Movement,int pos);
    void applyglobalMov(Movement);
    void relocate(BasePair anchor,int posAnchor);
    
    public:
    DNA(std::string,std::string,Movement mov = BDNA());
    ~DNA();
    
    int size();
    
    std::string printPDB();
    
    void writePDB(std::string);
    
    void changeRepresentation(std::string);
    
    void applyLocal(Movement,int posMov, int posAnchor = 0);
    void applyGlobal(Movement,int pos);
    //adaptation for accepting matrix
    void applyLocal(Matrix,int posMov, int posAnchor = 0);
    void applyGlobal(Matrix,int posAnchor);
    
    Matrix getLocalMatrix(int pos);
    
  };
  
}//end namespace

#endif // DNA_H 