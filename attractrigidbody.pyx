from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "attractrigidbody.h" namespace "PTools":
    cdef cppclass CppAttractRigidbody "PTools::AttractRigidbody":
        CppAttractRigidbody()
        CppAttractRigidbody(string&)  #filename
        CppAttractRigidbody(CppRigidbody&)

        unsigned int getAtomTypeNumber(unsigned int)

        double getCharge(unsigned int)

        int isAtomActive(unsigned int)

        void resetForces()
        #void addForces(  #TODO: later
 
        void setRotation(bool)
        void setTranslation(bool)

        unsigned int Size()



cdef CppAttractRigidbody* _getAttractRigidbody_from_py_name(pyname):
    cdef char* name = pyname
    cdef string *cppname = new string(name)
    cdef CppAttractRigidbody *newrigid = new CppAttractRigidbody(deref(cppname))
    del cppname
    return newrigid


cdef class AttractRigidbody:
    cdef CppAttractRigidbody* thisptr

    def __cinit__(self, arg):
        if isinstance(arg, Rigidbody):
           rigidbody = <Rigidbody> arg
           rigidbodyptr = <CppRigidbody*> rigidbody.thisptr
           self.thisptr = new CppAttractRigidbody(deref(rigidbodyptr))
           return
        elif isinstance(arg, str):
           self.thisptr = _getAttractRigidbody_from_py_name(arg)
           return
        else:
           print "Should never reach here(attractrigidbody.pyx:AttractRigidbody:__cinit__)"


    def __dealloc__(self):
       del self.thisptr

    
    def getAtomTypeNumber(self, atomid):
         return self.thisptr.getAtomTypeNumber(atomid)

    def getCharge(self, atomid):
         return self.thisptr.getCharge(atomid)

    #void setRotation(bool)
    def setRotation(self, flag):
        self.thisptr.setRotation(flag)

    #void setTranslation(bool)
    def setTranslation(self, flag):
        self.thisptr.setTranslation(flag)

    def isAtomActive(self, atomid):
        return self.thisptr.isAtomActive(atomid)

    #void resetForces()
    def resetForces(self):
        self.thisptr.resetForces()

    
    def Size(self):
        return self.thisptr.Size()