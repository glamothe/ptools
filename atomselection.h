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


class AtomSelection{

private:
    //private data
    Rigidbody* m_rigid;
    std::vector<uint> m_list;

    //private methods



public:
    AtomSelection(){};
    AtomSelection(const AtomSelection& oldsel);
    AtomSelection(Rigidbody& rigid); ///< translate a Rigidbody object into an AtomSelection
    ~AtomSelection();

    uint Size() {return m_list.size();}; ///< returns the size of the selection
    void SetRigid(Rigidbody& rig) {m_rigid=&rig;};
    Atom& operator[] (uint i) {return m_rigid->GetAtom(m_list[i]);}; ///< return the i-th atom of the list
    void AddAtomIndex(uint i) {m_list.push_back(i);}; ///< adds an Atom index
    Rigidbody CreateRigid(); ///< makes a new rigidcopy (independant copy) from an AtomsSlection object.




    //friends:
    friend AtomSelection operator& (const AtomSelection& atsel1,const  AtomSelection& atsel2);
    friend AtomSelection operator| (const AtomSelection& atsel1,const AtomSelection& atsel2);
};


AtomSelection operator& (const AtomSelection& atsel1,const  AtomSelection& atsel2);
AtomSelection operator| (const AtomSelection& atsel1,const AtomSelection& atsel2);


}

#endif // ATOMSELECTION_H
