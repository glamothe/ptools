
#ifndef DNA_H
#define DNA_H

#include <vector>  
#include <BasePair.h>
#include <Movement.h>

namespace PTools
{
  
  class DNA 
  {
    
    public:
    ///initialize a new object with a sequence and a database of pdb to pick from. an initial movement for the construction of the dna can be precised. 
    DNA(std::string,std::string,Movement mov = BDNA());
    ~DNA();
    
    ///return the number of BasePair
    int size();
    
    ///return a string containing the atoms data following the PDB format 
    std::string printPDB();
    
    ///write in a file the atoms data following the PDB format
    void writePDB(std::string);
    
    ///change the database use for the BasePair. the new database must contain the same names for pdb that the old one.
    void changeRepresentation(std::string);
    
    ///apply a Movement to a specified BasePair. you can specify an anchor 
    void applyLocal(Movement,int posMov, int posAnchor = 0);
    ///apply a Movement to all the BasePairs and reposition the DNA according to the anchor
    void applyGlobal(Movement,int posAnchor);

    ///apply a Matrix to a specified BasePair. you can specify an anchor 
    void applyLocal(Matrix,int posMov, int posAnchor = 0);
    ///apply a Matrix to all the BasePairs and reposition the DNA according to the anchor
    void applyGlobal(Matrix,int posAnchor);
    
    ///return the local Matrix of the specified BasePair (for the position i the Matrix to go from i-1 to i)
    Matrix getLocalMatrix(int pos);
    

    private:

    //attribut
    std::vector<BasePair> strand;
    
    //methods
    ///return a string with the seq of all the diferent chain ID of a Rigidbody.
    std::string getChainIDs(Rigidbody);
    ///construct the strand from the sequence, the  chain Ids and a corresponding vector of Rigidbody
    void buildStrand(std::string,std::string,std::vector<Rigidbody>);
    /// apply an initial Movement during the initialisation of the DNA
    void applyInitialMov(Movement);
    ///rebuild the DNA from a specified position. useful for taking into account a change in a base.
    Matrix reconstruct(int pos,Matrix);
    ///give the BasePair a correct number according to they rank in the strand
    void makeResIDs();
    ///apply a Movement to a specified BasePair.
    void applylocalMov(Movement,int pos);
    ///apply a Movement to all the BasePairs
    void applyglobalMov(Movement);
    ///reposition the DNA according to the anchor
    void relocate(BasePair anchor,int posAnchor);

  
  
  };
  
}//end namespace

#endif // DNA_H 