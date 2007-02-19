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
    AtomSelection(Rigidbody& rigid);
    ~AtomSelection();

    uint Size() {return m_list.size();}; ///< returns the size of the selection
    void SetRigid(Rigidbody& rig) {m_rigid=&rig;};
    Atom& operator[] (uint i) {return m_rigid->GetAtom(m_list[i]);}; ///< return the i-th atom of the list
    void AddAtomIndex(uint i) {m_list.push_back(i);}; ///< adds an Atom index
    Rigidbody CreateRigid(); //TODO: imlementation...


};

AtomSelection SelectAllAtoms(Rigidbody& rigid);
AtomSelection SelectAtomType(AtomSelection& select, std::string type);


}

#endif // ATOMSELECTION_H
