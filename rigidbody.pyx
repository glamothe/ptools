from cython.operator cimport dereference as deref
from libcpp.string cimport string

    
cdef extern from "rigidbody.h" namespace "PTools":
    cdef cppclass CppRigidbody "PTools::Rigidbody":
        CppRigidbody(string)
        CppRigidbody()
        unsigned int Size()
        

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

        
cdef CppRigidbody* _getRigidbody_from_py_name(pyname):
    cdef char* name = pyname
    cdef string *cppname = new string(name)
    cdef CppRigidbody *newrigid = new CppRigidbody(deref(cppname))
    return newrigid
    

def c_to_python_string():
    cdef char * test = "hello world"
    cdef bytes b = test
    return b