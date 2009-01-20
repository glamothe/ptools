// $Id$
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


#include<stdexcept>


#include "coordsarray.h"
#include "geometry.h"

namespace PTools {


void normalizeMode(VCoord3D & mode)
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



}//namespace PTools


