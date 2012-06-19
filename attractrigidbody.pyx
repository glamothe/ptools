from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "attractrigidbody.h" namespace "PTools":
    cdef cppclass CppAttractRigidbody "PTools::AttractRigidbody":
        CppAttractRigidbody()
        CppAttractRigidbody(string&)  #filename
        CppAttractRigidbody(CppRigidbody&)
        CppAttractRigidbody(CppAttractRigidbody&)

        unsigned int getAtomTypeNumber(unsigned int)

        double getCharge(unsigned int)

        int isAtomActive(unsigned int)

        void resetForces()
        #void addForces(  #TODO: later
 
        void setRotation(int)
        void setTranslation(int)
        CppCoord3D FindCenter()

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
        elif isinstance(arg, AttractRigidbody):
           oldrigidbody = <AttractRigidbody> arg
           oldrigidbody_ptr = <CppAttractRigidbody*> oldrigidbody.thisptr
           self.thisptr = new CppAttractRigidbody(deref(oldrigidbody_ptr) )          
           return
        else: 
           print "Should never reach here(attractrigidbody.pyx:AttractRigidbody:__cinit__)"
           print arg


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
        
    def FindCenter(self):
        cdef CppRigidbody* rig = <CppRigidbody*> self.thisptr
        cdef CppCoord3D* co = new CppCoord3D (rig.FindCenter())
        ret = Coord3D()
        del ret.thisptr
        ret.thisptr = co
        
        return ret
        
    def Translate(self, Coord3D co):
        cdef CppRigidbody* rig = <CppRigidbody*> self.thisptr
        rig.Translate(deref(co.thisptr))
        
    def  AttractEulerRotate(self, double phi, double ssi, double rot):
        cdef CppRigidbody* rig = <CppRigidbody*> self.thisptr
        rig.AttractEulerRotate(phi, ssi, rot)
        