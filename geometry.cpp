#include "geometry.h"


namespace PTools{



/** \brief Matrix multiplication
*  This function is a simple matrix multiplication routine for 4x4 matrix
*/
void mat44xmat44( const dbl mat1[ 4 ][ 4 ], const dbl mat2[ 4 ][ 4 ], dbl result[ 4 ][ 4 ] )
{
// gives mat1*mat2 (mat2 left multiplied by mat1)
// this works even if result == mat1 (ie pointing the to same memory)

    dbl temp[4][4];

    //std::cout << mat1 << " " << mat2 << " " << result;
    //printmat44(mat1);
    //printmat44(mat2);

    for ( int rl = 0; rl < 4; rl++ )
        for ( int rc = 0; rc < 4; rc++ )
        {
            // calcul element result[rl][rc]:
            dbl sum = 0.0 ;
            for ( int p = 0; p < 4; p++ )
                sum += mat1[ rl ][ p ] * mat2[ p ][ rc ] ;
            temp[ rl ][ rc ] = sum ;
        }

    //printmat44(result);
    memcpy(result, temp, 16*sizeof(dbl));

}



void MakeRotationMatrix( Coord3D A, Coord3D B, dbl theta, dbl out[ 4 ][ 4 ] )
{

    //calcul vecteur AB (dx;dy;dz):
    dbl dx = B.x - A.x ;
    dbl dy = B.y - A.y ;
    dbl dz = B.z - A.z ;

    dbl mat1[ 4 ][ 4 ] ;


    //translation vecteur BA:
    for ( int i = 0; i < 4; i++ )
        for ( int j = 0; j < 4; j++ )
            if ( i != j )
            {
                mat1[ i ][ j ] = 0 ;
            }
            else
                mat1[ i ][ j ] = 1 ;

    mat1[ 0 ][ 3 ] = -A.x;
    mat1[ 1 ][ 3 ] = -A.y;
    mat1[ 2 ][ 3 ] = -A.z;

    //rotation pour ramener sur le plan Oxz: Rotation 1 autour de X, angle -gamma (-g).
    dbl d = sqrt( dy*dy + dz*dz ) ; //projeté de AB sur le plan Oxy

    if ( real(d) == 0 )  // AB appartient à (Ox)
    {
        dbl cost = cos( theta );
        dbl sint = sin( theta );

        out[ 0 ][ 0 ] = 1 ;
        out[ 0 ][ 1 ] = 0 ;
        out[ 0 ][ 2 ] = 0 ;
        out[ 0 ][ 3 ] = 0 ;
        out[ 1 ][ 0 ] = 0 ;
        out[ 1 ][ 1 ] = cost ;
        out[ 1 ][ 2 ] = sint ;
        out[ 1 ][ 3 ] = 0 ;
        out[ 2 ][ 0 ] = 0 ;
        out[ 2 ][ 1 ] = -sint;
        out[ 2 ][ 2 ] = cost ;
        out[ 2 ][ 3 ] = 0 ;
        out[ 3 ][ 0 ] = 0 ;
        out[ 3 ][ 1 ] = 0 ;
        out[ 3 ][ 2 ] = 0 ;
        out[ 3 ][ 3 ] = 1 ;
        //printmat44(out);
        return ;

    }


    dbl cosg = dz / d ;
    dbl sing = dy / d ;
    dbl mat2[ 4 ][ 4 ] ;

    mat2[ 0 ][ 0 ] = 1 ;
    mat2[ 0 ][ 1 ] = 0 ;
    mat2[ 0 ][ 2 ] = 0 ;
    mat2[ 0 ][ 3 ] = 0 ;
    mat2[ 1 ][ 0 ] = 0 ;
    mat2[ 1 ][ 1 ] = cosg ;
    mat2[ 1 ][ 2 ] = -sing ;
    mat2[ 1 ][ 3 ] = 0 ;
    mat2[ 2 ][ 0 ] = 0;
    mat2[ 2 ][ 1 ] = sing;
    mat2[ 2 ][ 2 ] = cosg ;
    mat2[ 2 ][ 3 ] = 0 ;
    mat2[ 3 ][ 0 ] = 0 ;
    mat2[ 3 ][ 1 ] = 0 ;
    mat2[ 3 ][ 2 ] = 0;
    mat2[ 3 ][ 3 ] = 1 ;

    //printmat44(mat2);

    dbl mat3[ 4 ][ 4 ];
    mat44xmat44( mat2, mat1, mat3 ); // mat3 == mat2*mat1 (!= mat1*mat2 )


    // rotation pour ramener sur l'axe Oz: Rotation 2. Axe (Oy), angle p.
    dbl f = sqrt(  dx*dx + dy*dy + dz*dz ); //norme
    dbl cosp, sinp ;
    cosp = d / f;
    sinp = dx / f;




    mat1[ 0 ][ 0 ] = cosp ;
    mat1[ 0 ][ 1 ] = 0 ;
    mat1[ 0 ][ 2 ] = -sinp ;
    mat1[ 0 ][ 3 ] = 0 ;
    mat1[ 1 ][ 0 ] = 0 ;
    mat1[ 1 ][ 1 ] = 1 ;
    mat1[ 1 ][ 2 ] = 0 ;
    mat1[ 1 ][ 3 ] = 0 ;
    mat1[ 2 ][ 0 ] = sinp ;
    mat1[ 2 ][ 1 ] = 0 ;
    mat1[ 2 ][ 2 ] = cosp ;
    mat1[ 2 ][ 3 ] = 0 ;
    mat1[ 3 ][ 0 ] = 0 ;
    mat1[ 3 ][ 1 ] = 0 ;
    mat1[ 3 ][ 2 ] = 0 ;
    mat1[ 3 ][ 3 ] = 1 ;

    mat44xmat44( mat1, mat3, mat2 ); //résultat dans mat2


    //rotation effective (autour axe 0z, angle theta)
    dbl rotmatrix[ 4 ][ 4 ] ;
    dbl cost = cos( theta );
    dbl sint = sin( theta );

    rotmatrix[ 0 ][ 0 ] = cost ;
    rotmatrix[ 0 ][ 1 ] = sint;
    rotmatrix[ 0 ][ 2 ] = 0;
    rotmatrix[ 0 ][ 3 ] = 0;
    rotmatrix[ 1 ][ 0 ] = -sint;
    rotmatrix[ 1 ][ 1 ] = cost;
    rotmatrix[ 1 ][ 2 ] = 0;
    rotmatrix[ 1 ][ 3 ] = 0;
    rotmatrix[ 2 ][ 0 ] = 0 ;
    rotmatrix[ 2 ][ 1 ] = 0 ;
    rotmatrix[ 2 ][ 2 ] = 1;
    rotmatrix[ 2 ][ 3 ] = 0;
    rotmatrix[ 3 ][ 0 ] = 0 ;
    rotmatrix[ 3 ][ 1 ] = 0 ;
    rotmatrix[ 3 ][ 2 ] = 0;
    rotmatrix[ 3 ][ 3 ] = 1;

    mat44xmat44( rotmatrix, mat2, mat3 ); //résultat dans mat3


    //rotation -2:
    mat1[ 0 ][ 0 ] = cosp ;
    mat1[ 0 ][ 1 ] = 0 ;
    mat1[ 0 ][ 2 ] = sinp ;
    mat1[ 0 ][ 3 ] = 0 ;
    mat1[ 1 ][ 0 ] = 0 ;
    mat1[ 1 ][ 1 ] = 1 ;
    mat1[ 1 ][ 2 ] = 0 ;
    mat1[ 1 ][ 3 ] = 0 ;
    mat1[ 2 ][ 0 ] = -sinp ;
    mat1[ 2 ][ 1 ] = 0 ;
    mat1[ 2 ][ 2 ] = cosp ;
    mat1[ 2 ][ 3 ] = 0 ;
    mat1[ 3 ][ 0 ] = 0 ;
    mat1[ 3 ][ 1 ] = 0 ;
    mat1[ 3 ][ 2 ] = 0 ;
    mat1[ 3 ][ 3 ] = 1 ;




    mat44xmat44( mat1, mat3, rotmatrix ); //résultat dans rotmatrix;

    //rotation -1:

    mat2[ 0 ][ 0 ] = 1 ;
    mat2[ 0 ][ 1 ] = 0 ;
    mat2[ 0 ][ 2 ] = 0 ;
    mat2[ 0 ][ 3 ] = 0 ;
    mat2[ 1 ][ 0 ] = 0 ;
    mat2[ 1 ][ 1 ] = cosg ;
    mat2[ 1 ][ 2 ] = sing ;
    mat2[ 1 ][ 3 ] = 0 ;
    mat2[ 2 ][ 0 ] = 0 ;
    mat2[ 2 ][ 1 ] = -sing ;
    mat2[ 2 ][ 2 ] = cosg ;
    mat2[ 2 ][ 3 ] = 0 ;
    mat2[ 3 ][ 0 ] = 0 ;
    mat2[ 3 ][ 1 ] = 0 ;
    mat2[ 3 ][ 2 ] = 0 ;
    mat2[ 3 ][ 3 ] = 1 ;



    mat44xmat44( mat2, rotmatrix, mat3 );


    //translation-1:
    for ( int i = 0; i < 4; i++ )
        for ( int j = 0; j < 4; j++ )
            if ( i != j )
            {
                mat1[ i ][ j ] = 0 ;
            }
            else
                mat1[ i ][ j ] = 1 ;

    mat1[ 0 ][ 3 ] = A.x;
    mat1[ 1 ][ 3 ] = A.y;
    mat1[ 2 ][ 3 ] = A.z;

    mat44xmat44( mat1, mat3, out );

};




void ABrotate( Coord3D A, Coord3D B, Rigidbody& target, dbl theta )
{

    dbl matrix[ 4 ][ 4 ];
    MakeRotationMatrix( A, B, theta, matrix );
    target.MatrixMultiply(matrix);
//     mat44xrigid( source, result, matrix );
}






void VectProd( const Coord3D& u, const Coord3D& v, Coord3D& UvectV )
{
    UvectV.x = u.y * v.z - u.z * v.y ;
    UvectV.y = u.z * v.x - u.x * v.z ;
    UvectV.z = u.x * v.y - u.y * v.x ;
}

void printmat44( const dbl mat[ 4 ][ 4 ] )
{

    for (uint i=0; i<4; i++)
    {
        for (uint j=0; j<4; j++)
        {
            printf("%12.7f", real(mat[i][j])) ;
        }
        std::cout << std::endl;
    }

    std::cout << "\n\n";
}


void MakeVect( const Coord3D& a, const Coord3D& b, Coord3D& result )
{
    result.x = b.x - a.x;
    result.y = b.y - a.y;
    result.z = b.z - a.z;
}




dbl dihedral( const Coord3D& a, const Coord3D& b, const Coord3D& c, const Coord3D& d )
{
    //calculate the dihedral angle defined by a, b, c and d.
    //The method is described in: J.K Rainey, Ph.D. Thesis,
    // University of Toronto, 2003. Collagen structure and preferential assembly explored by parallel microscopy and
    //bioinformatics.
    //and also described here: http://structbio.biochem.dal.ca/jrainey/dihedralcalc.html


    //Here we go !

    //vecteur ba :
    Coord3D BA;
    MakeVect( b, a, BA );


    Coord3D CD;
    MakeVect( c, d, CD );
    Coord3D BC;
    MakeVect( b, c, BC );
    Coord3D n1, n2 ;

    VectProd( BC, BA, n1 ); // n1=
    VectProd( BC, CD, n2 );

    //std::cout <<"ScalProd: " <<ScalProd(BA,n1)<< "\n" ;

    dbl n1n2 = ScalProd( n1, n2 );
    dbl cost = n1n2 / sqrt( ScalProd( n1, n1 ) * ScalProd( n2, n2 ) );

    dbl theta = acos( cost );
    std::cout << "Theta: " << theta / 3.14159265*180 << "\n" ;

    Coord3D n3;
    VectProd( n2, n1, n3 );

    throw "incomplete implemenation of dihedral";

    return 0.0;

}



dbl Angle(const Coord3D& vector1, const Coord3D& vector2)
{
    dbl pdtscal=ScalProd(vector1,vector2);
    dbl A = sqrt(ScalProd(vector1,vector1)) ;
    dbl B = sqrt(ScalProd(vector2,vector2));

    dbl costheta = pdtscal / (A*B) ;
    return acos(costheta);
}

} //namespace PTools

