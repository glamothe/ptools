//
// C++ Implementation: atomselection
//
// Description:
//
//

#include "atomselection.h"

namespace PTools {

AtomSelection::AtomSelection(Rigidbody& rigid)
{
    m_rigid=&rigid;
    for (uint i=0; i < rigid.Size(); i++)
    {
        this->AddAtomIndex(i);
    }
}


AtomSelection::~AtomSelection()
{
}

AtomSelection::AtomSelection(const AtomSelection& oldsel)
{
    this->m_rigid = oldsel.m_rigid;
}


// AtomSelection AtomSelection::operator = (const AtomSelection& atsel)
// {
//     if (this != &atsel)
//     {
//         this->m_rigid = atsel.m_rigid;
//         this->m_list  = atsel.m_list;
// 
//     }
// 
// }



AtomSelection SelectAllAtoms( Rigidbody& rigid)
{
    AtomSelection select(rigid);
    for (uint i=0; i < rigid.Size(); i++)
    {
        select.AddAtomIndex(i);
    }


    return select;

}




AtomSelection SelectAtomType(AtomSelection& select, std::string type)
{
    AtomSelection newsel(select);

    for (uint i=0; i<select.Size(); i++)
    {
        if (select[i].GetType()==type)
            newsel.AddAtomIndex(i);
    }

}





}
