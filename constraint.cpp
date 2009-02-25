// $Id$
/****************************************************************************
 *   Copyright (C) 2009   Adrien Saladin                                    *
 *   adrien.saladin@gmail.com                                               *
 *   Copyright (C) 2009   Chantal Prevost                                   *
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


#include "constraint.h"

namespace PTools{

Constraint CreateDefaultConstraint(const AttractRigidbody& rec,const AttractRigidbody& lig, uint recnb, uint lignb)
{
    Constraint constr;
    constr.lig1 = recnb;
    constr.lig2 = lignb;
    constr.at1 = -1 ; //center of mass for ligand 1
    double mindist2 = 1e15;
    int pluspres = -1;
    Coord3D centrerec = rec.FindCenter();

    uint ligsize = lig.Size();

    for (uint i=0; i<ligsize; i++)
    {
        Coord3D co = lig.GetCoords(i);
        double distance =  Norm2(co-centrerec);
        if (distance  < mindist2)
           {
            pluspres = i;
            mindist2 = distance;
           }
    }
    constr.at2 = (int) pluspres;
    return constr;
}

}