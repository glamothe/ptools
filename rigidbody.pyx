from cython.operator cimport dereference as deref
from libcpp.string cimport string

    
cdef extern from "rigidbody.h" namespace "PTools":
    cdef cppclass CppRigidbody "PTools::Rigidbody":
        CppRigidbody(string)
        CppRigidbody()
        unsigned int Size()
        CppCoord3D GetCoords(unsigned int)
        

cdef class Rigidbody:
    cdef CppRigidbody* thisptr
    
    def __cinit__(self, filename=''):
        if filename ==  '':
            self.thisptr = new CppRigidbody()
        else:
            self.thisptr = _getRigidbody_from_py_name(filename)
           
    def __dealloc__(self):
        del self.thisptr
    def __len__(self):
        return self.thisptr.Size()
    def getCoords(self, unsigned int i):
        cdef Coord3D c = Coord3D () 
        cdef CppCoord3D cpp = self.thisptr.GetCoords(i)
        c.x = cpp.x
        c.y = cpp.y
        c.z = cpp.z
        return c


        
cdef CppRigidbody* _getRigidbody_from_py_name(pyname):
    cdef char* name = pyname
    cdef string *cppname = new string(name)
    cdef CppRigidbody *newrigid = new CppRigidbody(deref(cppname))
    return newrigid
    

def c_to_python_string():
    cdef char * test = "hello world"
    cdef bytes b = test
    return b