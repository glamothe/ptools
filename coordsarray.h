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

#ifndef _COORDS_ARRAY_H_
#define _COORDS_ARRAY_H_

#include <vector>
#include<stdexcept>


#include "coord3d.h"

namespace PTools {


typedef std::vector<Coord3D> VCoords;

void mat44xmat44( const dbl mat1[4][4], const dbl mat2[4][4], dbl result[4][4] );

inline void matrix44xVect(const dbl mat[ 4 ][ 4 ], const Coord3D& vect, Coord3D& out )
{
    out.x = vect.x * mat[ 0 ][ 0 ] + vect.y * mat[ 0 ][ 1 ] + vect.z * mat[ 0 ][ 2 ] + mat[ 0 ][ 3 ] ;
    out.y = vect.x * mat[ 1 ][ 0 ] + vect.y * mat[ 1 ][ 1 ] + vect.z * mat[ 1 ][ 2 ] + mat[ 1 ][ 3 ] ;
    out.z = vect.x * mat[ 2 ][ 0 ] + vect.y * mat[ 2 ][ 1 ] + vect.z * mat[ 2 ][ 2 ] + mat[ 2 ][ 3 ] ;
}



// we define here two mode Policies: NoMode and NormalMode
// we have to decide at compile-time which policy should be used
struct NoMode
{
    static inline void applyMode(Coord3D & co, int atomindex) {}
};

void normalizeMode(VCoord3D & mode);

struct NormalMode
{
    inline void applyMode(Coord3D & co, int atomindex)
    {

        for (uint modenumber=0; modenumber < this-> m_lambdaMode.size();  ++modenumber )
        {
            co += m_modesArray[modenumber][atomindex] * m_lambdaMode[modenumber];
        }

    }

    ///adds a mode to the mode list
    void addMode(VCoord3D & mode, dbl eigen)
    {
        m_modesArray.push_back(mode);
        normalizeMode(m_modesArray[m_modesArray.size()-1]);
        m_lambdaMode.push_back(0.0);
        m_eigen.push_back(eigen);
    }

    ///ask for a certain mode to be applied before getting the coordinates
    void applyMode(uint modenumber, dbl lambda) {
        assert(m_lambdaMode.size()> modenumber );
        m_lambdaMode[modenumber] = lambda ;
    }




    std::vector<dbl> m_lambdaMode ;
    std::vector<dbl> m_eigen;
    std::vector<VCoord3D> m_modesArray; ///< array of modes (normal modes)

};




template <class modePolicy>
class CoordsArray: public modePolicy
{



private:  //private data

    /* don't forget the constructors if you add some private data ! */
    VCoords _refcoords;
    mutable VCoords _movedcoords;
    dbl mat44[4][4]; // 4x4 matrix

    mutable bool _uptodate ;

    mutable void (CoordsArray::*_getcoords)(const uint i, Coord3D& co) const; //C++ member function pointer. Points to a CoordArray function that takes a const uint and a Coord3D& and returns void.




private: //private methods



    void _modified() {
        _uptodate = false;
        _getcoords = & CoordsArray::_safegetcoords;
    }; // call this function when _movedcoords needs an update before getting real coordinates

    void _safegetcoords(const uint i, Coord3D& co) const {

        assert(_refcoords.size() == _movedcoords.size());

        for (uint j=0; j<_refcoords.size(); j++)
        {
            matrix44xVect(mat44, _refcoords[j], _movedcoords[j]);
        }

        _uptodate = true;
        //modify the function pointer _getcoords to call the "unsafe" method next time (faster)
        _getcoords = & CoordsArray::unsafeGetCoords;
        (*this.* _getcoords)(i, co); //return the correct function

    };



public:


    CoordsArray() //constructor
    {
        for (uint i=0; i<4; i++)
            for (uint j=0; j<4; j++)
            {
                mat44[i][j]=0;
                if (i==j) mat44[i][j]=1;
            }

    }


    CoordsArray(const CoordsArray & ca) //copy constructor
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





    /// get the cached coordinates. You must ensure that update() has been called first !
    void inline unsafeGetCoords(const uint i, Coord3D& co) const {
        co = _movedcoords[i];
    };

    void AddCoord(const Coord3D& co) {
        _refcoords.push_back(co);
        _movedcoords.push_back(co);
        _modified();
    };
    uint Size() const {
        return _refcoords.size();
    };


    void GetCoords(const uint i, Coord3D& co)  const throw(std::out_of_range)
    {

        if (i>=Size())
        {
            std::string message = (std::string) "CoordsArray::GetCoords : out of range :  ";
            message += i ;
            message += " is out of bounds (object size: ";
            message +=  Size() ;
            message += ")\n";
            std::cerr << message ;
            throw std::out_of_range (message);
        }

        (*this.*_getcoords)(i,co);
    };



    void SetCoords(const uint k, const Coord3D& co)
    {
//sets the coordinate [i] to be 'co' after rotation/translation

        Coord3D co2 (co);

        co2.x -= mat44[0][3];
        co2.y -= mat44[1][3];
        co2.z -= mat44[2][3];

        dbl matinv[4][4]; //invert matrix


        for (uint i=0; i<3; i++)
            for (uint j=0; j<3;j++)
                matinv[i][j]=mat44[j][i];

        for (uint i=0; i<4; i++)
        {
            matinv[i][3]=0;
            matinv[3][i]=0;
        }
        matinv[3][3]=1;


        Coord3D final;


        PTools::matrix44xVect(matinv,co2, final );

        _refcoords[k] = final;
        _modified();


    }


    /// Translate the whole object
    void Translate(const Coord3D& tr)
    {
        //updates rotation/translation matrix:
        this->mat44[0][3]+=tr.x;
        this->mat44[1][3]+=tr.y;
        this->mat44[2][3]+=tr.z;

        _modified();
    }




    /*!  \brief this function makes an euler rotation with the Attract convention.
    *
    *   Note that for this new implementation only the 4x4 rotational/translational
    *   matrix is updated. This may allow a big speedup (to be tested) and a
    *   higher flexibility  (  rig.Translate(a); rig.Translate(minus(a)); may now be delayed
    *   until the coordinates are really needed.
    *   If coordinates are never asked (why?), then no costly calculation is performed !
    */
    void AttractEulerRotate(dbl phi, dbl ssi, dbl rot)
    {

        dbl  cp, cs, ss, sp, cscp, sscp, sssp, crot, srot, cssp ;

        cs=cos(ssi);
        cp=cos(phi);
        ss=sin(ssi);
        sp=sin(phi);
        cscp=cs*cp;
        cssp=cs*sp;
        sscp=ss*cp;
        sssp=ss*sp;
        crot=cos(rot);
        srot=sin(rot);


        dbl eulermat[4][4];

        eulermat[0][0] = crot*cscp + srot*sp;
        eulermat[0][1] = srot*cscp - crot*sp;
        eulermat[0][2] = sscp;
        eulermat[0][3] = 0.0;

        eulermat[1][0] = crot*cssp-srot*cp ;
        eulermat[1][1] = srot*cssp+crot*cp;
        eulermat[1][2] = sssp;
        eulermat[1][3] = 0.0;

        eulermat[2][0] = -crot*ss;
        eulermat[2][1] = -srot*ss;
        eulermat[2][2] = cs;
        eulermat[2][3] = 0.0;

        eulermat[3][0] = 0.0;
        eulermat[3][1] = 0.0;
        eulermat[3][2] = 0.0;
        eulermat[3][3] = 1.0;

        //matrix multiplication
        this->MatrixMultiply(eulermat);
//       mat44xmat44( eulermat , this->mat44, this->mat44);

    }







    void ResetMatrix()
    {

        _modified();

        for (uint i=0; i<4; i++)
            for (uint j=0; j<4; j++)
            {
                if (i!=j) mat44[i][j]=0;
                else mat44[i][j]=1;
            }

    }


    std::string PrintMatrix() const
    {
        std::string out;
        out += "### MAT BEGIN\n";
        for (uint i=0; i<4; i++)
        {

            out += "MAT   ";
            for (uint j=0; j<4; j++)
            {
                char tmp[15];
                tmp[14]=0;

                sprintf(tmp, "%14.7f ", real(this->mat44[i][j])) ;
                out += tmp;
            }
            out +="\n";
        }
        out +=  "### MAT END\n";
        return out;
    }


    ///return the rotation/translation matrix
    Matrix GetMatrix() const
    {
        Matrix matrix(4,4);

        for (uint i=0; i<4; i++)
            for (uint j=0; j<4; j++)
                matrix(i,j)=mat44[i][j];

        return matrix;
    }




    void MatrixMultiply(const dbl mat[4][4])
    {
        mat44xmat44( mat, mat44, mat44 ); //multiply our internal matrix4x4 by mat
        _modified();  // invalidates coordinates
    }


// protected:




};



typedef CoordsArray<NoMode> NoModeCoordsArray;




} //namespace PTools




#endif // _COORDS_ARRAY_H_







