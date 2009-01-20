// $Id$

#include "pdbio.h"

using namespace std;

namespace PTools{


// Pdb::Pdb(const std::string& pdbname)
// {
// 
// 
// 
// }



std::string strip( std::string const& str, char const* sepSet ) {
    std::string::size_type const first = str.find_first_not_of(sepSet);
    return ( first==std::string::npos )
           ? std::string()
           : str.substr(first, str.find_last_not_of(sepSet)-first+1);
}


bool isAtom(const std::string & line ) {
    //cout << ligne.size()<< endl ;
    if (line.size()<10 ) return false;

    if (line.substr(0,6)==(std::string) "ATOM  " ) return true ;
    return false;
}


bool isHeteroAtom(const std::string& line) {
    if (line.size()<10) return false;

    if (line.substr(0,6) == "HETATM") return true;
    return false;
}




std::string readatomtype(const std::string &ligne) {
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
                   (int(*)(int)) toupper //to upper: convert to upper case
                  );

    //cout << type <<"a" ;



    return type;
}


std::string readresidtype(const std::string &ligne) {
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
                   (int(*)(int)) toupper //to upper: convert to upper case
                  );

    //cout << type <<"a" ;



    return type;
}



} //namespace PTools



