from cython.operator cimport dereference as deref

cdef extern from "coord3d.h" namespace "PTools":
    cdef cppclass Coord3D:  
        double x, y, z
        Coord3D()
        Coord3D(double nx, double ny, double nz)
        bint operator==(Coord3D&)

cdef class PyCoord3D:
    cdef Coord3D *thisptr
    def __cinit__(self, x=0, y=0, z=0):
        self.thisptr = new Coord3D(x,y,z)
    def __dealloc__(self):
        del self.thisptr

    def _compare(self, other):
        cdef PyCoord3D tmp = <PyCoord3D> other
        cdef Coord3D myself = deref(self.thisptr)
        cdef Coord3D tocompare = deref(tmp.thisptr)
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
        cdef PyCoord3D c
        c = PyCoord3D()
        c.x = self.x + other.x
        c.y = self.y + other.y
        c.z = self.z + other.z
        return c
    