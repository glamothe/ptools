
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

DNA::DNA( const DNA& model )
{
    unsigned int modelSize  = model.size();
    for (uint i =0; i< modelSize; i++)
    {
        strand.push_back(model[i]);
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
    unsigned int DNASize  = (strand.size()*2)-1;
    unsigned int strandSize  = strand.size();
    Parameter param =Parameter();
    for ( unsigned int i = 0; i < strandSize; i++ )// strandSize
    {
        Rigidbody modelOfBasePair = getModelOfBasePair( model, i, DNASize-i);
        strand[i].apply(getMatBetwenBasePair ( modelOfBasePair,i ));
    }
}


Matrix DNA::getMatBetwenBasePair( const Rigidbody& modelOfBasePair,int pos)const{
    Parameter param =Parameter();
    Superpose_t sup =superpose (param.buildAxisCentered(modelOfBasePair),param.buildAxisCentered(strand[pos].getRigidBody()));
    return sup.matrix;
}


void DNA::renumberModel (Rigidbody& model)const
{

  unsigned int tempId=  model.GetAtomProperty(0).GetResidId();
  string tempChain=model.GetAtomProperty(0).GetChainId();
  unsigned int nbRes=0;
  unsigned int second = 0;

  bool isjumna = isJumna(model);

  unsigned int modelSize=model.Size();
  for (unsigned int i =0; i < modelSize; i++ )
  {
    Atomproperty ap=model.GetAtomProperty(i);
    unsigned int Id = ap.GetResidId();
    if ( tempId != Id )
    {
        if (isjumna && tempChain!= ap.GetChainId())
        {
            if (second == 0)
            {
                second=nbRes*2 +1;
            }
            nbRes= second--;
            tempId =Id;
        }
        else
        {
            tempId =Id;
            nbRes++;
        }
    }
    ap.SetResidId(nbRes);
    model.SetAtomProperty(i,ap);
  }
}

bool DNA::isJumna (const Rigidbody& model)const
{
   //jumna have an inversed numerotation so on a 10 base dna (strand A: 0,1,2,3,4 and strand B:5,6,7,8,9)
   //the base 0 is associated with 5 instead of 9.
   //to check wich convention is followed, I'm gonna test the distance between 0-5 and 0-9, the shorter being the correct coupling
    AtomSelection sel = model.SelectAtomType("C1'");
    if (sel.Size() == 0) sel = model.SelectAtomType("C1*");
    if (sel.Size() == 0) sel = model.SelectAtomType("GS1");


    double d1 = Dist(sel[0],sel[sel.Size()-1]);
    double d2 = Dist(sel[0],sel[sel.Size()/2]);

    return (d1>d2);
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
      changeFormat();
}

string DNA::getSeq ( const Rigidbody& model)const
{
    string seq;
    unsigned int strandSize;
    if ((model.SelectAtomType("C1'").Size()/2) >0)
    {
        strandSize = model.SelectAtomType("C1'").Size()/2;
    }else if ((model.SelectAtomType("C1*").Size()/2) >0)
    {
        strandSize = model.SelectAtomType("C1*").Size()/2;
    }else if ((model.SelectAtomType("GS1").Size()/2) >0)
    {
        strandSize = model.SelectAtomType("GS1").Size()/2;
    }else {return "";}
    for ( unsigned int i=0 ; i< strandSize ; i++ )
    {
         string type = model.SelectResRange( i, i)[0].GetResidType();
         
         if      ( type.find ('A') != string::npos || type.find ('a') != string::npos) seq+='A';
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
    return ( (seq.size() >=3) && ((seq.substr(seq.size()-3,seq.size())=="pdb") ||(seq.substr(seq.size()-3,seq.size())=="red")) );
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


void DNA::changeFormat()
{
  unsigned int nbAtom = 0;
  unsigned int strandSize  = strand.size();
  for (unsigned int i =0; i < strandSize; i++ )
  {
    //corect ID chain
    strand[i].setChainID();
    //numerotation residu
    strand[i].setResID(i,(strandSize + strandSize-1)-i);
    //numerotation atom (first part)
    nbAtom = strand[i].setAtomNumberOfBase("A",nbAtom);

  }
  for (unsigned int i = strandSize-1; i > 0 ; i-- )
  {
    //numerotation atom (second part)
    nbAtom = strand[i].setAtomNumberOfBase("B",nbAtom);
  }
  //last part of atom numerotation (because of error with decreasing unsigned int)
  strand[0].setAtomNumberOfBase("B",nbAtom);
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
  string strandA, strandB;
  unsigned int strandSize  = strand.size();
  for ( unsigned int i =0; i < strandSize ; i++ )
  {
    strandA += strand[i].printPDBofBase("A");
    strandB += strand[strandSize-1-i].printPDBofBase("B");
  }
  string out= strandA + strandB;
  return out.substr(0,out.size()-1);
}

std::string DNA::printPDBofStrand( std::string chain ) const
{
  string out;
  unsigned int strandSize  = strand.size();
  for ( unsigned int i =0; i < strandSize ; i++ )
  {
    out += strand[i].printPDBofBase( chain );
  }
  return out.substr(0,out.size()-1);
}

string DNA::printParam() const
{
  stringstream ss;
  unsigned int strandSize  = strand.size();
  for ( unsigned int i =1; i < strandSize ; i++ )
  {
    ss << "BasePair "<< i-1 <<"->"<<i<< " : "<<getLocalParameter(i).toFormatedString()+"\n";
  }
  return ss.str().substr(0,ss.str().size()-1);
}


void DNA::writePDB(std::string fileName)const
{
  ofstream myfile;
  myfile.open(fileName.c_str());
  myfile << printPDB();
  myfile.close();
}

Rigidbody DNA::createRigid()const
{
  //we use the method createRigidOfStrand() to make sure that the rigidbody returned follow the established format
  Rigidbody chainA = createRigidOfStrand("A");
  Rigidbody chainB = createRigidOfStrand("B");

  //we add the atoms of the chain B to the chainA
  unsigned int chainBsize  = chainB.Size();
  for ( unsigned int i =0; i < chainBsize ; i++ )
  {
      chainA.AddAtom(chainB.CopyAtom(i));
  }
  return chainA;
}

Rigidbody DNA::createRigidOfStrand(std::string chain)const
{
      Rigidbody dna;
  unsigned int strandSize  = strand.size();


  for ( unsigned int i =0; i < strandSize ; i++ )
  {
      Rigidbody basePair;
      if (chain == "B" || chain == "b")
      {
          basePair = strand[strandSize-1-i].getRigidBodyOfBase(chain);
      }
      else
      {
          basePair = strand[i].getRigidBodyOfBase(chain);
      }
      unsigned int basePairSize  = basePair.Size();
      for (unsigned int j=0; j <basePairSize ; j++)
      {
          Atom a =basePair.CopyAtom(j);
          dna.AddAtom(a);
      }
  }
  return dna;
}

void DNA::changeRepresentation(std::string dataBaseFile)
{
  Rigidbody dataBase = Rigidbody(dataBaseFile);
  string chainIDs = getChainIDs(dataBase);
  
  //"map" for the rigidbody, an iD corespond to its rigidbody
  vector<Rigidbody> vbase = buildVbase(chainIDs,dataBase);

    unsigned int chainIDsSize = chainIDs.size();

  unsigned int strandSize  = strand.size();
  for (unsigned int i = 0; i < strandSize ; i++)
  {
    Movement mov = Movement(strand[i].getMatrix());

    for (unsigned int j =0; j < chainIDsSize; j++ )
    {     
      if ( strand[i].getType()[0] == chainIDs[j])
      {
	strand[i]=BasePair(vbase[j]);
	strand[i].apply(mov);
      }
    }
  }
  
  changeFormat();
}


Matrix DNA::getLocalMatrix(int pos)const
{
   if (pos == 0) return strand[0].getMatrix();
  
   Matrix mtot = strand[pos].getMatrix();
   Matrix prec = inverseMatrix44(strand[pos-1].getMatrix());

   return matrixMultiply( prec, mtot );
   
}

Parameter DNA::getLocalParameter(int pos)const
{
  Parameter param =Parameter();
  param.MeasureParameters(param.buildAxisCentered(strand[pos-1].getRigidBody()),param.buildAxisCentered(strand[pos].getRigidBody()));
  return param;

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
  unsigned int strandSize  = strand.size();
  if (strandSize>1){
    nextlocal = getLocalMatrix(1);
  }
  strand[0].apply(mov);

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


void DNA::applyGlobal(const Matrix& m ,int posAnchor)
{
  applyLocal ( Movement(m), posAnchor);
}


void DNA::apply(const Movement& mov)
{
    DNA::apply(mov.getMatrix());
}


void DNA::apply(const Matrix& m)
{
  unsigned int strandSize  = strand.size();
  for (unsigned int i=0; i <strandSize; i++)
  {
    Rigidbody rb = strand[i].getRigidBody();
    rb.ApplyMatrix(m);
    strand[i].setRigidBody(rb);
  }
}



def rmsd(model, dna):
	rigmodel = model.createRigid()
	rigdna = dna.createRigid()
	total = 0.0
	for i in xrange(0, rigmodel.Size()):
		atom1=rigmodel.CopyAtom(i)
		atom2=rigdna.CopyAtom(i);

		total+=Dist2(atom1,atom2);


	return sqrt(total/(float(rigmodel.Size()))) ;



void DNA::relocate(const BasePair& anchor,int posAnchor)
{
  apply(superpose(anchor.getRigidBody(),strand[posAnchor].getRigidBody(),0).matrix);
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

