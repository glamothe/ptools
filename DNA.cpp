
#include <rigidbody.h>
#include <pdbio.h>
#include <superpose.h>
#include <atomselection.h>
#include <iostream>
#include <DNA.h>
#include <Movement.h>
#include <fstream>
#include <limits>
#include <limits.h>
#include "Parameter.h"

using namespace std;
using namespace PTools;


//constructor/destructor
DNA::DNA(string dataBaseFile, string seq, const Movement& mov)
{
  //if we want to build the dna from a model
  if(isPdbFile (seq))
  {
      buildDNAfromPDB ( dataBaseFile, seq );
  }
  else
  {
      assembleSeq (dataBaseFile,seq);
      applyInitialMov(mov);
  }
}


void DNA::buildDNAfromPDB (string dataBaseFile, string pdbFile )
{
    
    Rigidbody model = Rigidbody(pdbFile);
    renumberModel ( model);
    string seq =getSeq(model);
    assembleSeq (dataBaseFile,seq);

    placeBasePairs(model);
}


DNA::~DNA()
{

}


void DNA::placeBasePairs(const Rigidbody& model)
{
    if (strand[0].getRigidBody().SelectAtomType("GS2").Size()> 0)
    {
        placeBasePairs_CG(model);
    }
    else
    {
        placeBasePairs_AA(model);
    }

}


void DNA::placeBasePairs_CG(const Rigidbody& model)
{
    unsigned int DNASize  = (strand.size()*2)-1;
    unsigned int strandSize  = strand.size();
    
    for ( unsigned int i = 0; i < strandSize ; i++ )
    {
        Rigidbody modelOfBasePair = getModelOfBasePair( model, i, DNASize-i);

        strand[i].apply(getMatCG2AA ( modelOfBasePair,i ));
    }
}


void DNA::placeBasePairs_AA(const Rigidbody& model)
{
    unsigned int DNASize  = (strand.size()*2)-1;
    unsigned int strandSize  = strand.size();

    for ( unsigned int i = 0; i < strandSize ; i++ )
    {
        Rigidbody modelOfBasePair = getModelOfBasePair( model, i, DNASize-i );
        strand[i].apply(getMatAA2AA ( modelOfBasePair,i ));
    }
}


Matrix DNA::getMatAA2AA( const Rigidbody& modelOfBasePair,int pos)const{
    Parameter param =Parameter();
    return superpose (param.buildAxisAAGeometricCenter(modelOfBasePair),param.buildAxisAAGeometricCenter(strand[pos].getRigidBody())).matrix;
}


Matrix DNA::getMatCG2AA( const Rigidbody& modelOfBasePair,int pos)const{
    Parameter param =Parameter();
    return superpose (param.buildAxisAAGeometricCenter(modelOfBasePair),param.buildAxisCGGeometricCenter(strand[pos].getRigidBody())).matrix;
}


void DNA::renumberModel (Rigidbody& model)const
{

  unsigned int tempId=  model.GetAtomProperty(0).GetResidId();;
  unsigned int modelSize=model.Size();
  unsigned int nbRes=0;
  for (unsigned int i =0; i < modelSize; i++ )
  {
    unsigned int Id =  model.GetAtomProperty(i).GetResidId();
    if ( tempId != Id )
    {
        tempId =Id;
        nbRes++;        
    }
    Atomproperty ap=model.GetAtomProperty(i);
    ap.SetResidId(nbRes);
    model.SetAtomProperty(i,ap);
  }
}

Rigidbody DNA::getModelOfBasePair(const Rigidbody& model,int posA,int posB)const
{
        return (model.SelectResRange(posA, posA)|model.SelectResRange(posB, posB)).CreateRigid();


}


void DNA::assembleSeq (std::string dataBaseFile, std::string seq)
{
      Rigidbody dataBase = Rigidbody(dataBaseFile);      
      string chainIDs = getChainIDs(dataBase);

      //"map" for the rigidbody, an iD corespond to its rigidbody
      vector<Rigidbody> vbase = buildVbase(chainIDs,dataBase);
      //build the strand from the seq
      buildStrand(seq, chainIDs, vbase);
      //make sure that every BasePaire have a different id
      makeResIDs();
}

string DNA::getSeq ( const Rigidbody& model)const
{
    string seq;
    unsigned int strandSize = model.SelectAtomType("C1'").Size()/2;
    for ( unsigned int i=0 ; i< strandSize ; i++ )
    {
         string type = model.SelectResRange( i, i)[0].GetResidType();
         
         if      ( type.find ('A') != string::npos || type.find ('a') != string::npos ) seq+='A';
         else if ( type.find ('T') != string::npos || type.find ('t') != string::npos) seq+='T';
         else if ( type.find ('G') != string::npos || type.find ('g') != string::npos) seq+='G';
         else if ( type.find ('C') != string::npos || type.find ('c') != string::npos) seq+='C';
    }
    return seq;
}


vector<Rigidbody> DNA::buildVbase(string chainIDs, Rigidbody& dataBase)const
{
  vector<Rigidbody> vbase;
  unsigned int chainIDsSize = chainIDs.size();
  for (unsigned int i = 0; i < chainIDsSize ; i++)
  {

    vbase.push_back(dataBase.SelectChainId(chainIDs.substr(i,1)).CreateRigid());
  }
  return vbase;
}


string DNA::getChainIDs(const Rigidbody& rb)const
{
  string chainIDs;
  AtomSelection selection = rb.SelectAllAtoms ();
  string tmp = "";
  unsigned int selectionSize = selection.Size();

  for (unsigned int i=0; i < selectionSize ;i++)
  {
    string id = selection[i].GetChainId();
    if(id !=tmp)
    {
      tmp=id;
      chainIDs += id;
    }
  }
  return chainIDs;
}

bool DNA::isPdbFile (std::string seq) const
{
    return ( (seq.size() >=3) && (seq.substr(seq.size()-3,seq.size())=="pdb") );
}

void DNA::buildStrand(std::string seq, std::string chainIDs, const std::vector<Rigidbody>& vbase)
{
  unsigned int seqSize = seq.size();
  unsigned int chainIDsSize = chainIDs.size();

  for (unsigned int i =0; i < seqSize; i++ )
  {
    for (unsigned int j =0; j < chainIDsSize; j++ )
    {     
      if (seq[i] == chainIDs[j])
      {
	strand.push_back(BasePair(vbase[j]));
      }
    }    
  }  
}


void DNA::makeResIDs()
{
  unsigned int strandSize  = strand.size();
  for (unsigned int i =0; i < strandSize; i++ )
  {
    strand[i].setResID(i);
  }
}


void DNA::applyInitialMov(const Movement& mov)
{
  unsigned int strandSize  = strand.size();
  for (unsigned int i=1; i <strandSize; i++)
  {
    strand[i].apply(strand[i-1].getMovement());
    strand[i].apply(mov);
  }
}


unsigned int DNA::size()const
{
  return strand.size();
}


string DNA::printPDB()const
{
  string out;
  unsigned int strandSize  = strand.size();
  for ( unsigned int i =0; i < strandSize ; i++ )
  {
    out += strand[i].printPDB();
  }
  return out.substr(0,out.size()-1);
}


void DNA::writePDB(std::string fileName)const
{
  ofstream myfile;
  myfile.open(fileName.c_str());
  myfile << printPDB();
  myfile.close();
}


void DNA::changeRepresentation(std::string filename)
{
  Rigidbody all = Rigidbody(filename);
  string chainIDs = getChainIDs(all);
  
  //"map" for the rigidbody, an iD corespond to its rigidbody
  vector<Rigidbody> vbase;
  
  unsigned int chainIDsSize = chainIDs.size();
  for (unsigned int i = 0; i < chainIDsSize ; i++)
  {
    vbase.push_back(all.SelectChainId(chainIDs.substr(i,1)).CreateRigid());
  }
  
  unsigned int strandSize  = strand.size();
  for (unsigned int i = 0; i < strandSize ; i++)
  {
    Movement mov = Movement(strand[i].getMatrix());

    for (unsigned int j =0; j < chainIDsSize; j++ )
    {     
      if ( strand[i].getChainID()[0] == chainIDs[j])
      {
	strand[i]=BasePair(vbase[j]);
	strand[i].apply(mov);
      }
    }
  }
  
  makeResIDs();
}


Matrix DNA::getLocalMatrix(int pos)const
{
   if (pos == 0) return strand[0].getMatrix();
  
   Matrix mtot = strand[pos].getMatrix();
   Matrix prec = inverseMatrix44(strand[pos-1].getMatrix());

   return matrixMultiply( prec, mtot );
   
}


void DNA::applylocalMov(const Movement& mov,int pos)
{
  Matrix nextlocal = getLocalMatrix(pos+1);
  strand[pos].apply(mov);

  unsigned int strandSize  = strand.size();
  for (unsigned int i=pos+1; i <strandSize; i++)
  {
    nextlocal = reconstruct(i,nextlocal);
  }
}


void DNA::applyglobalMov(const Movement& mov)
{
  Matrix nextlocal;
  if (strand.size()>1){
    nextlocal = getLocalMatrix(1);
  }
  strand[0].apply(mov);

  unsigned int strandSize  = strand.size();
  for (unsigned int i=1; i <strandSize; i++)
  {
    nextlocal = reconstruct(i,nextlocal);
    strand[i].apply(mov);
  }  
    
}


void DNA::applyLocal(const Movement& mov,int posMov, int posAnchor)
{
  BasePair anchor = strand[posAnchor];
  applylocalMov(mov,posMov);
  relocate(anchor,posAnchor);
}


void DNA::applyLocal(const Matrix&  m,int posMov, int posAnchor)
{
  applyLocal ( Movement(m), posMov, posAnchor);
}


void DNA::applyGlobal(const Movement& mov ,int posAnchor)
{
  BasePair anchor = strand[posAnchor];
  applyglobalMov(mov);
  relocate(anchor,posAnchor);
}


void DNA::applyGlobal(const Matrix& m ,int posAnchor){
  applyLocal ( Movement(m), posAnchor);
}


void DNA::relocate(const BasePair& anchor,int posAnchor)
{
  Movement callback = Movement(superpose(anchor.getRigidBody(),strand[posAnchor].getRigidBody(),0).matrix);

  unsigned int strandSize  = strand.size();
  for (unsigned int i=0; i <strandSize; i++)
  {
    Rigidbody rb = strand[i].getRigidBody();
    rb.ApplyMatrix(callback.getMatrix());
    strand[i].setRigidBody(rb);
  }
}


Matrix DNA::reconstruct(int pos, const Matrix& local)
{
  Matrix nextlocal;
  if ((unsigned int)pos<strand.size()) nextlocal = getLocalMatrix(pos+1);
  
  Matrix prec = strand[pos-1].getMatrix();
  
  strand[pos].apply(inverseMatrix44(strand[pos].getMatrix()));//erasing the old matrix
  
  strand[pos].apply(prec);
  strand[pos].apply(local);
  
  return nextlocal;
}

