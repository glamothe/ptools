
#include "basetypes.h"
#include "cython_wrappers.h"
#include "atom.h"

void cy_copy_array2d(Matrix * source, Matrix * dest)
{
 *dest = *source;
}

void cy_copy_atom(Atom* source, Atom* dest)
{
 *dest = *source ;
}