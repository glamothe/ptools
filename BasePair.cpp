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


string BasePair::PrintPDB()const
{
  return rigbody.PrintPDB ();
}

std::string BasePair::PrintPDBofBase(std::string chain) const
{
    return rigbody.SelectChainId(chain).CreateRigid().PrintPDB();
}

void BasePair::SetChainID(){
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

void BasePair::Apply( const Movement& m)
{
  m.Apply(rigbody);
}


void BasePair::Apply(const Matrix& m)
{
  Apply(Movement (m));
}

unsigned int BasePair::Size() const{
    return rigbody.Size();
}

Matrix BasePair::GetMatrix() const
{
  return rigbody.GetMatrix();
}


Movement BasePair::GetMovement()const
{
  return Movement(GetMatrix());
}


Rigidbody BasePair::GetRigidBody()const
{
  return rigbody;
}


Rigidbody BasePair::GetRigidBodyOfBase(std::string chain)const
{
  return rigbody.SelectChainId(chain).CreateRigid();
}


void BasePair::SetResID(int idA,int idB)
{
  unsigned int baseSize=rigbody.Size();
  for(unsigned int i =0; i< baseSize ; i++)
  {
    Atomproperty ap=rigbody.GetAtomProperty(i);
    if (ap.GetChainId()=="A")
    {
        ap.SetResidId(idA);
    }
    else
    {
        ap.SetResidId(idB);
    }     
    rigbody.SetAtomProperty(i,ap);
  }
}

uint BasePair::SetAtomNumberOfBase(std::string chain,int num)
{
  unsigned int baseSize=rigbody.Size();
  for(unsigned int i =0; i< baseSize ; i++)
  {
    Atomproperty ap=rigbody.GetAtomProperty(i);
    if (ap.GetChainId()==chain)
    {
        ap.SetAtomId(num);
        num++;
        rigbody.SetAtomProperty(i,ap);
    }
  }
  return num;
}

uint BasePair::GetResIDofBase(std::string chain)const
{
  Atomproperty ap = rigbody.SelectChainId(chain).CreateRigid().GetAtomProperty(0);
  return ap.GetResidId();
}


void  BasePair::SetRigidBody(const Rigidbody& rigbody)
{
  this->rigbody=rigbody;
}

string BasePair::GetType() const {
    return type;
}

void BasePair::SetType(string type) {
    this->type = type;
}
//end namespace