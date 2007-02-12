#ifndef PDBIO_H
#define PDBIO_H

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>

#include "rigidbody.h"

namespace PTools
{

void readPDB(std::ifstream& fichier,Rigidbody& protein ); ///< read a PDB file from a file pointer and load datas in Rigidbody
void readPDB(const std::string name,Rigidbody& protein ); ///< read a PDB file from a filename and load datas in Rigidbody
void writePDB(const Rigidbody& rigid, std::string filename); ///< write a PDB file given a Rigidbody and a filename

}

#endif //#ifndef PDBIO_H


