/****************************************************************************
 *   Copyright (C) 2008   Adrien Saladin                                    *
 *   adrien.saladin@gmail.com                                               *
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
 ***************************************************************************/

#include <iostream>
#include <sstream>

#include "attractrigidbody.h"

namespace PTools{

///////////// AttractRigidbody implementation:

AttractRigidbody::AttractRigidbody(const Rigidbody & rig)
        : Rigidbody(rig)
{
    // extracts the "extra" field of Atoms to the m_atomTypeNumber array:
    uint   atcategory  = 0;
    dbl  atcharge   = 0.0;

    for (uint i = 0; i < Size() ; ++i)
    {
        Atomproperty & at (mAtomProp[i]);
        std::string extra = at.GetExtra();

        std::istringstream iss( extra );
        iss >> atcategory >> atcharge ;
        m_atomTypeNumber.push_back(atcategory-1);  // -1 to directly use the atomTypeNumber into C-array 
        m_charge.push_back(atcharge);

   if (isAtomActive(i))
   {
      m_activeAtoms.push_back(i);
   }

    }

    setRotation(true);
    setTranslation(true);

    resetForces();
}






/*!  \brief normalize a normal mode vector
*
*    this function normalizes a mode vector assuming it to be a n*3 linear vector.
*
*/
void AttractRigidbody::normalizeMode(VCoord3D & mode)
{

dbl sum = 0.0 ;
for (uint i=0; i<mode.size(); i++)
{
sum+=Norm2(mode[i]); // we normalize a n*3 linear like vector, so Norm2 is the correct function to use.
}

dbl norm = sqrt(sum);

for (uint i=0; i<mode.size(); i++)
{
  mode[i]=mode[i]/norm;  //divide each component by the vector norm
}

}




void AttractRigidbody::addMode(VCoord3D & mode, dbl eigen) 
{
    m_modesArray.push_back(mode);
    normalizeMode(m_modesArray[m_modesArray.size()-1]);
    m_lambdaMode.push_back(0.0);
    m_eigen.push_back(eigen);
}


double AttractRigidbody::getEigen(uint i) {return m_eigen.at(i);};

/////////////////////////////


// ///apply a normal mode to an AttractRigidbody
// void applyMode(AttractRigidbody & src, AttractRigidbody& dest, const std::vector<Coord3D> & mode, dbl scalar){
// assert(src.Size() == dest.Size());
// assert(mode.size() == src.Size());
// for(uint i=0; i<src.Size(); i++)
// {
//     Coord3D co = src.GetCoords(i);
//     Coord3D displacment = mode[i];
//     co +=displacment*scalar;
//     dest.SetCoords(i, co);
// }
// 
// 
// }



} //namespace PTools


