
cdef extern from "DNA.h" namespace "PTools":
    cdef cppclass CppDNA "PTools::DNA":
        CppDNA()

cdef class DNA:
    cdef CppDNA* thisptr

    def __cinit__(self):
        self.thisptr = new CppDNA()

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <CppDNA*> 0
