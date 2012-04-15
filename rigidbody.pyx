from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "rigidbody.h" namespace "PTools":
    cdef cppclass CppRigidbody "PTools::Rigidbody":
        CppRigidbody(string)
        CppRigidbody()
        CppRigidbody(CppRigidbody&)
        unsigned int Size()
        CppCoord3D GetCoords(unsigned int)
        void ABrotate(CppCoord3D&, CppCoord3D&, double)
        void Translate(CppCoord3D&)

#cdef CppRigidbody* copy_rigidbody(newptr, ptr):
#    cdef CppRigidbody *result
#        result = new CppRigidbody (ptr)
#        return result


cdef class Rigidbody:
    cdef CppRigidbody* thisptr

    def __cinit__(self, filename=''):
        cdef CppRigidbody* oldrigidptr
        cdef Rigidbody  oldrigid
        print type(filename)
        if isinstance(filename, str):
            if filename ==  '':
                self.thisptr = new CppRigidbody()
            else:
                print "there is a filename, loading the pdb file"
                self.thisptr = _getRigidbody_from_py_name(filename)
        if isinstance(filename, Rigidbody):
            oldrigid = <Rigidbody> filename
            oldrigidptr = <CppRigidbody*> (oldrigid.thisptr)
            
            self.thisptr = new CppRigidbody(deref(oldrigidptr)  )
            if not self.thisptr:
                print "FATAL: this should never happen"

           
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
    def Translate(self, Coord3D tr):
        if self.thisptr:
            print "rigidbody thisptr in not null"
        if tr.thisptr:
            print "Coord3D thisptr in not null"
            
        self.thisptr.Translate(deref(tr.thisptr))
        
#    def ABrotate(self, Coord3D A, Coord3D B, double theta):
#        self.thisptr.ABrotate(deref(A.thisptr),deref(B.thisptr),theta)
#        return None

        
cdef CppRigidbody* _getRigidbody_from_py_name(pyname):
    cdef char* name = pyname
    cdef string *cppname = new string(name)
    cdef CppRigidbody *newrigid = new CppRigidbody(deref(cppname))
    return newrigid
    

def c_to_python_string():
    cdef char * test = "hello world"
    cdef bytes b = test
    return b
