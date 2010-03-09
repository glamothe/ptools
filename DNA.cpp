
#include <ptools.h>
#include <iostream>
#include <DNA.h>
#include <Movement.h>
#include <fstream>

using namespace std;
using namespace PTools;


//constructor/destructor
DNA::DNA(string filename, string seq, Movement mov)
{
  Rigidbody all = Rigidbody(filename);
  string chainIDs = getChainIDs(all);
  
  //"map" for the rigidbody, an iD corespond to its rigidbody
  vector<Rigidbody> vbase;
  for (unsigned int i = 0; i < chainIDs.size() ; i++)
  {
    vbase.push_back(all.SelectChainId(chainIDs.substr(i,1)).CreateRigid());
  }
  
  //build the strand from the seq
  buildStrand(seq, chainIDs, vbase);
  
  //make sure that every BasePaire have a different id
  makeResIDs();
  
  applyInitialMov(mov);
}


DNA::~DNA()
{
  
}


string DNA::getChainIDs(Rigidbody rb)
{
  string chainIDs;
  AtomSelection selection = rb.SelectAllAtoms ();
  string tmp = "";
  for (unsigned int i=0; i < selection.Size() ;i++)
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


void DNA::buildStrand(std::string seq, std::string chainIDs, std::vector<Rigidbody> vbase)
{
  for (unsigned int i =0; i < seq.size(); i++ )
  {
    for (unsigned int j =0; j < chainIDs.size(); j++ )
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
  for (unsigned int i =0; i < strand.size(); i++ )
  {
    strand[i].setResID(i);
  }
}


void DNA::applyInitialMov(Movement mov)
{
  for (unsigned int i=1; i <strand.size(); i++)
  {
    strand[i].apply(strand[i-1].getMovement());
    strand[i].apply(mov);
  }
}


int DNA::size()
{
  return strand.size();
}


string DNA::printPDB()
{
  string out;
  for ( unsigned int i =0; i < strand.size(); i++ )
  {
    out += strand[i].printPDB();
  }
  return out;
}


void DNA::writePDB(std::string fileName)
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
  for (unsigned int i = 0; i < chainIDs.size() ; i++)
  {
    vbase.push_back(all.SelectChainId(chainIDs.substr(i,1)).CreateRigid());
  }
  
  for (unsigned int i = 0; i < strand.size() ; i++)
  {
    Movement mov = Movement(strand[i].getMatrix());
    for (unsigned int j =0; j < chainIDs.size(); j++ )
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


Matrix DNA::getLocalMatrix(int pos)
{
   if (pos == 0) return strand[0].getMatrix();
  
   Matrix mtot = strand[pos].getMatrix();
   Matrix prec = inverseMatrix44(strand[pos-1].getMatrix());

   return matrixMultiply( prec, mtot );
   
}


void DNA::applylocalMov(Movement mov,int pos)
{
  Matrix nextlocal = getLocalMatrix(pos+1);
  strand[pos].apply(mov);
  for (unsigned int i=pos+1; i <strand.size(); i++)
  {
    nextlocal = reconstruct(i,nextlocal);
  }
}


void DNA::applyglobalMov(Movement mov)
{
  Matrix nextlocal;
  if (strand.size()>1){
    nextlocal = getLocalMatrix(1);
  }
  strand[0].apply(mov);
  
  for (unsigned int i=1; i <strand.size(); i++)
  {
    nextlocal = reconstruct(i,nextlocal);
    strand[i].apply(mov);
  }  
    
}


void DNA::applyLocal(Movement mov,int posMov, int posAnchor)
{
  BasePair anchor = strand[posAnchor];
  applylocalMov(mov,posMov);
  relocate(anchor,posAnchor);
}


void DNA::applyLocal(Matrix m,int posMov, int posAnchor)
{
  applyLocal ( Movement(m), posMov, posAnchor);
}


void DNA::applyGlobal(Movement mov ,int posAnchor)
{
  BasePair anchor = strand[posAnchor];
  applyglobalMov(mov);
  relocate(anchor,posAnchor);
}


void DNA::applyGlobal(Matrix m ,int posAnchor){
  applyLocal ( Movement(m), posAnchor);
}


void DNA::relocate(BasePair anchor,int posAnchor)
{
  Movement callback = Movement(superpose(anchor.getRigidBody(),strand[posAnchor].getRigidBody(),0).matrix);
  
  for (unsigned int i=0; i <strand.size(); i++)
  {
    Rigidbody rb = strand[i].getRigidBody();
    rb.ApplyMatrix(callback.getMatrix());
    strand[i].setRigidBody(rb);
  }
}


Matrix DNA::reconstruct(int pos, Matrix local)
{
  Matrix nextlocal;
  if ((unsigned int)pos<strand.size()) nextlocal = getLocalMatrix(pos+1);
  
  Matrix prec = strand[pos-1].getMatrix();
  
  strand[pos].apply(inverseMatrix44(strand[pos].getMatrix()));//erasing the old matrix
  
  strand[pos].apply(prec);
  strand[pos].apply(local);
  
  return nextlocal;
}

