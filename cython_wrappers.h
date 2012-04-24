#ifndef _CYTHON_WRAPPERS_H_
#define _CYTHON_WRAPPERS_H_

#include "atom.h"
#include "basetypes.h"
using namespace PTools;

void cy_copy_array2d(Matrix * source, Matrix * dest);
void cy_copy_atom(Atom* source, Atom* dest);

#endif