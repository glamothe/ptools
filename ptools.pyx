cdef extern from "coord3d.h" namespace "PTools":
    cdef cppclass CppCoord3D  "PTools::Coord3D":  
        double x, y, z
        CppCoord3D()
        CppCoord3D(double nx, double ny, double nz)
        int operator==(CppCoord3D&)
        

cdef class Coord3D:
    cdef CppCoord3D *thisptr
    def __cinit__(self):
        self.thisptr = new CppCoord3D()
    def __dealloc__(self):
        del self.thisptr
    