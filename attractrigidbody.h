// $Id$
/****************************************************************************
 *   Copyright (C) 2008   Adrien Saladin                                    *
 *   adrien.saladin@gmail.com                                               *
 *   Copyright (C) 2008   Pierre Poulain                                    *
 *   Copyright (C) 2008   Sebastien Fiorucci                                *
 *   Copyright (C) 2008   Chantal Prevost                                   *
 *   Copyright (C) 2008   Martin Zacharias                                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
 *                                                                          *
 *                                          *
 ***************************************************************************/



#ifndef _ATTRACTRIGIDBODY_H_
#define _ATTRACTRIGIDBODY_H_

#include "rigidbody.h"

namespace PTools{


/*! \brief Rigidbody specific to Attract's reduced model
*
*   this objects stores "atom type number" property
*   which is required for Attract's forcefields and for
*   some attract's specific pairlist
*/
template <class T>
class AttractRigidbody: public Rigidbody<T>
{
public:


    /// initilize a new object from a regular Rigidbody object
    explicit AttractRigidbody<T>(const Rigidbody<T> & rig)
	: Rigidbody<T>(rig)
{
    // extracts the "extra" field of Atoms to the m_atomTypeNumber array:
    uint   atcategory  = 0;
    dbl  atcharge   = 0.0;

    for (uint i = 0; i < this->Size() ; ++i)
    {
        Atomproperty & at (this->mAtomProp[i]);
        std::string extra = at.GetExtra();

        std::istringstream iss( extra );
        iss >> atcategory >> atcharge ;
        m_atomTypeNumber.push_back(atcategory-1);  // -1 to directly use the atomTypeNumber into C-array
        m_charge.push_back(atcharge);

    }

    setRotation(true);
    setTranslation(true);

    resetForces();
}





    AttractRigidbody(){};
    virtual ~AttractRigidbody(){};

    uint getAtomTypeNumber(uint i) const
    {
        return m_atomTypeNumber[i];
    };
    dbl getCharge(uint i) const
    {
        return m_charge[i];
    };

    virtual bool isAtomActive(uint i) const {

       uint atomtype = this->m_atomTypeNumber[i];
       for(uint j=0; j<m_dummytypes.size(); j++)
         if(m_dummytypes[j]==atomtype)
               return false;
       return true; 
    };


    void resetForces()
    {
        m_forces = std::vector<Coord3D> (this->Size() ) ;
    }

    void addForces(const std::vector<Coord3D>& forces)
    {
        for (uint i=0; i<forces.size(); i++)
            m_forces[i]+=forces[i];
    }


    void setRotation(bool value) {hasrotation  = value;} ///< allow/disallow rotation
    void setTranslation(bool value) {hastranslation = value;} ///< allow/disallow translation


    /// set a list of ignored atom types
    void setDummyTypes(const std::vector<uint>& dummy)
{
    m_dummytypes = dummy;
    this->updateActiveList();
}


    bool operator==(const AttractRigidbody& at) {return false;}; //don't use it, needed to expose vector<AttractRigidobdy> to python by boost::vector indexing suite. 

    void updateActiveList()
{
 std::vector<uint> newactivelist;

 for(uint i=0; i<this->Size(); i++)
 {
   if( isAtomActive(i) ) newactivelist.push_back(i);
 }

std::swap(m_activeAtoms, newactivelist);

}



private:
    std::vector<uint> m_atomTypeNumber ;
    std::vector<dbl> m_charge ;
    std::vector<Coord3D> m_forces ;

    std::vector<uint> m_dummytypes; ///< list of ignored atom types
    std::vector<uint> m_activeAtoms; ///< list of active atoms (atoms that are taken into account for interaction)

    bool hastranslation;
    bool hasrotation;

    friend class BaseAttractForceField;
    friend class AttractForceField2;
    friend class AttractForceField1;
    friend class McopForceField;


};  //end class AttractRigid


} //end namespace PTools


#endif


