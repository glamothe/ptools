#ifndef PDBPARSER_H
#define PDBPARSER_H

#include <cctype>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>


#include "rigidbody.h"


namespace PTools
{

void readPDB(std::ifstream& fichier,Rigidbody& protein );
void readPDB(const std::string name,Rigidbody& protein );



}

#endif //#ifdef PDBPARSER_H