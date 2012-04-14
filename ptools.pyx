from cython.operator cimport dereference as deref

cdef extern from "coord3d.h" namespace "PTools":
    cdef cppclass CppCoord3D  "PTools::Coord3D":  
        double x, y, z
        CppCoord3D()
        CppCoord3D(double nx, double ny, double nz)
        bint operator==(CppCoord3D&)

    cdef CppCoord3D operator+ (CppCoord3D& A, CppCoord3D& B)
    cdef CppCoord3D operator* (double scal, CppCoord3D A)
    
    cdef double Norm(CppCoord3D&)
        
cdef makeCoord3D(CppCoord3D c):
    cdef Coord3D result = Coord3D(c.x, c.y, c.z)
    return result
        
        
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
    
    def __add__(Coord3D self, Coord3D other):
        
          cdef CppCoord3D cppself = deref(self.thisptr)
          cdef CppCoord3D cppother = deref(other.thisptr)
          cdef CppCoord3D cppresult = cppself+cppother
        
          cdef Coord3D result = Coord3D(cppresult.x, cppresult.y, cppresult.z)
          return result
          
    def __mul__(Coord3D self, double scal):
        
         cdef CppCoord3D r = scal*deref(self.thisptr)
         return makeCoord3D(r)
         
    def __str__(self):
        return "%f %f %f"%(self.x, self.y, self.z)
        
          
def norm(Coord3D v):
    return Norm(deref(v.thisptr))

    