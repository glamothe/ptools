// $Id$
//
// C++ Interface: atomselection
//
// Description:
//
//

#ifndef ATOMSELECTION_H
#define ATOMSELECTION_H

#include "rigidbody.h"

namespace PTools {

template <class RigidType>
class AtomSelection_t{

private:
    //private data
    const RigidType* m_rigid;
    std::vector<uint> m_list;

    //private methods



public:
    AtomSelection_t(){};
    AtomSelection_t(const AtomSelection_t& oldsel)
{
    this->m_rigid = oldsel.m_rigid;
    this->m_list = oldsel.m_list;
}

    /// translate a Rigidbody object into an AtomSelection_t
    AtomSelection_t(const RigidType& rigid)
	{
	m_rigid=&rigid;
	for (uint i=0; i < rigid.Size(); i++)
	{
		this->AddAtomIndex(i);
	}
	}



    ~AtomSelection_t(){};

    uint Size() const {return m_list.size();}; ///< returns the size of the selection
    void SetRigid(const RigidType& rig) {m_rigid=&rig;};

    /// return the i-th atom of the list
    Atom operator[] (uint i) const {
          if (i>=this->Size()) throw std::range_error("AtomSelection_t: array out of bounds");
          return m_rigid->CopyAtom(m_list[i]);}; 

    Atom CopyAtom(uint i) const {return m_rigid->CopyAtom(m_list[i]);}
    void AddAtomIndex(uint i) {m_list.push_back(i);}; ///< adds an Atom index


    RigidType CreateRigid() ///< makes a new rigidcopy (independant copy) from an AtomsSlection object.
{
    RigidType newrigid;
    for (uint i=0; i<this->Size(); i++)
    {
        Atom at = m_rigid->CopyAtom(m_list[i]);
        newrigid.AddAtom(at);
    }

    return newrigid;
}



    AtomSelection_t non(const AtomSelection_t& atsel)
{
return !atsel;
}


    //friends:
    friend AtomSelection_t<RigidType> operator& (const AtomSelection_t<RigidType>& atsel1, const AtomSelection_t<RigidType>& atsel2)
{
    AtomSelection_t<RigidType> selout;
    if (atsel1.m_rigid != atsel2.m_rigid)
    {
        selout.m_rigid=0;
        std::cout << "Warning: tring to find the intersection of two different rigidbody" << std::endl;
        return selout;
    }
    //else:

    selout.m_rigid = atsel1.m_rigid;
    set_intersection(atsel1.m_list.begin(), atsel1.m_list.end(),
                     atsel2.m_list.begin(), atsel2.m_list.end(), back_inserter(selout.m_list));

    return selout;
}






    friend AtomSelection_t<RigidType> operator| (const AtomSelection_t<RigidType>& atsel1, const AtomSelection_t<RigidType>& atsel2)
{
    AtomSelection_t<RigidType> selout;
    AtomSelection_t<RigidType> cpatsel1(atsel1); //makes a copy of atsel1
    AtomSelection_t<RigidType> cpatsel2(atsel2); // makes a copy of atsel2

    if (atsel1.m_rigid != atsel2.m_rigid)
    {
        selout.m_rigid=0;
        std::cout<<"Warning: for now you should not make union of two different rigidbody this way!" << std::endl;
        return selout;
    }
    //else:
    selout.m_rigid = atsel1.m_rigid;
    sort(cpatsel1.m_list.begin(), cpatsel1.m_list.end());
    sort(cpatsel2.m_list.begin(), cpatsel2.m_list.end());
    set_union(cpatsel1.m_list.begin(), cpatsel1.m_list.end(),
              cpatsel2.m_list.begin(), cpatsel2.m_list.end(), back_inserter(selout.m_list));

    std::vector<uint> list2;
    unique_copy(selout.m_list.begin(),selout.m_list.end(),back_inserter(list2));
    swap(selout.m_list, list2);

    return selout;
}








    friend AtomSelection_t<RigidType> operator! (const AtomSelection_t<RigidType>& seltoinverse)
    {

      AtomSelection_t<RigidType>selout;
      selout.SetRigid(*seltoinverse.m_rigid);
      AtomSelection_t<RigidType> all = seltoinverse.m_rigid->SelectAllAtoms();
      set_difference(all.m_list.begin(), all.m_list.end(),
                     seltoinverse.m_list.begin(), seltoinverse.m_list.end(),
                     back_inserter(selout.m_list));

      return selout;
    }


};


typedef AtomSelection_t<Rigidbody> AtomSelection;



//     template <class T>
//     AtomSelection_t<T> operator& (const AtomSelection_t<T>& atsel1, const AtomSelection_t<T>& atsel2);
// 
//     template <class T>
//     AtomSelection_t<T> operator| (const AtomSelection_t<T>& atsel1, const AtomSelection_t<T>& atsel2);
// 
//     template <class T>
//     AtomSelection_t<T> operator! (const AtomSelection_t<T>& seltoinverse);






}

#endif // ATOMSELECTION_H
