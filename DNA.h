
#ifndef DNA_H
#define DNA_H

#include <vector>  
#include <BasePair.h>
#include <Movement.h>
#include <Parameter.h>
#include "rigidbody.h"

namespace PTools
{
  
  class DNA 
  {
    
    public:
    ///initialize a new object with a sequence and a database of pdb to pick from. an initial movement for the construction of the dna can be precised. 
    DNA( std::string , std::string , const Movement & mov = BDNA());
    
    ~DNA();


    ///return the number of BasePair
    unsigned int size() const;
    
    ///return a string containing the atoms data following the PDB format 
    std::string printPDB() const;
    
    ///write in a file the atoms data following the PDB format
    void writePDB(std::string)const;
    
    ///change the database use for the BasePair. the new database must contain the same names for pdb that the old one.
    void changeRepresentation(std::string);
    
    ///apply a Movement to a specified BasePair. you can specify an anchor 
    void applyLocal(const Movement&,int posMov, int posAnchor = 0);
    ///apply a Movement to all the BasePairs and reposition the DNA according to the anchor
    void applyGlobal(const Movement&,int posAnchor);

    ///apply a Matrix to a specified BasePair. you can specify an anchor 
    void applyLocal(const Matrix&,int posMov, int posAnchor = 0);
    ///apply a Matrix to all the BasePairs and reposition the DNA according to the anchor
    void applyGlobal(const Matrix&,int posAnchor);
    
    ///return the local Matrix of the specified BasePair (for the position i the Matrix to go from i-1 to i)
    Matrix getLocalMatrix(int pos)const;

    

    private:

    //attribut
    std::vector<BasePair> strand;
    
    //methods
    ///return a string with the seq of all the diferent chain ID of a Rigidbody.
    std::string getChainIDs(const Rigidbody&)const;
    ///construct the strand from the sequence, the  chain Ids and a corresponding vector of Rigidbody
    void buildStrand(std::string,std::string,const std::vector<Rigidbody> &);
    /// apply an initial Movement during the initialisation of the DNA
    void applyInitialMov(const Movement&);
    ///rebuild the DNA from a specified position. useful for taking into account a change in a base.
    Matrix reconstruct(int pos,const Matrix&);
    ///give the BasePair a correct number according to they rank in the strand
    void makeResIDs();
    ///apply a Movement to a specified BasePair.
    void applylocalMov(const Movement&,int pos);
    ///apply a Movement to all the BasePairs
    void applyglobalMov(const Movement& );
    ///reposition the DNA according to the anchor
    void relocate(const BasePair& anchor,int posAnchor);

    ///initialize a new object from a PDB file
    void buildDNAfromPDB ( std::string database, std::string pdbfile);
    ///check if a file have a pdb extension
    bool isPdbFile (std::string)const;
    ///return the sequence of a DNA model
    std::string getSeq (const Rigidbody& model)const;
    ///return a vector with an example of each base
    std::vector<Rigidbody> buildVbase(std::string chainIDs, Rigidbody& dataBase)const;
    ///create the different base in strand following the seq order.
    void assembleSeq (std::string dataBaseFile, std::string seq);

    ///change the numerotation of the base in the model to conform to the following order
    ///A 1 2 3
    ///B 6 5 4
    void renumberModel (Rigidbody& model)const;

    ///place the base in the same position as the model
    void placeBasePairs(const Rigidbody& model);

    ///place the base pair in coarse grain in the same position as the model
    void placeBasePairs_CG(const Rigidbody& model);
    ///place the base pair in all atom in the same position as the model
    void placeBasePairs_AA(const Rigidbody& model);

    ///return the base pair composed of the base on posA in the strand A of model and the base on posB in the strand B of model
    Rigidbody getModelOfBasePair(const Rigidbody& model,int posA,int posB)const;
    ///return the matrix betwen a model base pair and the basepair (in all atom) on pos in strand
    Matrix getMatAA2AA(const Rigidbody& modelOfBasePair,int pos)const;
    ///return the matrix betwen a model base pair and the basepair (in coarse grain) on pos in strand
    Matrix getMatCG2AA(const Rigidbody& modelOfBasePair,int pos)const;
  };
  
}//end namespace

#endif // DNA_H 