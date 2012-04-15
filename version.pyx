cdef extern from "version.h" :
    cdef cppclass CppVersion "Version":
        string revid

cdef class Version:
    cdef CppVersion * thisptr
    def __cinit__(self):
        self.thisptr = new CppVersion()
    def __dealloc__(self):
        del self.thisptr
    
    property revid:
        def __get__(self):
            cdef bytes pyrevid = <bytes?> self.thisptr.revid.c_str() #not sure if this one is safe...
            return pyrevid
