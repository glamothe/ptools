from cython.operator cimport dereference as deref
from libcpp.string cimport string

cdef extern from "<vector>" namespace "std":
    cdef cppclass vector[T]:
        cppclass iterator:
            T operator*()
            iterator operator++()
            bint operator==(iterator)
            bint operator!=(iterator)
        vector()
        void push_back(T&)
        T& operator[](int)
        T& at(int)
        iterator begin()
        iterator end()



cdef extern from "attractforcefield.h" namespace "PTools":
    cdef cppclass CppAttractForceField2 "PTools::AttractForceField2":
       CppAttractForceField2(string&, double)
       void AddLigand(CppAttractRigidbody&)
       double Function(vector[double]&)
       double getVdw()
       double getCoulomb()


cdef class AttractForceField2:
   
    cdef CppAttractForceField2* thisptr


    def __cinit__(self, filename, cutoff):
        cdef char* c_filename
        cdef string * cppname

        c_filename = <char*> filename
        cppname = new string(c_filename)
        
        self.thisptr = new CppAttractForceField2(deref(cppname), cutoff)

    def AddLigand(self, AttractRigidbody rig):
        self.thisptr.AddLigand(deref(rig.thisptr))

    def Function(self, vec):
        cdef vector[double] v
        for el in vec:
           v.push_back(el)

        return self.thisptr.Function(v)