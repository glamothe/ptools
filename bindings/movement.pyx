# distutils: language = c++


cdef extern from "Movement.h" namespace "PTools":
    cdef cppclass CppMovement "PTools::Movement":
        CppMovement() except+
        CppMovement(Array2D[double] &)
        Array2D[double] m
        void Apply(CppRigidbody&)


cdef class Movement:
    cdef CppMovement *thisptr
    def __cinit__(self):
        self.thisptr = new CppMovement()
    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
    def Apply(self, Rigidbody rigid):
        self.thisptr.Apply(deref(rigid.thisptr))
