#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>
#include <istream>

#include "atom.h"
#include "stdio.h"
#include "pdbio.h"

using namespace std;

namespace PTools{


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



/** 
 * parses a PDB-formatted string and updates the given Atom instance
 * 
 */
void readAtom(const std::string& line, Atom& at)
{
    //TODO: needs factorization with ReadPDB
    
    if (isAtom(line))
        {
            std::string sx,sy,sz;

            sx=line.substr(30,8);
            sy=line.substr(38,8);
            sz=line.substr(46,8);


            at.coords.x=atof(sx.c_str());
            at.coords.y=atof(sy.c_str());
            at.coords.z=atof(sz.c_str());

//             at.atomType = readatomtype(line);
            at.atomType = line.substr(12,4);
            std::transform(at.atomType.begin(),at.atomType.end(),
                   at.atomType.begin(), //destination
                   (int(*)(int)) toupper //to upper: convert to upper case
                  );


            at.residType = readresidtype(line);
            std::string chainID = line.substr(21,1);
            if (chainID == " ") chainID = "";
            at.chainId = chainID;
            at.residId = atoi(line.substr(22,4).c_str());
            at.atomId = atoi(line.substr(6,5).c_str());
            std::string extra = line.substr(54,line.size()-1-54+1); //extracts everything after the position 27 to the end of line
            at.extra = extra ;
            
            return;

        }
        
    throw std::runtime_error("in pdbio.readAtom(): input is not an atom\n");
    
}



void ReadPDB(istream& file, Rigidbody& protein) {

    std::string line ;
    int line_count = 0 , atom_count=0;

    while ( std::getline(file, line))
    {
        line_count++ ;
        if (isAtom(line))
        {

            atom_count++ ;

            std::string sx,sy,sz;

            sx=line.substr(30,8);
            sy=line.substr(38,8);
            sz=line.substr(46,8);

            Coord3D pos;
            pos.x=atof(sx.c_str());
            pos.y=atof(sy.c_str());
            pos.z=atof(sz.c_str());

            Atomproperty a;
//             a.atomType = readatomtype(line);
            a.atomType = line.substr(12,4);
            std::transform(a.atomType.begin(),a.atomType.end(),
                   a.atomType.begin(), //destination
                   (int(*)(int)) toupper //to upper: convert to upper case
                  );
            
            
            a.residType = readresidtype(line);
            std::string chainID = line.substr(21,1);
            if (chainID == " ") chainID = "";
            a.chainId = chainID;
            a.residId = atoi(line.substr(22,4).c_str());
            a.atomId = atoi(line.substr(6,5).c_str());
            std::string extra = line.substr(54,line.size()-1-54+1); //extracts everything after the position 27 to the end of line
            a.extra = extra ;

            protein.AddAtom(a,pos);

        }
    }
}



void ReadPDB(const std::string name,Rigidbody& protein ) {
    std::string nomfich=name ;
	// pointer toward the filename given in the constructor argument
    ifstream file(nomfich.c_str()); 
    if (!file)
    {
        std::ostringstream oss;
        throw std::invalid_argument("##### ReadPDB:Could not open file \"" + nomfich + "\" #####") ;
    }

    ReadPDB(file, protein);
    file.close();
    return;

}

void WritePDB(const Rigidbody& rigid, std::string filename)
{

    FILE* file= fopen(filename.c_str(),"w") ;

    for (uint i=0; i<rigid.Size();i++)
    {

        const char * chainID="A" ;

        Atom at = rigid.CopyAtom(i);
        const char* atomname=at.atomType.c_str();
        const char* residName=at.residType.c_str();
        int residnumber = at.residId;
        chainID = at.chainId.c_str();

        int atomnumber = at.atomId;

        Coord3D coord = at.coords;
        dbl x = coord.x;
        dbl y = coord.y;
        dbl z = coord.z ;



//      fprintf(file, "ATOM  %5d %-4s %3s %1s%4d    %8.3f%8.3f%8.3f%s",atomnumber,atomname,residName,chainID,residnumber,real(x),real(y),real(z),at.extra.c_str());
//      fprintf(file, "ATOM  %5d  %-4s%3s %1s%4d    %8.3f%8.3f%8.3f%s", atomnumber,atomname,residName,chainID,residnumber,real(x),real(y),real(z),at.extra.c_str());
//      fprintf(file, "ATOM  %5d %-4s %3s %1s%4d    %8.3f%8.3f%8.3f%s", atomnumber,atomname,residName,chainID,residnumber,real(x),real(y),real(z),at.extra.c_str());
        fprintf(file, "ATOM  %5d %-4s %3s %1s%4d    %8.3f%8.3f%8.3f%s", atomnumber,atomname,residName,chainID,residnumber,real(x),real(y),real(z),at.extra.c_str());
        
        
//         fprintf(file, "ATOM  %5d %4s%1s%3s %1s%4d%1s   %8.3f%8.3f%8.3f%6.2f%6.2f          %2s%2s")
        fprintf(file,"\n");
    }

    fclose(file);
}

} //namespace PTools



