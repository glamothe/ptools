
#include "basetypes.h"
#include "cython_wrappers.h"

void cy_copy_array2d(Matrix * source, Matrix * dest)
{
 *dest = *source;
}