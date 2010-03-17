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


BasePair::BasePair(const Rigidbody& rigbody)
{
  this->rigbody=rigbody;
}


BasePair::~BasePair()
{
  
}


string BasePair::printPDB()const
{
  return rigbody.PrintPDB ();
}


void BasePair::apply( const Movement& m)
{
  m.apply(rigbody);
}


void BasePair::apply(const Matrix& m)
{
  apply(Movement (m));
}


Matrix BasePair::getMatrix() const
{
  return rigbody.GetMatrix();
}


Movement BasePair::getMovement()const
{
  return Movement(getMatrix());
}


string BasePair::getChainID()const
{
  return rigbody.GetAtomProperty(0).GetChainId();
}


Rigidbody BasePair::getRigidBody()const
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


uint BasePair::getResID()const
{
  Atomproperty ap = rigbody.GetAtomProperty(0);
  return ap.GetResidId();
}


void  BasePair::setRigidBody(const Rigidbody& rigbody)
{
  this->rigbody=rigbody;
}