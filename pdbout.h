#ifndef PDBOUT_H
#define PDBOUT_H

#include <string>

#include "rigidbody.h"

namespace PTools{

void WritePDB(const Rigidbody& rigid, std::string filename); ///< write a PDB file given a Rigidbody and a filename

}

#endif //#ifndef PDBOUT_H