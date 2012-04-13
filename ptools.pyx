from cython.operator cimport dereference as deref

cdef extern from "coord3d.h" namespace "PTools":
    cdef cppclass CppCoord3D  "PTools::Coord3D":  
        double x, y, z
        CppCoord3D()
        CppCoord3D(double nx, double ny, double nz)
        bint operator==(CppCoord3D&)

cdef class Coord3D:
    cdef CppCoord3D *thisptr
    def __cinit__(self, x=0, y=0, z=0):
        self.thisptr = new CppCoord3D(x,y,z)
    def __dealloc__(self):
        del self.thisptr

    def _compare(self, other):
        cdef Coord3D tmp = <Coord3D> other
        cdef CppCoord3D myself = deref(self.thisptr)
        cdef CppCoord3D tocompare = deref(tmp.thisptr)
        return myself == tocompare
 
    def __richcmp__(self, other, b):
        return self._compare(other)

    
    
        
    property x:
       def __get__(self): return self.thisptr.x
       def __set__(self,x): self.thisptr.x = x
    property y:
       def __get__(self): return self.thisptr.y
       def __set__(self,y): self.thisptr.y = y
    property z:
       def __get__(self): return self.thisptr.z
       def __set__(self,z): self.thisptr.z = z
    
    def __add__(self, other):
        cdef Coord3D c
        c = Coord3D()
        c.x = self.x + other.x
        c.y = self.y + other.y
        c.z = self.z + other.z
        return c
    