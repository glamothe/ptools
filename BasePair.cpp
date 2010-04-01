#include <rigidbody.h>
#include <pdbio.h>
#include <iostream>
#include <BasePair.h>
#include <Movement.h>

#include "atomselection.h"

using namespace std;
using namespace PTools;


BasePair::BasePair(std::string filename)
{
  ReadPDB(filename,rigbody);
  type = rigbody.GetAtomProperty(0).GetResidType();
}


BasePair::BasePair(const Rigidbody& rigbody)
{
  this->rigbody=rigbody;
  type = rigbody.GetAtomProperty(0).GetResidType();
}


BasePair::~BasePair()
{
  
}


string BasePair::printPDB()const
{
  return rigbody.PrintPDB ();
}

std::string BasePair::printPDBofBase(std::string chain) const
{
    return rigbody.SelectChainId(chain).CreateRigid().PrintPDB();
}

void BasePair::setChainID(){
  unsigned int rigSize=rigbody.Size();
  for(unsigned int i =0; i< rigSize ; i++)
  {
    Atomproperty ap=rigbody.GetAtomProperty(i);
    if (ap.GetResidType()==type)
    {
        ap.SetChainId("A");
    }
    else
    {
        ap.SetChainId("B");
    }
    rigbody.SetAtomProperty(i,ap);
  }
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


Rigidbody BasePair::getRigidBody()const
{
  return rigbody;
}

  
void BasePair::setResID(int resID)
{
  unsigned int rigSize=rigbody.Size();
  for(unsigned int i =0; i< rigSize ; i++)
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

string BasePair::getType() const {
    return type;
}

void BasePair::setType(string type) {
    this->type = type;
}
//end namespace