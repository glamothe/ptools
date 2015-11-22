cdef extern from "BasePair.h" namespace "PTools":
    cdef cppclass CppBasePair "PTools::BasePair":
        CppBasePair(string)
        CppBasePair(CppRigidbody&)
        CppBasePair(CppBasePair&)
        CppRigidbody GetRigidBody()


cdef class BasePair:
    cdef CppBasePair* thisptr

    def __cinit__(self,filename=None):
        cdef string fn
        if filename is not None:
            fn = <string?> fn
            self.thisptr = new CppBasePair(fn)

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <CppBasePair*> 0

    def GetRigidBody(self):
        ret = Rigidbody()
        if ret.thisptr:
            del ret.thisptr
        cdef CppRigidbody rig
        ret.thisptr = new CppRigidbody(rig)
        return ret
