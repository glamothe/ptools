from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "scorpionforcefield.h" namespace "PTools":
    cdef cppclass CppScorpionForceField "PTools::ScorpionForceField":
       CppScorpionForceField(string&, double)
       void AddLigand(CppAttractRigidbody&)
       double Function(vector[double]&)
       double getVdw()
       double getCoulomb()
       double nonbon8(CppAttractRigidbody& , CppAttractRigidbody& , CppAttractPairList & , int) 


cdef class ScorpionForceField:
   
    cdef CppScorpionForceField* thisptr


    def __cinit__(self, filename, cutoff):
        cdef char* c_filename
        cdef string * cppname

        c_filename = <char*> filename
        cppname = new string(c_filename)
        self.thisptr = new CppScorpionForceField(deref(cppname), cutoff)
        del cppname

    def __dealloc__(self):
        del self.thisptr

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

    def nonbon8(self, AttractRigidbody rec, AttractRigidbody lig, AttractPairList pl, verbose=False):
        return self.thisptr.nonbon8(deref(rec.thisptr), deref(lig.thisptr), deref(pl.thisptr), verbose)