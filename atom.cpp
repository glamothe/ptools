#include "atom.h"
#include "coord3d.h"
#include "rigidbody.h"

namespace PTools{

coord3D Atom::GetCoords() const {if (!hasOwner) return mCoords; else return mOwner->GetCoord(mIndex); };


}

