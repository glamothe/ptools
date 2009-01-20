// $Id$
#ifndef PDBIO_H
#define PDBIO_H

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <stdexcept>

#include <algorithm>



#include "atom.h"
#include "stdio.h"
#include "rigidbody.h"

namespace PTools
{

// class Model
// {
// 
//     enum
//     { main=0, hetatm, number_of_parts};
// 
//     Rigidbody _parts[number_of_parts]; //array with the correct size, with named elements
// 
// // Rigidbody _main;
// // Rigidbody _hetatm;
// 
// public:
// 
//     Rigidbody getMain(){return _parts[main];};
//     Rigidbody getHetatm(){return _parts[hetatm];};
// // Rigidbody getwater(){};
// 
// 
//     void Translate(const Coord3D& tr)
//     {
//         for (uint i=0; i<number_of_parts; i++)
//             _parts[i].Translate(tr);
//     }
// 
// /// rotation around (AB) axis.
//     void ABrotate(const Coord3D& A, const Coord3D& B, dbl theta)
//     {
//         for (uint i=0; i<number_of_parts; i++)
//             _parts[i].ABrotate(A, B, theta);
//     }
// 
// 
// /// Euler rotation (Attract convention)
//     void AttractEulerRotate(dbl phi, dbl ssi, dbl rot)
//     {
//         for (uint i=0; i<number_of_parts; i++)
//             _parts[i].AttractEulerRotate(phi, ssi, rot);
//     }
// 
// 
// 
// };
// 
// 
// class Pdb{
// 
//     std::vector<Model> _models;
// 
// public:
// 
//     Pdb(const std::string& pdbname);
//     uint nbmodels(){return _models.size();};
// 
//     Model operator[](int i) {if (i>=(int)0 && i<(int)_models.size()) return _models[i]; else throw std::range_error("model out of bounds");};
//
//
// };


std::string strip( std::string const& str, char const* sepSet );
bool isAtom(const std::string & line );
bool isHeteroAtom(const std::string& line);
std::string readatomtype(const std::string &ligne);
std::string readresidtype(const std::string &ligne);



/// read a PDB file from a file pointer and load datas in Rigidbody
template<class Rigid_type>
void ReadPDB(std::ifstream& fichier,Rigid_type & protein )
{

    std::string ligne ;
    int compteur = 0 , compteur1=0;
    //std::cout << "file opening  " << nomfich.c_str() << std::endl ;


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

            Coord3D pos;
            pos.x=atof(sx.c_str());
            pos.y=atof(sy.c_str());
            pos.z=atof(sz.c_str());

            Atomproperty a;
            a.SetType( readatomtype(ligne));
            a.SetResidType(readresidtype(ligne));
            a.SetChainId(ligne.substr(21,1));
            a.SetResidId(atoi(ligne.substr(22,4).c_str()));
            a.SetAtomId(atoi(ligne.substr(6,5).c_str()));
            std::string extra = ligne.substr(54,ligne.size()-1-54+1); //extracts everything after the position 27 to the end of line
            a.SetExtra(extra);

            protein.AddAtom(a,pos);

        }


    }

}







/// read a PDB file from a filename and load datas in Rigidbody
template<class Rigid_type>
void ReadPDB(const std::string name,Rigid_type & protein )
{
    std::string nomfich=name ;
	// pointer toward the filename given in the constructor argument
    std::ifstream fichier(nomfich.c_str()); 
    if (!fichier)
    {
        std::ostringstream oss;
        throw std::invalid_argument("##### ReadPDB:Could not open file \"" + nomfich + "\" #####") ;
//        exit(-1);
    }

    ReadPDB(fichier, protein);
    fichier.close();
    return;

}







/// write a PDB file given a Rigidbody and a filename
template<class Rigid_type>
void WritePDB(const Rigid_type& rigid, std::string filename)
{

    FILE* file= fopen(filename.c_str(),"w") ;

    for (uint i=0; i<rigid.Size();i++)
    {

        const char * chainID="A" ;

        Atom at = rigid.CopyAtom(i);
        const char* atomname=at.GetType().c_str();
        const char* residName=at.GetResidType().c_str();
        int residnumber = at.GetResidId();
        chainID=at.GetChainId().c_str();

        int atomnumber = at.GetAtomId();

        Coord3D coord = at.GetCoords();
        dbl x = coord.x;
        dbl y = coord.y;
        dbl z = coord.z ;



        fprintf(file,"ATOM  %5d  %-4s%3s %1s%4d    %8.3f%8.3f%8.3f%s",atomnumber,atomname,residName,chainID,residnumber,real(x),real(y),real(z),at.GetExtra().c_str());
        fprintf(file,"\n");
    }

    fclose(file);
}








}

#endif //#ifndef PDBIO_H


