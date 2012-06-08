from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "scorpionforcefield.h" namespace "PTools":
    cdef cppclass CppScorpionForceField "PTools::ScorpionForceField":
       CppScorpionForceField(string&, double)
       void AddLigand(CppAttractRigidbody&)
       double Function(vector[double]&)
       double getVdw()
       double getCoulomb()


cdef class ScorpionForceField:
   
    cdef CppScorpionForceField* thisptr


    def __cinit__(self, filename, cutoff):
        cdef char* c_filename
        cdef string * cppname

        c_filename = <char*> filename
        cppname = new string(c_filename)
        
        self.thisptr = new CppScorpionForceField(deref(cppname), cutoff)

    def AddLigand(self, AttractRigidbody rig):
        self.thisptr.AddLigand(deref(rig.thisptr))

    def Function(self, vec):
        cdef vector[double] v
        for el in vec:
           v.push_back(el)

        return self.thisptr.Function(v)
        
    def getVdw(self):
        return self.thisptr.getVdw()
    
    def getCoulomb(self):
        return self.thisptr.getCoulomb()
