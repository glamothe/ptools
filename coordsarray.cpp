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



#include "coordsarray.h"
#include "geometry.h"

namespace PTools{



CoordsArray::CoordsArray()
{
    for (uint i=0; i<4; i++)
        for (uint j=0; j<4; j++)
        {
            mat44[i][j]=0;
            if (i==j) mat44[i][j]=1;
        }

}


CoordsArray::CoordsArray(const CoordsArray & ca) //copy constructor
{
    _refcoords   = ca._refcoords;
    _movedcoords = ca._movedcoords;

    _modified();

//copy of the matrix:
//Note: very naive way. May be slower than memcpy()
    for (uint i=0; i<4; i++)
        for (uint j=0; j<4; j++)
            this->mat44[i][j]=ca.mat44[i][j];

}


void CoordsArray::Translate(const Coord3D& tr)
{
    //updates rotation/translation matrix:
    this->mat44[0][3]+=tr.x;
    this->mat44[1][3]+=tr.y;
    this->mat44[2][3]+=tr.z;

    _modified();
}



void CoordsArray::MatrixMultiply(const dbl mat[4][4])
{
    mat44xmat44( mat, mat44, mat44 ); //multiply our internal matrix4x4 by mat
    _modified();  // invalidates coordinates
}

void CoordsArray::ResetMatrix()
{

    _modified();

    for (uint i=0; i<4; i++)
        for (uint j=0; j<4; j++)
        {
            if (i!=j) mat44[i][j]=0;
            else mat44[i][j]=1;
        }

}



std::string CoordsArray::PrintMatrix() const
{
    std::string out;
    out += "### MAT BEGIN\n";
    for (uint i=0; i<4; i++)
    {

        out += "MAT   ";
        for (uint j=0; j<4; j++)
        {
            char tmp[14]; tmp[14]=0;

            sprintf(tmp, "%13.7f", real(this->mat44[i][j])) ;
            out += tmp;
        }
        out +="\n";
    }
    out +=  "### MAT END\n";
    return out;
}



void CoordsArray::GetCoords(const uint i, Coord3D& co)  const
{

    if (i>=Size())
    {
        std::cerr <<"i: " <<  i << " size: " << Size()  << std::endl;

    }
    (*this.*_getcoords)(i,co);
};


}//namespace PTools


