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


#include "rigidbody.h"

namespace PTools
{

class Model
{

    enum
    { main=0, hetatm, number_of_parts};

    Rigidbody _parts[number_of_parts]; //array with the correct size, with named elements

// Rigidbody _main;
// Rigidbody _hetatm;

public:

    Rigidbody getMain(){return _parts[main];};
    Rigidbody getHetatm(){return _parts[hetatm];};
// Rigidbody getwater(){};


    void Translate(const Coord3D& tr)
    {
        for (uint i=0; i<number_of_parts; i++)
            _parts[i].Translate(tr);
    }

/// rotation around (AB) axis.
    void ABrotate(const Coord3D& A, const Coord3D& B, dbl theta)
    {
        for (uint i=0; i<number_of_parts; i++)
            _parts[i].ABrotate(A, B, theta);
    }


/// Euler rotation (Attract convention)
    void AttractEulerRotate(dbl phi, dbl ssi, dbl rot)
    {
        for (uint i=0; i<number_of_parts; i++)
            _parts[i].AttractEulerRotate(phi, ssi, rot);
    }



};


class Pdb{

    std::vector<Model> _models;

public:

    Pdb(const std::string& pdbname);
    uint nbmodels(){return _models.size();};

    Model operator[](int i) {if (i>=(int)0 && i<(int)_models.size()) return _models[i]; else throw std::range_error("model out of bounds");};


};



void ReadPDB(std::ifstream& fichier,Rigidbody& protein ); ///< read a PDB file from a file pointer and load datas in Rigidbody
void ReadPDB(const std::string name,Rigidbody& protein ); ///< read a PDB file from a filename and load datas in Rigidbody
void WritePDB(const Rigidbody& rigid, std::string filename); ///< write a PDB file given a Rigidbody and a filename

}

#endif //#ifndef PDBIO_H


