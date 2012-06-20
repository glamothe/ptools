from cython.operator cimport dereference as deref
from libcpp.string cimport string


cdef extern from "pairlist.h" namespace "PTools":
    cdef cppclass CppAttractPairList "PTools::AttractPairList":
       CppAttractPairList()
       CppAttractPairList(CppAttractRigidbody &, CppAttractRigidbody &, double )
       CppAttractPairList(CppAttractRigidbody &, CppAttractRigidbody & )



cdef class AttractPairList:
    cdef CppAttractPairList * thisptr

    def __cinit__(self, *args):
        self.thisptr = <CppAttractPairList*> 0
        if len(args) == 0: 
            self.thisptr = new CppAttractPairList()
        if isinstance(args[0], AttractRigidbody) and isinstance(args[1], AttractRigidbody):
            r1 = <AttractRigidbody> args[0]
            r2 = <AttractRigidbody> args[1]

            if len(args) == 2:
               self.thisptr = new CppAttractPairList(deref(r1.thisptr), deref(r2.thisptr))
               return
            if len(args) == 3 :
               self.thisptr = new CppAttractPairList(deref(r1.thisptr), deref(r2.thisptr), <double?> args[2] )
               return
        raise TypeError("arguments error in AttractPairList.__cinit__()")
          

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr



       