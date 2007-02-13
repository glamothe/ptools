#include "geometry.h"


namespace PTools{

void mat44xVect( double mat[ 4 ][ 4 ], const Coord3D& vect, Coord3D& out )
{
    out.x = vect.x * mat[ 0 ][ 0 ] + vect.y * mat[ 0 ][ 1 ] + vect.z * mat[ 0 ][ 2 ] + mat[ 0 ][ 3 ] ;
    out.y = vect.x * mat[ 1 ][ 0 ] + vect.y * mat[ 1 ][ 1 ] + vect.z * mat[ 1 ][ 2 ] + mat[ 1 ][ 3 ] ;
    out.z = vect.x * mat[ 2 ][ 0 ] + vect.y * mat[ 2 ][ 1 ] + vect.z * mat[ 2 ][ 2 ] + mat[ 2 ][ 3 ] ;
}


void mat44xmat44( const double mat1[ 4 ][ 4 ], const double mat2[ 4 ][ 4 ], double result[ 4 ][ 4 ] )
{
    //printmat44(mat1);
    //printmat44(mat2);


    for ( int rl = 0; rl < 4; rl++ )
        for ( int rc = 0; rc < 4; rc++ )
        {
            // calcul element result[rl][rc]:
            double sum = 0 ;
            for ( int p = 0; p < 4; p++ )
                sum += mat1[ rl ][ p ] * mat2[ p ][ rc ] ;
            result[ rl ][ rc ] = sum ;

        }

    //printmat44(result);
}



void MakeRotationMatrix( Coord3D A, Coord3D B, double theta, double out[ 4 ][ 4 ] )
{

    //calcul vecteur AB (dx;dy;dz):
    double dx = B.x - A.x ;
    double dy = B.y - A.y ;
    double dz = B.z - A.z ;

    double mat1[ 4 ][ 4 ] ;


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
    double d = sqrt( pow( dy, 2 ) + pow( dz, 2 ) ) ; //projeté de AB sur le plan Oxy

    if ( d == 0 )  // AB appartient à (Ox)
    {
        double cost = cos( theta );
        double sint = sin( theta );

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


    double cosg = dz / d ;
    double sing = dy / d ;
    double mat2[ 4 ][ 4 ] ;

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

    double mat3[ 4 ][ 4 ];
    mat44xmat44( mat2, mat1, mat3 ); // mat3 == mat2*mat1 (!= mat1*mat2 )


    // rotation pour ramener sur l'axe Oz: Rotation 2. Axe (Oy), angle p.
    double f = sqrt( pow( dx, 2 ) + pow( dy, 2 ) + pow( dz, 2 ) ); //norme
    double cosp, sinp ;
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
    double rotmatrix[ 4 ][ 4 ] ;
    double cost = cos( theta );
    double sint = sin( theta );

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



void XRotation( const Rigidbody& source, Rigidbody& result, double alpha )
{

    double cosa = cos( alpha );
    double sina = sin( alpha );

    double rotmatrix[ 4 ][ 4 ];


    //rotation d'angle alpha autour de (Ox), dans le sens indirect en regardant vers l'origine.
    rotmatrix[ 0 ][ 0 ] = 1 ;
    rotmatrix[ 0 ][ 1 ] = 0;
    rotmatrix[ 0 ][ 2 ] = 0;
    rotmatrix[ 0 ][ 3 ] = 0 ;
    
    rotmatrix[ 1 ][ 0 ] = 0;
    rotmatrix[ 1 ][ 1 ] = cosa ;
    rotmatrix[ 1 ][ 2 ] = sina;
    rotmatrix[ 1 ][ 3 ] = 0;
    
    rotmatrix[ 2 ][ 0 ] = 0;
    rotmatrix[ 2 ][ 1 ] = -sina;
    rotmatrix[ 2 ][ 2 ] = cosa;
    rotmatrix[ 2 ][ 3 ] = 0;
    
    rotmatrix[ 3 ][ 0 ] = 0;
    rotmatrix[ 3 ][ 1 ] = 0;
    rotmatrix[ 3 ][ 2 ] = 0;
    rotmatrix[ 3 ][ 3 ] = 1;



    for ( uint iatom = 0; iatom < source.Size(); iatom++ )
    {
        Coord3D res;
        Coord3D vect = source.GetCoords(iatom);
   
        mat44xVect( rotmatrix, vect, res ) ;
        result.SetCoords(iatom, res) ;

    }



}



void mat44xrigid( const Rigidbody& source, Rigidbody& result, double mat[ 4 ][ 4 ] )
{

    for ( uint iatom = 0; iatom < source.Size(); iatom++ )
    {
        Coord3D res;
        Coord3D vect = source.GetCoords(iatom);
        mat44xVect( mat, vect, res ) ;
        
        result.SetCoords(iatom, res) ; 
    }

}




void ABrotate( Coord3D A, Coord3D B, const Rigidbody& source, Rigidbody& result, double theta )
{
    double matrix[ 4 ][ 4 ];
    MakeRotationMatrix( A, B, theta, matrix );
    mat44xrigid( source, result, matrix );
}


/**
Effectue une rotation d'Euler selon la convention ZYZ
Attention: ne marche que si l'objet source est déjà centré

TODO: tests !

*/
void EulerZYZ(const Rigidbody & source, Rigidbody & cible, double theta, double phi, double psi)
{

double ct = cos(theta);
double st = sin(theta);
double cp = cos(phi);
double sp = sin(phi);
double cs = cos(psi);
double ss = sin(psi);

for (uint at=0; at<source.Size(); at++)
{
Coord3D vec = source.GetCoords(at);
double x = vec.x;
double y = vec.y;
double z = vec.z;


Coord3D target;
target.x = ct*( sp*z + cp*(cs*x-ss*y)) -st*(cs*y+ss*x) ;
target.x = st*( sp*z + cp*(cs*x-ss*y)) +ct*(cs*y+ss*x) ;
target.z = cp*z-sp*(cs*x-ss*y) ;

cible.SetCoords(at,target) ;


}


}















void ProdVect( const Coord3D& u, const Coord3D& v, Coord3D& UvectV )
{
    UvectV.x = u.y * v.z - u.z * v.y ;
    UvectV.y = u.z * v.x - u.x * v.z ;
    UvectV.z = u.x * v.y - u.y * v.x ;
}

void printmat44( const double mat[ 4 ][ 4 ] )
{
    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
        {
            std::cerr << mat[ i ][ j ] << "   " ;
        }
        std::cerr << "\n" ;

    }
    std::cerr << "\n\n";
}


void MakeVect( const Coord3D& a, const Coord3D& b, Coord3D& result )
{
    result.x = b.x - a.x;
    result.y = b.y - a.y;
    result.z = b.z - a.z;
}




double dihedral( const Coord3D& a, const Coord3D& b, const Coord3D& c, const Coord3D& d )
{
    //calculate the dihedral angle defined by a, b, c and d.
    //The method is described in: J.K Rainey, Ph.D. Thesis,
    // University of Toronto, 2003. Collagen structure and preferential assembly explored by parallel microscopy and
    //bioinformatics.


    //Here we go !

    //vecteur ba :
    Coord3D BA;
    MakeVect( b, a, BA );


    Coord3D CD;
    MakeVect( c, d, CD );
    Coord3D BC;
    MakeVect( b, c, BC );
    Coord3D n1, n2 ;

    ProdVect( BC, BA, n1 ); // n1=
    ProdVect( BC, CD, n2 );

    //std::cout <<"prodscal: " <<prodscal(BA,n1)<< "\n" ;

    double n1n2 = prodscal( n1, n2 );
    double cost = n1n2 / sqrt( prodscal( n1, n1 ) * prodscal( n2, n2 ) );

    double theta = acos( cost );
    std::cout << "Theta: " << theta / 3.14159265*180 << "\n" ;

    Coord3D n3;
    ProdVect( n2, n1, n3 );
    
    throw "incomplete implemenation of dihedral";
    
    return 0.0;

}



double Angle(const Coord3D& vector1, const Coord3D& vector2)
{
double pdtscal=prodscal(vector1,vector2);
double A = sqrt(prodscal(vector1,vector1)) ; 
double B = sqrt(prodscal(vector2,vector2));

double costheta = pdtscal / (A*B) ; 
return acos(costheta);
}



void Translate(const Rigidbody& source, Rigidbody& target, const Coord3D& trans)
{
      assert(source.Size()==target.Size());
      for (uint i=0; i < source.Size(); i ++)
      {
            Coord3D translated = target.GetCoords(i)+trans ; 
            target.SetCoords(i, translated);

      }

}


}

