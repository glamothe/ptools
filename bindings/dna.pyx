
cdef extern from "DNA.h" namespace "PTools":
    cdef cppclass CppDNA "PTools::DNA":
        CppDNA()
        CppDNA(CppDNA&)
        CppDNA SubDNA(int, int)

cdef class DNA:
    cdef CppDNA* thisptr

    def __cinit__(self):
        self.thisptr = new CppDNA()

    def __dealloc__(self):
        if self.thisptr:
            del self.thisptr
            self.thisptr = <CppDNA*> 0

    def SubDNA(self, int start, int end):
        ret = DNA()
        if ret.thisptr:
            del ret.thisptr
        cdef CppDNA cdna = self.thisptr.SubDNA(start, end)
        ret.thisptr = new CppDNA(cdna)
        return ret
