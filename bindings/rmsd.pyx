cdef extern from "rmsd.h" namespace "PTools":
    cdef double CppRmsd "PTools::Rmsd" (CppRigidbody& r1, CppRigidbody& r2) except +ValueError

    
def Rmsd(Rigidbody r1, Rigidbody r2):
    cdef CppRigidbody* er1 = r1.thisptr
    cdef CppRigidbody* er2 = r2.thisptr
    return CppRmsd(deref(er1), deref(er2))
    

        