#include <rigidbody.h>
#include <pdbio.h>
#include <iostream>
#include <BasePair.h>
#include <Movement.h>

using namespace std;
using namespace PTools;


BasePair::BasePair(std::string filename)
{
  ReadPDB(filename,rigbody);
}


BasePair::BasePair(Rigidbody rigbody)
{
  this->rigbody=rigbody;
}


BasePair::~BasePair()
{
  
}


string BasePair::printPDB()
{
  return rigbody.PrintPDB ();
}


void BasePair::apply(Movement m)
{
  m.apply(rigbody);
}


void BasePair::apply(Matrix m)
{
  apply(Movement (m));
}


Matrix BasePair::getMatrix()
{
  return rigbody.GetMatrix();
}


Movement BasePair::getMovement()
{
  return Movement(getMatrix());
}


string BasePair::getChainID()
{
  return rigbody.GetAtomProperty(0).GetChainId();
}


Rigidbody BasePair::getRigidBody()
{
  return rigbody;
}

  
void BasePair::setResID(int resID)
{
  for(unsigned int i =0; i< rigbody.Size(); i++)
  {
    Atomproperty ap=rigbody.GetAtomProperty(i);
    ap.SetResidId(resID); 
    rigbody.SetAtomProperty(i,ap);
  }
}


uint BasePair::getResID()
{
  Atomproperty ap = rigbody.GetAtomProperty(0);
  return ap.GetResidId();
}


void  BasePair::setRigidBody(Rigidbody rigbody)
{
  this->rigbody=rigbody;
}