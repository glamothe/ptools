#include "pdbparser.h"


#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>


using namespace std;


namespace PTools{

std::string strip( std::string const& str, char const* sepSet )
{
      std::string::size_type const first = str.find_first_not_of(sepSet);
      return ( first==std::string::npos )
                  ? std::string()
      : str.substr(first, str.find_last_not_of(sepSet)-first+1);
} 


bool isAtom(const std::string & ligne )
{
    //cout << ligne.size()<< endl ;
    if (ligne.size()<10 ) return false;

    if (ligne.substr(0,6)==(std::string) "ATOM  " ) return true ;
    return false;


}




std::string readatomtype(const std::string &ligne)
{
    std::string type="" ;
    int i=12 ;
    while (ligne[i]==' ')
    {
        i++;
        if (i>15) return type ;
    }

    int j=i ;
    while (ligne[j]!=' ') j++ ;

    type=ligne.substr(i,j-i) ;
    std::transform(type.begin(),type.end(),
                   type.begin(), //destination
                   (int(*)(int)) toupper //to upper: met en majuscules
                  );

    //cout << type <<"a" ;



    return type;
}


std::string readresidtype(const std::string &ligne)
{
    std::string type="" ;
    int i=17 ;
    while (ligne[i]==' ')
    {
        i++;
        if (i>19) return type ;
    }

    int j=i ;
    while (ligne[j]!=' ') j++ ;

    type=ligne.substr(i,j-i) ;
    std::transform(type.begin(),type.end(),
                   type.begin(), //destination
                   (int(*)(int)) toupper //to upper: met en majuscules
                  );

    //cout << type <<"a" ;



    return type;
}






void readPDB(ifstream& fichier, Rigidbody& protein)

{

    std::string ligne ;
    int compteur = 0 , compteur1=0;
    //std::cout << "ouverture fichier  " << nomfich.c_str() << std::endl ;


    while ( std::getline(fichier, ligne))
    {
        compteur++ ;
        if (isAtom(ligne))
        {
            // cout <<ligne <<endl;
            compteur1++ ;

            std::string sx,sy,sz;

            sx=ligne.substr(30,8);
            sy=ligne.substr(38,8);
            sz=ligne.substr(46,8);

            coord3D pos;
            pos.x=atof(sx.c_str());
            pos.y=atof(sy.c_str());
            pos.z=atof(sz.c_str());

            Atom a;
            a.SetType( readatomtype(ligne));
            a.SetResidType(readresidtype(ligne));
            a.SetChainId(ligne.substr(21,1));
            a.SetResidId(atoi(ligne.substr(22,4).c_str()));
            a.SetAtomNumber(atoi(ligne.substr(7,4).c_str()));
            a.SetCoords(pos);


//             if (proteine.GetType()==AttractType)
//             {
//             a.setCategory( atoi( ligne.substr(57,2).c_str() ) );
//             a.setCharge( atof(ligne.substr(61,5).c_str()) );
//             }
 

            protein.AddAtom(a);

        }


    }

}









void readPDB(const std::string name,Rigidbody& protein )
{
    std::string nomfich=name ;
    ifstream fichier(nomfich.c_str()); //pointeur vers nom fichier donne en argument constructeur
    if (!fichier)
    {
        std::cout << "##### ERROR:Could not open file \"" << nomfich << "\" #####"<< std::endl ;
        exit(-1);
    }

    readPDB(fichier, protein);
    fichier.close();
    return;

}





}