cdef extern from "BasePair.h" namespace "PTools":
    cdef cppclass CppBasePair "PTools::BasePair":
        CppBasePair(string)
        CppBasePair(CppRigidbody&)
        CppBasePair(CppBasePair&)
        CppRigidbody GetRigidBody()


cdef class BasePair:
    cdef CppBasePair* thisptr

    def __cinit__(self,arg=None):
        cdef string fn

        if arg is not None:
            if isinstance(arg, str):
                fn = <string?> arg
                self.thisptr = new CppBasePair(fn)
            elif isinstance(arg, Rigidbody):
                rb = <Rigidbody> arg
                self.thisptr = new CppBasePair(deref(rb.thisptr))
            else:
                raise RuntimeError("unknown arg type")

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
